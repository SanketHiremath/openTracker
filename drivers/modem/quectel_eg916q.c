/*
 * SPDX-License-Identifier: Apache-2.0
 *
 * Quectel EG916Q-GL modem driver (AT UART, native embedded TCP/IP + MQTT).
 *
 * Written against Zephyr's public modem subsystem headers
 * (zephyr/modem/chat.h, pipe.h, backend/uart.h) -- the same primitives
 * zephyr/drivers/modem/modem_cellular.c is itself built from, and
 * independent of CONFIG_MODEM_CELLULAR. Nothing under zephyr/ is modified,
 * so this survives `west update`.
 *
 * Deliberately simpler than modem_cellular.c: this board wires exactly one
 * EG916Q-GL, whose GNSS output is on its OWN dedicated UART (see the gnss
 * node in app.overlay) -- so there is no need for CMUX to multiplex traffic
 * onto a single shared UART, the way modem_cellular.c must for modems that
 * only expose one channel.
 *
 * Networking uses the module's own embedded TCP/IP and MQTT AT-command
 * stack (AT+QIACT to bring up the PDP context's IP layer, AT+QMTOPEN/
 * AT+QMTCONN/AT+QMTPUBEX for MQTT-over-TLS) rather than PPP + a host-side
 * TLS/MQTT stack. This driver used to dial PPP (ATD*99#) and hand the link
 * to Zephyr's own net stack; that was dropped in favor of the module's
 * native stack per quectel_eg800qeg91xq_series_tcpip_application_note_v1-3.md
 * ("This significantly reduces the dependence on external PPP and TCP/IP
 * protocol stacks") and quectel_eg800qeg91xq_series_mqtt_application_note_v1-3.md.
 * Consequences: no PPP net_if exists for this modem at all now, and nothing
 * in the app calls PM_DEVICE_ACTION_RESUME/SUSPEND on it any more -- every
 * public entry point below manages power itself via
 * eg916q_ensure_powered_on()/eg916q_power_off_and_release().
 * eg916q_selftest() is the one exception to "self-contained": on success it
 * deliberately leaves the module powered on for the next call to reuse
 * (see its doc comment in the header) instead of tearing down a session
 * that's about to be immediately rebuilt.
 *
 * Several AT command specifics here (AT+QSSLCFG's exact parameter
 * semantics, AT+QFUPL's upload protocol) are inferred from general Quectel
 * conventions, not confirmed against this module's own SSL/FILE application
 * notes (referenced by the MQTT note as documents [1]/[2], not available at
 * the time this was written) -- verify against real hardware behavior.
 *
 * Power management is synchronous: every public entry point below blocks
 * the calling thread until the modem is ready (or, for eg916q_selftest(),
 * fully up), rather than kicking off an async workqueue state machine the
 * way modem_cellular.c does. That fits this codebase: the only caller
 * (modem_link.cpp, invoked from the dedicated cellular_task thread) exists
 * specifically to own long blocking I/O -- see CLAUDE.md's Cellular
 * section.
 *
 * AT command syntax for the non-networking commands (ATE0, AT+CMEE,
 * AT+CFUN, AT+CGDCONT, AT+CGREG/AT+CEREG, AT+QPOWD) has been checked
 * line-by-line against quectel_eg800qeg91xq_series_at_commands_manual_v1-1.md,
 * which explicitly covers this module family.
 *
 * PWRKEY pulse timing (CONFIG_MODEM_QUECTEL_EG916Q_POWER_PULSE_MS) is
 * confirmed against the EG916Q-GL hardware design manual: PWRKEY must be
 * held low for at least 10s to power on the module, hence the 10000 default.
 * RESET_N timing remains unconfirmed. Enable CONFIG_MODEM_MODULES_LOG_LEVEL_DBG
 * to see the raw AT exchange while debugging.
 */

#define DT_DRV_COMPAT quectel_eg916q

#include <stdlib.h>
#include <string.h>

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/modem/chat.h>
#include <zephyr/modem/pipe.h>
#include <zephyr/modem/backend/uart.h>

#include "quectel_eg916q.h"

#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(modem_quectel_eg916q, CONFIG_MODEM_QUECTEL_EG916Q_LOG_LEVEL);

#define EG916Q_CHAT_BUF_SIZE  128
#define EG916Q_CHAT_ARGV_SIZE 32

/* Internal identifiers for the native TCP/IP + MQTT AT commands. Only one
 * PDP context, SSL context and MQTT client are ever used, so these are
 * fixed rather than parameters -- there is nothing to pick between.
 */
#define EG916Q_PDP_CONTEXT_ID 1
#define EG916Q_SSL_CTX_IDX    2
#define EG916Q_MQTT_CLIENT_IDX 0

struct eg916q_config {
	const struct device *uart;
	struct gpio_dt_spec power_gpio;
	struct gpio_dt_spec reset_gpio;
};

struct eg916q_data {
	struct modem_pipe *uart_pipe;
	struct modem_backend_uart uart_backend;
	uint8_t uart_backend_receive_buf[CONFIG_MODEM_QUECTEL_EG916Q_UART_BUFFER_SIZE];
	uint8_t uart_backend_transmit_buf[CONFIG_MODEM_QUECTEL_EG916Q_UART_BUFFER_SIZE];

	struct modem_chat chat;
	uint8_t chat_receive_buf[EG916Q_CHAT_BUF_SIZE];
	uint8_t *chat_argv[EG916Q_CHAT_ARGV_SIZE];

	/* Written by the modem_chat callbacks below, called synchronously
	 * from within modem_chat_run_script() -- there's no other channel
	 * back to the caller. Each is read by its corresponding
	 * eg916q_*() function right after the relevant script returns.
	 */
	uint8_t last_csq_rssi;
	int last_qmtopen_result;
	int last_qmtconn_result;
	int last_qmtconn_ret_code;
	int last_qmtpubex_result;
	int last_qping_result;
	int last_qiopen_result;
	int last_qird_len;
	int last_reg_stat;
	/* Raw text after "+CPIN: " (e.g. "READY", "SIM PIN", "SIM PUK"),
	 * captured so a locked/missing SIM shows up as an explicit state
	 * instead of a bare timeout -- see eg916q_chat_on_cpin().
	 */
	char last_cpin_state[16];

	/* True once eg916q_ensure_powered_on() has powered the module on and
	 * attached chat, false once eg916q_power_off_and_release() has torn
	 * that down. See eg916q_ensure_powered_on()'s doc comment for why
	 * this exists: the self-test intentionally leaves the module powered
	 * on for the next real operation to reuse, rather than power-cycling
	 * on every single call.
	 */
	bool powered_on;
};

static bool eg916q_gpio_is_enabled(const struct gpio_dt_spec *spec)
{
	return spec->port != NULL;
}

/* ---------------------------------------------------------------------
 * Chat matches and scripts
 * ------------------------------------------------------------------ */

MODEM_CHAT_MATCH_DEFINE(eg916q_ok_match, "OK", "", NULL);

/* Catch-all: an empty pattern matches any complete line that no running
 * script step or abort match claimed (modem_chat_parse_find_catch_all_match()
 * in subsys/modem/modem_chat.c). Without it those lines are dropped silently,
 * which hid URCs like "+QMTSTAT: <idx>,<err>" (MQTT link closed) and left
 * failures showing up only as a bare script timeout. Scoped to this driver's
 * chat instance, so GNSS NMEA traffic (a separate modem_chat) isn't logged.
 */
static void eg916q_chat_on_unhandled(struct modem_chat *chat, char **argv, uint16_t argc,
				     void *user_data)
{
	ARG_UNUSED(chat);
	ARG_UNUSED(user_data);

	if (argc >= 2) {
		LOG_WRN("modem: %s", argv[1]);
	}
}

MODEM_CHAT_MATCHES_DEFINE(eg916q_unsol_matches, MODEM_CHAT_MATCH("", "", eg916q_chat_on_unhandled));

/* Bare, undelimited prompt the module returns before it expects raw
 * (non-AT-escaped) bytes on the wire (AT+QISEND's '>', AT+QMTPUBEX's '>') --
 * unreliable to match via modem_chat itself, since it requires a CRLF to
 * complete any line and this prompt often has none; see
 * eg916q_http_get()'s doc comment. Still used by eg916q_network_selftest()
 * below (its own, currently unexercised copy of the HTTP GET path).
 */
MODEM_CHAT_MATCH_DEFINE(eg916q_prompt_match, ">", "", NULL);

MODEM_CHAT_MATCHES_DEFINE(eg916q_abort_matches, MODEM_CHAT_MATCH("ERROR", "", NULL),
			  MODEM_CHAT_MATCH_WILDCARD("+CME ERROR: *", "", NULL),
			  MODEM_CHAT_MATCH_WILDCARD("+CMS ERROR: *", "", NULL));

/* ATE0: disable echo. AT+CMEE=1: numeric error codes (gives the abort
 * matches above something concrete to match). AT+CFUN=1: full
 * functionality. AT+CGDCONT: set the default PDP context APN (the 3GPP
 * default-bearer auto-activation the TCP/IP app note describes uses this
 * context; AT+QICSGP below additionally configures the same context for the
 * native TCP/IP stack specifically). AT+CGREG=1 / AT+CEREG=1: enable
 * registration status reporting -- polled explicitly by
 * eg916q_wait_registered() below, not consumed as unsolicited codes; this
 * driver has no URC handling, by design (see the header comment).
 */
MODEM_CHAT_SCRIPT_CMDS_DEFINE(
	eg916q_init_chat_script_cmds, MODEM_CHAT_SCRIPT_CMD_RESP("AT", eg916q_ok_match),
	MODEM_CHAT_SCRIPT_CMD_RESP("ATE0", eg916q_ok_match),
	MODEM_CHAT_SCRIPT_CMD_RESP("AT+CMEE=1", eg916q_ok_match),
	MODEM_CHAT_SCRIPT_CMD_RESP("AT+CFUN=1", eg916q_ok_match),
	MODEM_CHAT_SCRIPT_CMD_RESP("AT+CGDCONT=" STRINGIFY(EG916Q_PDP_CONTEXT_ID) ",\"IP\",\"" CONFIG_MODEM_QUECTEL_EG916Q_APN "\"",
				   eg916q_ok_match),
	MODEM_CHAT_SCRIPT_CMD_RESP("AT+CGREG=1", eg916q_ok_match),
	MODEM_CHAT_SCRIPT_CMD_RESP("AT+CEREG=1", eg916q_ok_match));

/* 30s for the whole script (modem_chat's timeout covers all steps, not
 * each one): seven commands, and AT+CFUN=1 alone can take several seconds
 * while the radio initializes.
 */
MODEM_CHAT_SCRIPT_DEFINE(eg916q_init_chat_script, eg916q_init_chat_script_cmds,
			 eg916q_abort_matches, NULL, 30);

/* Registration poll. Both +CEREG (E-UTRAN/LTE) and +CGREG (GERAN/UTRAN
 * packet service, 3GPP TS 27.007) are polled -- this is an LTE Cat 1 module,
 * and +CGREG is not guaranteed to reflect LTE attach the way +CEREG is.
 * eg916q_wait_registered() below accepts either.
 *
 * Read-command reply: "+CxREG: <n>,<stat>[,...]" -- <n> is the URC mode the
 * init script set (1), <stat> the registration state. This parses <stat>
 * rather than only matching the two "registered" strings: an earlier
 * version matched only "+CxREG: 1,1"/"1,5", so every other state silently
 * timed out and the log showed nothing but "timed out" -- no way to tell
 * "still searching, wait longer" (2) from "registration denied" (3, e.g.
 * bad APN/SIM not provisioned) or "not searching" (0).
 */
static void eg916q_chat_on_creg(struct modem_chat *chat, char **argv, uint16_t argc,
				void *user_data)
{
	ARG_UNUSED(chat);
	struct eg916q_data *data = (struct eg916q_data *)user_data;

	if (argc < 3) {
		return;
	}

	data->last_reg_stat = atoi(argv[2]);
}

MODEM_CHAT_MATCH_DEFINE(eg916q_cereg_match, "+CEREG: ", ",", eg916q_chat_on_creg);
MODEM_CHAT_MATCH_DEFINE(eg916q_cgreg_match, "+CGREG: ", ",", eg916q_chat_on_creg);

MODEM_CHAT_SCRIPT_CMDS_DEFINE(eg916q_cereg_poll_chat_script_cmds,
			      MODEM_CHAT_SCRIPT_CMD_RESP("AT+CEREG?", eg916q_cereg_match),
			      MODEM_CHAT_SCRIPT_CMD_RESP("", eg916q_ok_match));

MODEM_CHAT_SCRIPT_DEFINE(eg916q_cereg_poll_chat_script, eg916q_cereg_poll_chat_script_cmds,
			 eg916q_abort_matches, NULL, 5);

MODEM_CHAT_SCRIPT_CMDS_DEFINE(eg916q_cgreg_poll_chat_script_cmds,
			      MODEM_CHAT_SCRIPT_CMD_RESP("AT+CGREG?", eg916q_cgreg_match),
			      MODEM_CHAT_SCRIPT_CMD_RESP("", eg916q_ok_match));

MODEM_CHAT_SCRIPT_DEFINE(eg916q_cgreg_poll_chat_script, eg916q_cgreg_poll_chat_script_cmds,
			 eg916q_abort_matches, NULL, 5);

/* SIM check -- the first step of the TCP/IP app note's flow chart (Figure
 * 1): "Query (U)SIM card status by AT+CPIN? ... fails to identify (U)SIM
 * card in 20s" -> give up. "+CPIN: READY" is the only state registration can
 * proceed from; no SIM typically replies "+CME ERROR: 10", caught by the
 * abort matches.
 *
 * This match's prefix/separator ("+CPIN: ", "") accepts any state text as a
 * successful line match (READY, SIM PIN, SIM PUK, ...) -- unlike an earlier
 * version that only matched the literal string "+CPIN: READY", so a
 * PIN-locked SIM's "+CPIN: SIM PIN" reply matched nothing at all and the
 * whole script just timed out with no way to tell "SIM PIN required" apart
 * from "no SIM inserted"/"module not responding". The callback records the
 * actual state text in data->last_cpin_state; eg916q_wait_sim_ready() below
 * is what actually decides success/failure by checking it against "READY".
 */
static void eg916q_chat_on_cpin(struct modem_chat *chat, char **argv, uint16_t argc,
				void *user_data)
{
	ARG_UNUSED(chat);
	struct eg916q_data *data = (struct eg916q_data *)user_data;

	if (argc < 2) {
		return;
	}

	strncpy(data->last_cpin_state, argv[1], sizeof(data->last_cpin_state) - 1);
	data->last_cpin_state[sizeof(data->last_cpin_state) - 1] = '\0';
}

MODEM_CHAT_MATCH_DEFINE(eg916q_cpin_match, "+CPIN: ", "", eg916q_chat_on_cpin);

MODEM_CHAT_SCRIPT_CMDS_DEFINE(eg916q_cpin_chat_script_cmds,
			      MODEM_CHAT_SCRIPT_CMD_RESP("AT+CPIN?", eg916q_cpin_match),
			      MODEM_CHAT_SCRIPT_CMD_RESP("", eg916q_ok_match));

MODEM_CHAT_SCRIPT_DEFINE(eg916q_cpin_chat_script, eg916q_cpin_chat_script_cmds,
			 eg916q_abort_matches, NULL, 5);

/* AT+QPOWD=1: Quectel normal power-down. This is the only deterministic way
 * this driver powers the module off -- PWRKEY is a momentary trigger, not a
 * level-sensed enable line, so releasing it afterward (the callers' plain
 * gpio_pin_set_dt(power_gpio, 0)) does NOT itself power the module off; it's
 * only there to leave the GPIO in its idle/released state. If this script
 * fails/times out, the module's actual power state is left unknown -- see
 * the callers' comments for what that risks (the next PWRKEY pulse could
 * toggle it further from off instead of turning it on).
 */
MODEM_CHAT_SCRIPT_CMDS_DEFINE(eg916q_shutdown_chat_script_cmds,
			      MODEM_CHAT_SCRIPT_CMD_RESP("AT+QPOWD=1", eg916q_ok_match));

MODEM_CHAT_SCRIPT_DEFINE(eg916q_shutdown_chat_script, eg916q_shutdown_chat_script_cmds,
			 eg916q_abort_matches, NULL, 5);

/* AT+CSQ response is "+CSQ: <rssi>,<ber>" -- rssi 0-31 maps to -113..-51 dBm,
 * 99 means unknown/undetectable; ber is unused here. Needs argv parsing (32
 * possible rssi values, each with several possible ber values, rules out
 * matching fixed strings the way eg916q_cgreg_registered_matches does), so
 * this uses a callback like an unsolicited match would, even though CSQ's
 * response is solicited -- same pattern zephyr/drivers/modem/modem_cellular.c
 * uses for the same command.
 */
static void eg916q_chat_on_csq(struct modem_chat *chat, char **argv, uint16_t argc,
			       void *user_data)
{
	ARG_UNUSED(chat);
	struct eg916q_data *data = (struct eg916q_data *)user_data;

	if (argc != 3) {
		return;
	}

	data->last_csq_rssi = (uint8_t)atoi(argv[1]);
}

MODEM_CHAT_MATCH_DEFINE(eg916q_csq_match, "+CSQ: ", ",", eg916q_chat_on_csq);

/* AT+CFUN=1 first: AT+CSQ reports RSSI 99 (unknown) unless the radio is
 * actually enabled, and unlike eg916q_init_chat_script's full sequence this
 * script skips straight to CSQ otherwise -- there's no guarantee the module
 * powers up with full functionality already on. Without this, every signal
 * check would report "no signal" regardless of actual conditions.
 */
MODEM_CHAT_SCRIPT_CMDS_DEFINE(eg916q_csq_chat_script_cmds,
			      MODEM_CHAT_SCRIPT_CMD_RESP("AT+CFUN=1", eg916q_ok_match),
			      MODEM_CHAT_SCRIPT_CMD_RESP("AT+CSQ", eg916q_csq_match),
			      MODEM_CHAT_SCRIPT_CMD_RESP("", eg916q_ok_match));

MODEM_CHAT_SCRIPT_DEFINE(eg916q_csq_chat_script, eg916q_csq_chat_script_cmds,
			 eg916q_abort_matches, NULL, 10);

/* AT/OK only -- see quectel_eg916q.h's eg916q_selftest() doc comment for why
 * this is a separate, narrower script than eg916q_init_chat_script.
 */
MODEM_CHAT_SCRIPT_CMDS_DEFINE(eg916q_selftest_chat_script_cmds,
			      MODEM_CHAT_SCRIPT_CMD_RESP("AT", eg916q_ok_match));

MODEM_CHAT_SCRIPT_DEFINE(eg916q_selftest_chat_script, eg916q_selftest_chat_script_cmds,
			 eg916q_abort_matches, NULL, 5);

/* --- Native TCP/IP: bring up the PDP context's IP layer inside the module,
 * no PPP/host net_if involved (quectel_eg800qeg91xq_series_tcpip_application_
 * note_v1-3.md section 1.1/2.3.2/2.3.3). AT+QICSGP configures the context
 * for the native stack specifically (separate from, but pointed at the same
 * context ID and APN as, AT+CGDCONT above); AT+QIACT activates it, which the
 * datasheet says can take up to 150s depending on the network.
 */
MODEM_CHAT_SCRIPT_CMDS_DEFINE(eg916q_qicsgp_chat_script_cmds,
			      MODEM_CHAT_SCRIPT_CMD_RESP(
				      "AT+QICSGP=" STRINGIFY(EG916Q_PDP_CONTEXT_ID)
				      ",1,\"" CONFIG_MODEM_QUECTEL_EG916Q_APN "\",\"\",\"\",0",
				      eg916q_ok_match));

MODEM_CHAT_SCRIPT_DEFINE(eg916q_qicsgp_chat_script, eg916q_qicsgp_chat_script_cmds,
			 eg916q_abort_matches, NULL, 10);

MODEM_CHAT_SCRIPT_CMDS_DEFINE(eg916q_qiact_chat_script_cmds,
			      MODEM_CHAT_SCRIPT_CMD_RESP("AT+QIACT=" STRINGIFY(EG916Q_PDP_CONTEXT_ID),
							 eg916q_ok_match));

MODEM_CHAT_SCRIPT_DEFINE(eg916q_qiact_chat_script, eg916q_qiact_chat_script_cmds,
			 eg916q_abort_matches, NULL, 150);

MODEM_CHAT_SCRIPT_CMDS_DEFINE(eg916q_qideact_chat_script_cmds,
			      MODEM_CHAT_SCRIPT_CMD_RESP(
				      "AT+QIDEACT=" STRINGIFY(EG916Q_PDP_CONTEXT_ID),
				      eg916q_ok_match));

MODEM_CHAT_SCRIPT_DEFINE(eg916q_qideact_chat_script, eg916q_qideact_chat_script_cmds,
			 eg916q_abort_matches, NULL, 40);

/* --- Bench connectivity check (AT+QPING + a raw TCP HTTP GET): a basic
 * "is the cellular link actually working at all" sanity test, deliberately
 * independent of the MQTT/TLS path above -- see eg916q_network_selftest()'s
 * doc comment.
 *
 * AT+QPING="1,\"www.google.com\",4,1" (contextID 1, 4s per-ping timeout,
 * pingnum 1) produces, on top of the immediate "OK": one per-ping result
 * line "+QPING: <ping_result>[,...]", then one final summary line
 * "+QPING: <finresult>[,<sent>,<rcvd>,<lost>,<min>,<max>,<avg>]". Both share
 * the same "+QPING: " prefix, so the same match/callback is used for both
 * script steps in sequence; the callback simply overwrites
 * last_qping_result each time it fires, so after the script completes it
 * holds the *last* line's leading field -- the final summary's <finresult>,
 * which is 0 on success. (The middle per-ping line's value is transiently
 * overwritten and never inspected, which is fine: with pingnum=1 there is
 * nothing else to distinguish it from.)
 */
static void eg916q_chat_on_qping(struct modem_chat *chat, char **argv, uint16_t argc,
				 void *user_data)
{
	ARG_UNUSED(chat);
	struct eg916q_data *data = (struct eg916q_data *)user_data;

	if (argc < 2) {
		return;
	}

	data->last_qping_result = atoi(argv[1]);
}

MODEM_CHAT_MATCH_DEFINE(eg916q_qping_match, "+QPING: ", ",", eg916q_chat_on_qping);

MODEM_CHAT_SCRIPT_CMDS_DEFINE(eg916q_qping_chat_script_cmds,
			      MODEM_CHAT_SCRIPT_CMD_RESP(
				      "AT+QPING=" STRINGIFY(EG916Q_PDP_CONTEXT_ID)
				      ",\"www.google.com\",4,1",
				      eg916q_ok_match),
			      MODEM_CHAT_SCRIPT_CMD_RESP("", eg916q_qping_match),
			      MODEM_CHAT_SCRIPT_CMD_RESP("", eg916q_qping_match));

/* 1 ping x 4s timeout should resolve well inside this, but DNS resolution
 * for "www.google.com" adds unknown slack. */
MODEM_CHAT_SCRIPT_DEFINE(eg916q_qping_chat_script, eg916q_qping_chat_script_cmds,
			 eg916q_abort_matches, NULL, 30);

/* AT+QIOPEN's command line is fixed (fixed host/port for this bench check,
 * unlike the MQTT path's caller-supplied broker), so unlike AT+QMTOPEN this
 * can be one ordinary static script: "OK" then the async "+QIOPEN: ..."
 * result, same two-stage shape documented for AT+QMTOPEN.
 */
static void eg916q_chat_on_qiopen(struct modem_chat *chat, char **argv, uint16_t argc,
				  void *user_data)
{
	ARG_UNUSED(chat);
	struct eg916q_data *data = (struct eg916q_data *)user_data;

	/* "+QIOPEN: <connectID>,<err>" */
	if (argc != 3) {
		return;
	}

	data->last_qiopen_result = atoi(argv[2]);
}

MODEM_CHAT_MATCH_DEFINE(eg916q_qiopen_match, "+QIOPEN: ", ",", eg916q_chat_on_qiopen);

/* contextID 1, connectID 0, TCP client, port 80, local port auto (0),
 * buffer access mode (0) -- matches AT+QMTOPEN's two-stage OK-then-URC shape.
 */
MODEM_CHAT_SCRIPT_CMDS_DEFINE(eg916q_qiopen_chat_script_cmds,
			      MODEM_CHAT_SCRIPT_CMD_RESP(
				      "AT+QIOPEN=" STRINGIFY(EG916Q_PDP_CONTEXT_ID)
				      ",0,\"TCP\",\"www.google.com\",80,0,0",
				      eg916q_ok_match),
			      MODEM_CHAT_SCRIPT_CMD_RESP("", eg916q_qiopen_match));

/* 150s: "It is suggested to wait for 150s for +QIOPEN ... to be outputted"
 * (TCP/IP app note, AT+QIOPEN section).
 */
MODEM_CHAT_SCRIPT_DEFINE(eg916q_qiopen_chat_script, eg916q_qiopen_chat_script_cmds,
			 eg916q_abort_matches, NULL, 150);

/* AT+QISEND's command line embeds a fixed request length (known at compile
 * time here, unlike the MQTT payload's runtime length), so this too can be
 * one static script: '>' prompt, then (after the raw HTTP request bytes are
 * written directly to the pipe, same technique as everywhere else in this
 * file) "SEND OK".
 *
 * The command line embeds sizeof(EG916Q_HTTP_GET_REQUEST), which -- despite
 * the request text itself being fixed -- is a value the *compiler*
 * computes, not the preprocessor; STRINGIFY() only stringifies macro
 * tokens, so it cannot turn a sizeof() expression into a string literal.
 * The command is therefore built at runtime via eg916q_run_cmd() (see
 * eg916q_network_selftest()), same as every other command in this file that
 * can't be a plain compile-time script.
 */
#define EG916Q_HTTP_GET_REQUEST \
	"GET / HTTP/1.1\r\nHost: www.google.com\r\nConnection: close\r\n\r\n"

MODEM_CHAT_MATCH_DEFINE(eg916q_send_ok_match, "SEND OK", "", NULL);

MODEM_CHAT_SCRIPT_CMDS_DEFINE(eg916q_qisend_finish_chat_script_cmds,
			      MODEM_CHAT_SCRIPT_CMD_RESP("", eg916q_send_ok_match));

MODEM_CHAT_SCRIPT_DEFINE(eg916q_qisend_finish_chat_script, eg916q_qisend_finish_chat_script_cmds,
			 eg916q_abort_matches, NULL, 10);

/* "+QIRD: <read_actual_length>\r\n<data>" then "OK" (or just "+QIRD: 0" then
 * "OK" if nothing has arrived yet -- read_actual_length is still the first
 * field either way, so the same match/callback covers both cases).
 */
static void eg916q_chat_on_qird(struct modem_chat *chat, char **argv, uint16_t argc,
				void *user_data)
{
	ARG_UNUSED(chat);
	struct eg916q_data *data = (struct eg916q_data *)user_data;

	if (argc < 2) {
		return;
	}

	data->last_qird_len = atoi(argv[1]);
}

MODEM_CHAT_MATCH_DEFINE(eg916q_qird_match, "+QIRD: ", ",", eg916q_chat_on_qird);

MODEM_CHAT_SCRIPT_CMDS_DEFINE(eg916q_qird_chat_script_cmds,
			      MODEM_CHAT_SCRIPT_CMD_RESP("AT+QIRD=0,1024", eg916q_qird_match),
			      MODEM_CHAT_SCRIPT_CMD_RESP("", eg916q_ok_match));

MODEM_CHAT_SCRIPT_DEFINE(eg916q_qird_chat_script, eg916q_qird_chat_script_cmds,
			 eg916q_abort_matches, NULL, 10);

MODEM_CHAT_SCRIPT_CMDS_DEFINE(eg916q_qiclose_chat_script_cmds,
			      MODEM_CHAT_SCRIPT_CMD_RESP("AT+QICLOSE=0", eg916q_ok_match));

MODEM_CHAT_SCRIPT_DEFINE(eg916q_qiclose_chat_script, eg916q_qiclose_chat_script_cmds,
			 eg916q_abort_matches, NULL, 15);

/* --- TLS for the native MQTT client. AT+QFUPL uploads the broker's CA cert
 * to the module's own UFS filesystem (raw bytes after a CONNECT prompt, like
 * AT+QIOPEN's transparent mode); AT+QSSLCFG then points SSL context
 * EG916Q_SSL_CTX_IDX at that file and sets server-authentication-only mode
 * (seclevel 1 -- no client cert/key, matching a broker that authenticates by
 * MQTT username/password rather than mTLS). Uploaded fresh every publish
 * rather than checked-and-skipped-if-present -- see eg916q_mqtt_publish()'s
 * doc comment.
 *
 * AT+QFUPL's command line embeds the cert's byte length, which is a runtime
 * value (not a compile-time constant the MODEM_CHAT_SCRIPT_*_DEFINE macros
 * below could hold) -- see eg916q_run_cmd()'s doc comment for how that
 * (and every other command line built from a caller-supplied string in this
 * file) is sent instead of a static script.
 *
 * "+QFUPL: <size>,<checksum>" follows the raw upload; the checksum isn't
 * verified here -- if the upload was corrupted, AT+QSSLCFG referencing this
 * file will fail the TLS handshake downstream, caught the same way a bad
 * cert from any other cause would be.
 */
MODEM_CHAT_MATCH_DEFINE(eg916q_qfupl_result_match, "+QFUPL: ", "", NULL);

MODEM_CHAT_SCRIPT_CMDS_DEFINE(eg916q_qfupl_finish_chat_script_cmds,
			      MODEM_CHAT_SCRIPT_CMD_RESP("", eg916q_qfupl_result_match),
			      MODEM_CHAT_SCRIPT_CMD_RESP("", eg916q_ok_match));

/* 30s, not the usual 10s: this follows a UFS filesystem write + checksum on
 * the module's side, not just an AT round trip.
 */
MODEM_CHAT_SCRIPT_DEFINE(eg916q_qfupl_finish_chat_script, eg916q_qfupl_finish_chat_script_cmds,
			 eg916q_abort_matches, NULL, 30);

MODEM_CHAT_SCRIPT_CMDS_DEFINE(eg916q_qsslcfg_chat_script_cmds,
			      MODEM_CHAT_SCRIPT_CMD_RESP(
				      "AT+QSSLCFG=\"cacert\"," STRINGIFY(EG916Q_SSL_CTX_IDX)
				      ",\"UFS:cacert.pem\"",
				      eg916q_ok_match),
			      MODEM_CHAT_SCRIPT_CMD_RESP(
				      "AT+QSSLCFG=\"seclevel\"," STRINGIFY(EG916Q_SSL_CTX_IDX) ",1",
				      eg916q_ok_match),
			      MODEM_CHAT_SCRIPT_CMD_RESP(
				      "AT+QSSLCFG=\"sslversion\"," STRINGIFY(EG916Q_SSL_CTX_IDX) ",4",
				      eg916q_ok_match),
			      MODEM_CHAT_SCRIPT_CMD_RESP(
				      "AT+QSSLCFG=\"ciphersuite\"," STRINGIFY(EG916Q_SSL_CTX_IDX)
				      ",0xFFFF",
				      eg916q_ok_match),
			      MODEM_CHAT_SCRIPT_CMD_RESP(
				      "AT+QSSLCFG=\"ignorelocaltime\"," STRINGIFY(EG916Q_SSL_CTX_IDX)
				      ",1",
				      eg916q_ok_match));

MODEM_CHAT_SCRIPT_DEFINE(eg916q_qsslcfg_chat_script, eg916q_qsslcfg_chat_script_cmds,
			 eg916q_abort_matches, NULL, 10);

/* "version",4 = MQTT 3.1.1. The app note lists 3 (3.1) and 4 (3.1.1) without
 * stating the default, so pin it rather than depend on module firmware.
 * "session",1 = clean session: pinned for the same reason (the app note
 * doesn't state clean_session's default either) -- always start fresh
 * rather than risk the broker trying to resume a stale/dirty session left
 * behind by an earlier failed attempt.
 */
MODEM_CHAT_SCRIPT_CMDS_DEFINE(eg916q_qmtcfg_ssl_chat_script_cmds,
			      MODEM_CHAT_SCRIPT_CMD_RESP(
				      "AT+QMTCFG=\"version\"," STRINGIFY(EG916Q_MQTT_CLIENT_IDX) ",4",
				      eg916q_ok_match),
			      MODEM_CHAT_SCRIPT_CMD_RESP(
				      "AT+QMTCFG=\"session\"," STRINGIFY(EG916Q_MQTT_CLIENT_IDX) ",1",
				      eg916q_ok_match),
			      MODEM_CHAT_SCRIPT_CMD_RESP(
				      "AT+QMTCFG=\"ssl\"," STRINGIFY(EG916Q_MQTT_CLIENT_IDX) ",1,"
				      STRINGIFY(EG916Q_SSL_CTX_IDX),
				      eg916q_ok_match));

MODEM_CHAT_SCRIPT_DEFINE(eg916q_qmtcfg_ssl_chat_script, eg916q_qmtcfg_ssl_chat_script_cmds,
			 eg916q_abort_matches, NULL, 10);

/* --- Native MQTT client. Each of QMTOPEN/QMTCONN/QMTPUBEX/QMTDISC/QMTCLOSE
 * follows the same two-stage shape documented in the MQTT app note's data-
 * interaction diagram: an immediate "OK" acknowledging the command syntax,
 * followed by a separate, later, asynchronous "+QMTxxxx: ..." URC-shaped
 * line carrying the actual result once the network round trip completes.
 * Each is therefore run as two separate modem_chat_run_script() calls (see
 * eg916q_mqtt_publish()): first the command itself via eg916q_run_cmd()
 * (its text is built at runtime from caller-supplied host/port/credentials/
 * topic, so it cannot be one of this file's compile-time script constants),
 * waiting only for the immediate "OK"; then one of the static, empty-request
 * "wait" scripts below, which send nothing further and wait for the
 * asynchronous result line, with the script's timeout sized for the
 * datasheet's documented worst case for that stage.
 */
static void eg916q_chat_on_qmtopen(struct modem_chat *chat, char **argv, uint16_t argc,
				   void *user_data)
{
	ARG_UNUSED(chat);
	struct eg916q_data *data = (struct eg916q_data *)user_data;

	/* "+QMTOPEN: <client_idx>,<result>" -- argv[0] is the matched prefix,
	 * argv[1] client_idx, argv[2] result.
	 */
	if (argc != 3) {
		return;
	}

	data->last_qmtopen_result = atoi(argv[2]);
}

MODEM_CHAT_MATCH_DEFINE(eg916q_qmtopen_match, "+QMTOPEN: ", ",", eg916q_chat_on_qmtopen);

MODEM_CHAT_SCRIPT_CMDS_DEFINE(eg916q_qmtopen_wait_chat_script_cmds,
			      MODEM_CHAT_SCRIPT_CMD_RESP("", eg916q_qmtopen_match));

/* 150s: "It is suggested to wait for 150s for +QIOPEN/+QMTOPEN to be
 * outputted" (TCP/IP app note, AT+QIOPEN section) -- QMTOPEN opens the same
 * underlying TCP connection.
 */
MODEM_CHAT_SCRIPT_DEFINE(eg916q_qmtopen_wait_chat_script, eg916q_qmtopen_wait_chat_script_cmds,
			 eg916q_abort_matches, NULL, 150);

static void eg916q_chat_on_qmtconn(struct modem_chat *chat, char **argv, uint16_t argc,
				   void *user_data)
{
	ARG_UNUSED(chat);
	struct eg916q_data *data = (struct eg916q_data *)user_data;

	/* "+QMTCONN: <client_idx>,<result>[,<ret_code>]". result==0 only means
	 * a CONNACK packet came back; ret_code is the broker's verdict in it
	 * (0 accepted, 1 bad protocol version, 2 client ID rejected, 3 server
	 * unavailable, 4 bad username/password, 5 not authorized). Reading
	 * result alone let a refused CONNACK through as success, and the
	 * failure only surfaced later as +QMTSTAT: 0,4 and a QMTPUBEX timeout.
	 */
	if (argc < 3) {
		return;
	}

	data->last_qmtconn_result = atoi(argv[2]);
	data->last_qmtconn_ret_code = (argc >= 4) ? atoi(argv[3]) : -1;
}

MODEM_CHAT_MATCH_DEFINE(eg916q_qmtconn_match, "+QMTCONN: ", ",", eg916q_chat_on_qmtconn);

MODEM_CHAT_SCRIPT_CMDS_DEFINE(eg916q_qmtconn_wait_chat_script_cmds,
			      MODEM_CHAT_SCRIPT_CMD_RESP("", eg916q_qmtconn_match));

/* "<pkt_timeout> x <retry_times> (default 15s), determined by network"
 * (MQTT app note, AT+QMTCONN section) -- generous margin over the default.
 */
MODEM_CHAT_SCRIPT_DEFINE(eg916q_qmtconn_wait_chat_script, eg916q_qmtconn_wait_chat_script_cmds,
			 eg916q_abort_matches, NULL, 60);

static void eg916q_chat_on_qmtpubex(struct modem_chat *chat, char **argv, uint16_t argc,
				    void *user_data)
{
	ARG_UNUSED(chat);
	struct eg916q_data *data = (struct eg916q_data *)user_data;

	/* "+QMTPUBEX: <client_idx>,<msgid>,<result>[,<value>]" -- result is
	 * the third field (argv[3]), not the second.
	 */
	if (argc < 4) {
		return;
	}

	data->last_qmtpubex_result = atoi(argv[3]);
}

MODEM_CHAT_MATCH_DEFINE(eg916q_qmtpubex_match, "+QMTPUBEX: ", ",", eg916q_chat_on_qmtpubex);

/* After the '>' prompt from the AT+QMTPUBEX command (sent dynamically via
 * eg916q_run_cmd() in eg916q_mqtt_publish(), since it embeds a caller-
 * supplied topic/length), the raw payload bytes are written directly to the
 * pipe (same technique as the cert upload), then this script waits for the
 * immediate "OK" and finally the asynchronous "+QMTPUBEX: ..." result --
 * QoS 1 means that result only arrives once the broker's PUBACK comes back
 * over the network.
 */
MODEM_CHAT_SCRIPT_CMDS_DEFINE(eg916q_qmtpubex_finish_chat_script_cmds,
			      MODEM_CHAT_SCRIPT_CMD_RESP("", eg916q_ok_match),
			      MODEM_CHAT_SCRIPT_CMD_RESP("", eg916q_qmtpubex_match));

MODEM_CHAT_SCRIPT_DEFINE(eg916q_qmtpubex_finish_chat_script, eg916q_qmtpubex_finish_chat_script_cmds,
			 eg916q_abort_matches, NULL, 60);

/* Best-effort teardown -- see eg916q_mqtt_publish()'s use of these: failures
 * are logged, not fatal, since the module is powered off (a real, deterministic
 * reset of all this state) immediately afterward regardless.
 */
MODEM_CHAT_MATCHES_DEFINE(eg916q_qmtdisc_matches, MODEM_CHAT_MATCH_WILDCARD("+QMTDISC: *", "", NULL));

MODEM_CHAT_SCRIPT_CMDS_DEFINE(eg916q_qmtdisc_chat_script_cmds,
			      MODEM_CHAT_SCRIPT_CMD_RESP(
				      "AT+QMTDISC=" STRINGIFY(EG916Q_MQTT_CLIENT_IDX), eg916q_ok_match),
			      MODEM_CHAT_SCRIPT_CMD_RESP_MULT("", eg916q_qmtdisc_matches));

MODEM_CHAT_SCRIPT_DEFINE(eg916q_qmtdisc_chat_script, eg916q_qmtdisc_chat_script_cmds,
			 eg916q_abort_matches, NULL, 15);

MODEM_CHAT_MATCHES_DEFINE(eg916q_qmtclose_matches,
			  MODEM_CHAT_MATCH_WILDCARD("+QMTCLOSE: *", "", NULL));

MODEM_CHAT_SCRIPT_CMDS_DEFINE(eg916q_qmtclose_chat_script_cmds,
			      MODEM_CHAT_SCRIPT_CMD_RESP(
				      "AT+QMTCLOSE=" STRINGIFY(EG916Q_MQTT_CLIENT_IDX), eg916q_ok_match),
			      MODEM_CHAT_SCRIPT_CMD_RESP_MULT("", eg916q_qmtclose_matches));

MODEM_CHAT_SCRIPT_DEFINE(eg916q_qmtclose_chat_script, eg916q_qmtclose_chat_script_cmds,
			 eg916q_abort_matches, NULL, 15);

/* ---------------------------------------------------------------------
 * Power sequencing
 * ------------------------------------------------------------------ */

static int eg916q_power_on(const struct eg916q_config *config)
{
	int err = gpio_pin_set_dt(&config->power_gpio, 1);

	if (err != 0) {
		return err;
	}

	k_sleep(K_MSEC(CONFIG_MODEM_QUECTEL_EG916Q_POWER_PULSE_MS));

	err = gpio_pin_set_dt(&config->power_gpio, 0);
	if (err != 0) {
		return err;
	}

	LOG_INF("power pulse sent, waiting %d ms for boot",
		CONFIG_MODEM_QUECTEL_EG916Q_STARTUP_TIME_MS);
	k_sleep(K_MSEC(CONFIG_MODEM_QUECTEL_EG916Q_STARTUP_TIME_MS));

	return 0;
}

/* 3GPP TS 27.007 <stat> for +CREG/+CGREG/+CEREG. -1 = no parseable reply. */
static const char *eg916q_reg_stat_str(int stat)
{
	switch (stat) {
	case 0:
		return "not registered, not searching";
	case 1:
		return "registered, home";
	case 2:
		return "not registered, searching";
	case 3:
		return "registration denied";
	case 4:
		return "unknown";
	case 5:
		return "registered, roaming";
	default:
		return "no reply";
	}
}

static bool eg916q_reg_stat_registered(int stat)
{
	return stat == 1 || stat == 5;
}

/* Polls +CEREG/+CGREG until either reports registered, or
 * CONFIG_MODEM_QUECTEL_EG916Q_REGISTER_TIMEOUT_S elapses. Logs the actual
 * <stat> whenever it changes, so a failure says *why* (see
 * eg916q_chat_on_creg()'s comment). Deadline-based rather than a fixed
 * attempt count: each poll's duration varies (fast reply vs. script
 * timeout), so "attempts = timeout / 2" never matched the configured time.
 */
static int eg916q_wait_registered(struct eg916q_data *data)
{
	const int64_t deadline =
		k_uptime_get() + (int64_t)CONFIG_MODEM_QUECTEL_EG916Q_REGISTER_TIMEOUT_S * 1000;
	int prev_cereg = -2;
	int prev_cgreg = -2;
	int cereg = -1;
	int cgreg = -1;

	while (k_uptime_get() < deadline) {
		data->last_reg_stat = -1;
		(void)modem_chat_run_script(&data->chat, &eg916q_cereg_poll_chat_script);
		cereg = data->last_reg_stat;

		data->last_reg_stat = -1;
		(void)modem_chat_run_script(&data->chat, &eg916q_cgreg_poll_chat_script);
		cgreg = data->last_reg_stat;

		if (cereg != prev_cereg || cgreg != prev_cgreg) {
			LOG_INF("registration: CEREG %d (%s), CGREG %d (%s)", cereg,
				eg916q_reg_stat_str(cereg), cgreg, eg916q_reg_stat_str(cgreg));
			prev_cereg = cereg;
			prev_cgreg = cgreg;
		}

		if (eg916q_reg_stat_registered(cereg) || eg916q_reg_stat_registered(cgreg)) {
			return 0;
		}

		k_sleep(K_SECONDS(2));
	}

	LOG_ERR("registration: not registered after %d s (last CEREG %d (%s), CGREG %d (%s))",
		CONFIG_MODEM_QUECTEL_EG916Q_REGISTER_TIMEOUT_S, cereg, eg916q_reg_stat_str(cereg),
		cgreg, eg916q_reg_stat_str(cgreg));
	return -ETIMEDOUT;
}

/* Waits up to ~20s for "+CPIN: READY", per the TCP/IP app note's flow
 * chart. Without this, a missing or PIN-locked SIM only ever showed up as a
 * registration timeout a minute later, indistinguishable from bad coverage.
 *
 * A script return of 0 only means the module replied with *some* "+CPIN: "
 * line, not necessarily READY (see eg916q_cpin_match's doc comment) -- the
 * actual state is checked here, against data->last_cpin_state.
 */
static int eg916q_wait_sim_ready(struct eg916q_data *data, const char *log_prefix)
{
	for (int attempt = 1; attempt <= 4; attempt++) {
		data->last_cpin_state[0] = '\0';

		const int err = modem_chat_run_script(&data->chat, &eg916q_cpin_chat_script);

		if (err == 0 && strcmp(data->last_cpin_state, "READY") == 0) {
			LOG_INF("%s: SIM ready", log_prefix);
			return 0;
		}

		if (err == 0) {
			LOG_WRN("%s: SIM state \"%s\" (not READY); retrying in 5 s", log_prefix,
				data->last_cpin_state);
		} else {
			LOG_WRN("%s: AT+CPIN? failed: %d; retrying in 5 s", log_prefix, err);
		}

		if (attempt < 4) {
			k_sleep(K_SECONDS(5));
		}
	}

	if (data->last_cpin_state[0] != '\0') {
		LOG_ERR("%s: SIM not ready after ~20 s -- last state \"%s\" (PIN/PUK locked?)",
			log_prefix, data->last_cpin_state);
		return -EACCES;
	}

	LOG_ERR("%s: SIM not ready after ~20 s -- no SIM inserted, or module not responding",
		log_prefix);
	return -ENXIO;
}

/* Powers the module on, opens its UART pipe and attaches chat -- unless
 * eg916q_selftest() already did exactly that and left it running (see
 * struct eg916q_data's powered_on comment), in which case this is a no-op
 * and the existing session is reused as-is.
 *
 * Added because every entry point in this file used to power-cycle
 * independently: the self-test would power on, run "AT", then power off
 * (a 10s pulse + 10s boot wait, then AT+QPOWD + a 12s shutdown wait), and
 * the very next cellular operation (network self-test / MQTT publish)
 * would immediately power back on from scratch, repeating that ~30-40s and
 * restarting network attach (registration etc.) from nothing. Since the
 * self-test already proved the module answers AT, there is no reason to
 * tear that session down only to rebuild it seconds later.
 */
static int eg916q_ensure_powered_on(const struct device *dev)
{
	const struct eg916q_config *config = dev->config;
	struct eg916q_data *data = dev->data;
	int err;

	if (data->powered_on) {
		return 0;
	}

	err = modem_pipe_open(data->uart_pipe, K_SECONDS(10));
	if (err != 0) {
		return err;
	}

	/* PWRKEY is a toggle, and the module may already be running: the MCU
	 * can enter System OFF (wake = full reboot) while a bench entry point
	 * has left the module on, and nothing here can read its power state.
	 * Pulsing PWRKEY then switches the running module *off*, the AT check
	 * fails and the self-test never releases the cellular task. So probe
	 * with a bare "AT" first and only pulse if it stays silent.
	 */
	modem_chat_attach(&data->chat, data->uart_pipe);

	if (modem_chat_run_script(&data->chat, &eg916q_selftest_chat_script) == 0) {
		LOG_INF("module already powered on; skipping power pulse");
		data->powered_on = true;
		return 0;
	}

	modem_chat_release(&data->chat);

	err = eg916q_power_on(config);
	if (err != 0) {
		modem_pipe_close(data->uart_pipe, K_SECONDS(5));
		return err;
	}

	modem_chat_attach(&data->chat, data->uart_pipe);
	data->powered_on = true;
	return 0;
}

/* Powers the module all the way off via the deterministic AT+QPOWD shutdown
 * (see eg916q_shutdown_chat_script's comment for why a plain GPIO deassert
 * alone is not a real power-off), then releases the chat/pipe/GPIO. Shared
 * end-of-function cleanup for every entry point in this file.
 */
static void eg916q_power_off_and_release(const struct eg916q_config *config,
					 struct eg916q_data *data, const char *log_prefix)
{
	data->powered_on = false;

	if (modem_chat_run_script(&data->chat, &eg916q_shutdown_chat_script) != 0) {
		LOG_WRN("%s: AT+QPOWD failed or timed out; power state of modem is now "
			"unknown",
			log_prefix);
	} else {
		/* "OK" to AT+QPOWD only means shutdown has *started*. The TCP/IP
		 * app note's power flow (Figure 1): "Send AT+QPOWD command, and
		 * then cut off power after 12s or more." Returning immediately
		 * let the very next entry point assert PWRKEY ~1s later, landing
		 * mid-shutdown -- observed on hardware as the next power-on's
		 * init script getting no reply at all. Block here so no caller
		 * can start a new power-on until shutdown has actually finished.
		 */
		k_sleep(K_SECONDS(12));
	}

	modem_chat_release(&data->chat);
	modem_pipe_close(data->uart_pipe, K_SECONDS(5));
	gpio_pin_set_dt(&config->power_gpio, 0);
}

/* eg916q_power_on()'s STARTUP_TIME_MS wait is only a best-effort estimate
 * of when the module finishes booting. Retry a bare "AT" until it answers,
 * rather than trusting a single fixed delay. Must run before any multi-
 * command script: modem_chat sends each request once and never resends, so
 * a script whose first "AT" lands while the module is still booting just
 * sits until its overall timeout -- observed on hardware as
 * "eg916q_init_chat_script: timed out" even though the module answered
 * AT+QPOWD fine a moment later.
 */
static int eg916q_wait_ready(struct eg916q_data *data, const char *log_prefix)
{
	int err = -ETIMEDOUT;

	for (int attempt = 1; attempt <= CONFIG_MODEM_QUECTEL_EG916Q_SELFTEST_RETRIES; attempt++) {
		err = modem_chat_run_script(&data->chat, &eg916q_selftest_chat_script);
		if (err == 0) {
			return 0;
		}
		LOG_WRN("%s: AT attempt %d/%d failed: %d", log_prefix, attempt,
			CONFIG_MODEM_QUECTEL_EG916Q_SELFTEST_RETRIES, err);
		if (attempt < CONFIG_MODEM_QUECTEL_EG916Q_SELFTEST_RETRIES) {
			k_sleep(K_SECONDS(5));
		}
	}

	LOG_ERR("%s: modem not responding after %d attempts: %d", log_prefix,
		CONFIG_MODEM_QUECTEL_EG916Q_SELFTEST_RETRIES, err);
	return err;
}

int eg916q_selftest(const struct device *dev)
{
	struct eg916q_data *data = dev->data;
	int err;

	err = eg916q_ensure_powered_on(dev);
	if (err != 0) {
		LOG_ERR("self-test: power-on failed: %d", err);
		return err;
	}

	err = eg916q_wait_ready(data, "self-test");
	if (err == 0) {
		LOG_INF("self-test: modem replied OK");
		/* Deliberately leave the module powered on and chat attached
		 * -- see eg916q_ensure_powered_on()'s doc comment. The next
		 * real cellular operation reuses this session instead of
		 * power-cycling again.
		 */
	} else {
		/* Nothing useful to keep alive if the module never even
		 * answered a bare "AT" -- power off rather than leave it in
		 * an unknown state for the next caller to inherit.
		 */
		eg916q_power_off_and_release(dev->config, data, "self-test");
	}

	return err;
}

int eg916q_power_down(const struct device *dev)
{
	struct eg916q_data *data = dev->data;

	if (!data->powered_on) {
		return 0;
	}

	eg916q_power_off_and_release(dev->config, data, "power down");
	return 0;
}

#define EG916Q_CSQ_RSSI_UNKNOWN 99

/* Polls AT+CSQ on an *already powered-on and chat-attached* session,
 * sleeping and retrying in place rather than power-cycling between
 * attempts, until the signal is at or above min_rssi_dbm.
 *
 * This replaces an earlier design where each check was its own standalone
 * power-on/AT+CSQ/power-off cycle, called repeatedly by the app layer. On
 * real hardware that proved unreliable: AT+CSQ routinely timed out
 * ("eg916q_csq_chat_script: timed out") on every retry, because a fresh
 * ~20s PWRKEY power-on cycle every 30s never gave the module enough settled
 * time before the next one interrupted it. Staying powered and re-polling
 * in place is both more reliable and faster once signal does arrive, at
 * the cost of keeping the modem (and its current draw) alive for however
 * long this takes -- a deliberate reliability-over-battery tradeoff for
 * now; see cellular_task.cpp's execute() for the app-level context.
 * (eg916q_get_signal_quality() below is a separate, one-shot debug helper
 * with no retry loop -- it never power-cycles either, so it doesn't hit the
 * same problem; it just leaves polling cadence to its caller.)
 *
 * deadline_ms (absolute k_uptime_get() time, 0 = none) bounds the retry loop:
 * once it passes, this returns -ETIMEDOUT instead of waiting for coverage
 * that may never come -- the caller is then expected to power the module
 * off. With 0 (the bench entry points) it only returns once signal is
 * usable.
 */
static bool eg916q_deadline_passed(int64_t deadline_ms)
{
	return deadline_ms != 0 && k_uptime_get() >= deadline_ms;
}

static int eg916q_wait_for_signal(struct eg916q_data *data, int min_rssi_dbm,
				  uint32_t retry_interval_s, int64_t deadline_ms)
{
	for (;;) {
		data->last_csq_rssi = EG916Q_CSQ_RSSI_UNKNOWN;

		const int err = modem_chat_run_script(&data->chat, &eg916q_csq_chat_script);

		if (err != 0) {
			LOG_WRN("signal check: AT+CSQ failed: %d; retrying in %u s", err,
				retry_interval_s);
		} else if (data->last_csq_rssi == EG916Q_CSQ_RSSI_UNKNOWN) {
			LOG_WRN("signal check: RSSI unknown; retrying in %u s", retry_interval_s);
		} else {
			const int rssi_dbm = -113 + (2 * (int)data->last_csq_rssi);

			if (rssi_dbm >= min_rssi_dbm) {
				LOG_INF("signal check: RSSI %u -> %d dBm: OK",
					data->last_csq_rssi, rssi_dbm);
				return 0;
			}
			LOG_WRN("signal check: %d dBm is below the %d dBm minimum; retrying in "
				"%u s",
				rssi_dbm, min_rssi_dbm, retry_interval_s);
		}

		if (deadline_ms != 0) {
			const int64_t left_ms = deadline_ms - k_uptime_get();

			if (left_ms <= 0) {
				LOG_WRN("signal check: gave up, session time limit reached");
				return -ETIMEDOUT;
			}

			k_sleep(K_MSEC(MIN((int64_t)retry_interval_s * 1000, left_ms)));
		} else {
			k_sleep(K_SECONDS(retry_interval_s));
		}
	}
}

/*
 * One-shot AT+CSQ signal check for bench debugging (see cellular_task.cpp,
 * currently gutted to just call this in a loop). Powers on (or reuses an
 * already-running session, same as every other entry point -- see
 * eg916q_ensure_powered_on()'s doc comment) and issues a single AT+CSQ, with
 * no retry loop and no minimum-signal gate: unlike eg916q_wait_for_signal(),
 * this always returns after one attempt so the caller controls the polling
 * cadence itself. Deliberately never powers off, matching every other entry
 * point here -- see eg916q_ensure_powered_on()'s doc comment on why a fresh
 * PWRKEY cycle before every check proved unreliable on real hardware.
 *
 * Returns 0 and writes the RSSI in dBm to *rssi_dbm on success, a negative
 * errno on failure (e.g. modem not powered/registered chat session), or
 * -EAGAIN if the module replied but RSSI is still unknown (99 -- normal
 * immediately after boot, before the radio has scanned).
 */
int eg916q_get_signal_quality(const struct device *dev, int *rssi_dbm)
{
	struct eg916q_data *data = dev->data;
	int err;

	err = eg916q_ensure_powered_on(dev);
	if (err != 0) {
		LOG_ERR("signal check: power-on failed: %d", err);
		return err;
	}

	err = eg916q_wait_ready(data, "signal check");
	if (err != 0) {
		return err;
	}

	data->last_csq_rssi = EG916Q_CSQ_RSSI_UNKNOWN;

	err = modem_chat_run_script(&data->chat, &eg916q_csq_chat_script);
	if (err != 0) {
		LOG_WRN("signal check: AT+CSQ failed: %d", err);
		return err;
	}

	if (data->last_csq_rssi == EG916Q_CSQ_RSSI_UNKNOWN) {
		return -EAGAIN;
	}

	*rssi_dbm = -113 + (2 * (int)data->last_csq_rssi);
	return 0;
}

/*
 * One-shot bench ping to www.google.com for debugging (see
 * cellular_task.cpp). Brings the module up to full network readiness --
 * registration, then AT+QICSGP/AT+QIACT to activate the PDP context's IP
 * layer -- then issues a single AT+QPING, exactly the same setup+ping
 * sequence as the "ping" half of eg916q_network_selftest() (see that
 * function's doc comment), just without the HTTP GET half. Deliberately
 * leaves the module powered on and the PDP context active on success --
 * matching every other bench-debug entry point here (see
 * eg916q_ensure_powered_on()'s doc comment) -- so a caller pinging on a
 * cadence reuses this same session next call instead of tearing it down and
 * bringing it back up every time; re-running AT+QICSGP/AT+QIACT against an
 * already-active context is expected to be a fast no-op/OK on this module.
 * On any failure the context is deactivated and the module powered off, so
 * the next call starts from a clean slate.
 *
 * Returns 0 if the ping succeeded, -ENOTCONN if the module responded but
 * the ping itself failed, another negative errno for an earlier setup
 * failure (power-on, registration, QICSGP/QIACT, ...).
 */
int eg916q_ping(const struct device *dev, int min_rssi_dbm, uint32_t retry_interval_s)
{
	const struct eg916q_config *config = dev->config;
	struct eg916q_data *data = dev->data;
	int err;

	data->last_qping_result = -1;

	err = eg916q_ensure_powered_on(dev);
	if (err != 0) {
		LOG_ERR("ping: power-on failed: %d", err);
		return err;
	}

	err = eg916q_wait_ready(data, "ping");
	if (err != 0) {
		goto teardown;
	}

	err = modem_chat_run_script(&data->chat, &eg916q_init_chat_script);
	if (err != 0) {
		LOG_ERR("ping: init chat script failed: %d", err);
		goto teardown;
	}

	err = eg916q_wait_sim_ready(data, "ping");
	if (err != 0) {
		goto teardown;
	}

	err = eg916q_wait_for_signal(data, min_rssi_dbm, retry_interval_s, 0);
	if (err != 0) {
		LOG_ERR("ping: signal check failed: %d", err);
		goto teardown;
	}

	err = eg916q_wait_registered(data);
	if (err != 0) {
		LOG_ERR("ping: network registration timed out: %d", err);
		goto teardown;
	}

	err = modem_chat_run_script(&data->chat, &eg916q_qicsgp_chat_script);
	if (err != 0) {
		LOG_ERR("ping: AT+QICSGP failed: %d", err);
		goto teardown;
	}

	err = modem_chat_run_script(&data->chat, &eg916q_qiact_chat_script);
	if (err != 0) {
		LOG_ERR("ping: AT+QIACT failed: %d", err);
		goto teardown;
	}

	err = modem_chat_run_script(&data->chat, &eg916q_qping_chat_script);
	if (err != 0) {
		LOG_ERR("ping: AT+QPING not acknowledged: %d", err);
	} else if (data->last_qping_result != 0) {
		LOG_ERR("ping: ping to www.google.com failed: finresult=%d",
			data->last_qping_result);
		err = -ENOTCONN;
	} else {
		LOG_INF("ping: ping to www.google.com OK");
		return 0;
	}

	if (modem_chat_run_script(&data->chat, &eg916q_qideact_chat_script) != 0) {
		LOG_WRN("ping: AT+QIDEACT failed or timed out");
	}

teardown:
	eg916q_power_off_and_release(config, data, "ping");
	return err;
}

/* Sends the raw bytes of a chat command line without going through
 * modem_chat's normal request path -- used where the command string has to
 * be built at runtime (host/port/topic/length are all caller-supplied), and
 * for the raw file/payload bodies that follow a CONNECT/">" prompt. Same
 * technique gps_task.cpp uses for raw UBX bytes on the GNSS UART: writing to
 * the UART directly does not disturb modem_chat's RX-side parsing, which
 * keeps running throughout.
 */
static void eg916q_raw_write(const struct device *uart, const char *data, size_t len)
{
	for (size_t i = 0; i < len; i++) {
		uart_poll_out(uart, (uint8_t)data[i]);
	}
}

/* Runs a single command line built at runtime (host/port/credentials/topic/
 * length are all caller-supplied, so none of this file's compile-time
 * MODEM_CHAT_SCRIPT_*_DEFINE scripts can hold them -- those macros require a
 * request string literal, not a buffer filled at call time), waiting for a
 * single response match. Builds the modem_chat_script/script_chat structs
 * that MODEM_CHAT_SCRIPT_CMD_RESP/MODEM_CHAT_SCRIPT_DEFINE would otherwise
 * generate at compile time, but on the stack instead -- safe because
 * modem_chat_run_script() is synchronous and returns before this function
 * does, so nothing outlives the stack frame that owns it.
 */
static int eg916q_run_cmd(struct modem_chat *chat, const char *cmd_line,
			  const struct modem_chat_match *response_match, uint32_t timeout_s)
{
	const struct modem_chat_script_chat chat_step = {
		.request = (const uint8_t *)cmd_line,
		.request_size = (uint16_t)strlen(cmd_line),
		.response_matches = response_match,
		.response_matches_size = 1,
		.timeout = 0,
	};
	const struct modem_chat_script script = {
		.name = "eg916q_dynamic",
		.script_chats = &chat_step,
		.script_chats_size = 1,
		.abort_matches = eg916q_abort_matches,
		.abort_matches_size = ARRAY_SIZE(eg916q_abort_matches),
		.callback = NULL,
		.timeout = timeout_s,
	};

	return modem_chat_run_script(chat, &script);
}

/* Writes a raw payload after a bare, undelimited prompt ('>' / CONNECT --
 * see eg916q_http_get()'s doc comment for why modem_chat can't wait for
 * those) and returns the result of finish_script, which waits for the
 * module's CRLF-terminated reply lines.
 *
 * Order matters. The parser reset (release+attach) clears the never-
 * delimited prompt fragment out of modem_chat's receive buffer. The finish
 * script is then armed *before* the payload goes out: while no script is
 * running, modem_chat clears its response matches and silently drops any
 * line that arrives (modem_chat_script_stop() in subsys/modem/modem_chat.c),
 * and some replies come back almost instantly -- AT+QMTPUBEX's "OK" did, on
 * real hardware, and was lost every time when the script was only started
 * after the payload write.
 */
static int eg916q_write_payload_and_finish(struct eg916q_data *data, const struct device *uart,
					   const char *payload, size_t payload_len,
					   const struct modem_chat_script *finish_script)
{
	int err;

	modem_chat_release(&data->chat);
	modem_chat_attach(&data->chat, data->uart_pipe);

	err = modem_chat_run_script_async(&data->chat, finish_script);
	if (err != 0) {
		return err;
	}

	/* Let the chat workqueue actually start the script before the reply
	 * can arrive.
	 */
	k_sleep(K_MSEC(10));

	eg916q_raw_write(uart, payload, payload_len);

	err = k_sem_take(&data->chat.script_stopped_sem, K_FOREVER);
	if (err != 0) {
		return err;
	}

	return (data->chat.script_result == MODEM_CHAT_SCRIPT_RESULT_SUCCESS) ? 0 : -EAGAIN;
}

/* Uploads ca_cert_pem to the module's UFS filesystem as the given filename,
 * via AT+QFUPL's CONNECT-prompt raw upload protocol.
 *
 * AT+QFDEL first: on real hardware, AT+QFUPL against a filename that already
 * exists in UFS never reaches a usable upload state at all -- observed as
 * the finish script below timing out waiting for "+QFUPL: ...", regardless
 * of how long that wait is given (raising it from 10s to 30s made no
 * difference), once a same-named file was left behind by an earlier
 * partial/failed upload attempt (this cert is re-uploaded fresh on every
 * publish -- see this file's earlier comment on AT+QSSLCFG -- so a stale
 * file from a prior run is expected, not a one-off). Deleting first makes
 * every attempt start from a clean slate; the result is deliberately
 * ignored -- "OK" (file existed, now gone) and "ERROR" (no such file) are
 * both fine outcomes, since either way UFS has no file at this name
 * afterward.
 *
 * The CONNECT prompt itself is a bare, undelimited line (see
 * eg916q_http_get()'s doc comment for the general explanation of why
 * modem_chat can't reliably wait for one of these), so this raw-writes the
 * command (replicating modem_chat's own "\r\n" send-delimiter), sleeps
 * briefly for the module to respond, then hands off to
 * eg916q_write_payload_and_finish().
 */
static int eg916q_qfupl(struct eg916q_data *data, const struct device *uart,
			const char *filename, const char *pem, size_t pem_len)
{
	char cmd[80];
	int len;
	int err;

	len = snprintk(cmd, sizeof(cmd), "AT+QFDEL=\"UFS:%s\"", filename);
	(void)eg916q_run_cmd(&data->chat, cmd, &eg916q_ok_match, 10);

	len = snprintk(cmd, sizeof(cmd), "AT+QFUPL=\"UFS:%s\",%u,100\r\n", filename,
		       (unsigned)pem_len);

	eg916q_raw_write(uart, cmd, (size_t)len);

	k_sleep(K_SECONDS(1));

	err = eg916q_write_payload_and_finish(data, uart, pem, pem_len,
					      &eg916q_qfupl_finish_chat_script);
	if (err != 0) {
		LOG_ERR("QFUPL: upload of %s not acknowledged: %d", filename, err);
		return err;
	}

	return 0;
}

/*
 * One-shot bench HTTP GET of www.google.com for debugging (see
 * cellular_task.cpp): same registration/QICSGP/QIACT setup as
 * eg916q_ping() above, then a plain (non-TLS) HTTP GET over a raw TCP
 * socket via AT+QIOPEN/AT+QISEND/AT+QIRD/AT+QICLOSE -- the same steps as
 * the HTTP-GET half of eg916q_network_selftest() below (see that function's
 * doc comment). Unlike eg916q_network_selftest(), and matching eg916q_ping()
 * above, this always deactivates the PDP context and closes the socket
 * before returning (a GET already tears down its own socket either way),
 * but deliberately leaves the module *powered on* on success -- same
 * ensure_powered_on() reuse as every other bench-debug entry point here.
 *
 * Returns 0 if the GET got a non-empty response back, -ENOTCONN if any
 * later stage (open/send/read) failed, another negative errno for an
 * earlier setup failure (power-on, registration, QICSGP/QIACT, ...).
 */
int eg916q_http_get(const struct device *dev, int min_rssi_dbm, uint32_t retry_interval_s)
{
	const struct eg916q_config *config = dev->config;
	struct eg916q_data *data = dev->data;
	int err;
	bool http_ok = false;

	data->last_qiopen_result = -1;
	data->last_qird_len = -1;

	err = eg916q_ensure_powered_on(dev);
	if (err != 0) {
		LOG_ERR("http get: power-on failed: %d", err);
		return err;
	}

	err = eg916q_wait_ready(data, "http get");
	if (err != 0) {
		goto teardown;
	}

	err = modem_chat_run_script(&data->chat, &eg916q_init_chat_script);
	if (err != 0) {
		LOG_ERR("http get: init chat script failed: %d", err);
		goto teardown;
	}

	err = eg916q_wait_sim_ready(data, "http get");
	if (err != 0) {
		goto teardown;
	}

	err = eg916q_wait_for_signal(data, min_rssi_dbm, retry_interval_s, 0);
	if (err != 0) {
		LOG_ERR("http get: signal check failed: %d", err);
		goto teardown;
	}

	err = eg916q_wait_registered(data);
	if (err != 0) {
		LOG_ERR("http get: network registration timed out: %d", err);
		goto teardown;
	}

	err = modem_chat_run_script(&data->chat, &eg916q_qicsgp_chat_script);
	if (err != 0) {
		LOG_ERR("http get: AT+QICSGP failed: %d", err);
		goto teardown;
	}

	err = modem_chat_run_script(&data->chat, &eg916q_qiact_chat_script);
	if (err != 0) {
		LOG_ERR("http get: AT+QIACT failed: %d", err);
		goto teardown;
	}

	err = modem_chat_run_script(&data->chat, &eg916q_qiopen_chat_script);
	if (err != 0) {
		LOG_ERR("http get: AT+QIOPEN not acknowledged: %d", err);
		goto deactivate;
	}
	if (data->last_qiopen_result != 0) {
		LOG_ERR("http get: AT+QIOPEN reported failure: %d", data->last_qiopen_result);
		goto deactivate;
	}

	/* AT+QISEND's '>' prompt is a bare, undelimited character (no trailing
	 * CRLF -- that's the whole point, so the module can start reading raw
	 * payload bytes immediately after it). modem_chat's parser only ever
	 * fires a match once a full CRLF-terminated line completes (see
	 * modem_chat_process_byte()/modem_chat_parse_end_del_complete() in
	 * subsys/modem/modem_chat.c); a match with no delimiter is found
	 * character-by-character but the script is never advanced, so trying
	 * to wait for '>' via eg916q_run_cmd()/modem_chat_run_script() (as an
	 * earlier version of this function did) deadlocks: modem_chat waits
	 * forever for a CRLF that will never come, while the module is
	 * waiting for us to just start sending. Confirmed on real hardware as
	 * a 10s "eg916q_dynamic: timed out" / "-11" (-EAGAIN) every time.
	 *
	 * Fix: don't go through modem_chat for this step at all. Send the
	 * command as a raw write (replicating modem_chat's own "\r\n"
	 * send-delimiter), sleep briefly for the module to process it and
	 * emit the prompt, then write the payload raw -- we don't need to
	 * see the '>' to know it's coming; the datasheet guarantees it.
	 * modem_chat's own parse buffer is left holding this never-delimited
	 * "> " fragment (nothing ever resets receive_buf_len without a
	 * completed line), which would corrupt the *next* script's matching
	 * (that script counts bytes from the start of receive_buf, so a
	 * stale prefix throws off every match_size comparison) -- so
	 * modem_chat_release()+modem_chat_attach() forces the same parser
	 * reset attach() already does on a fresh pipe, without touching the
	 * still-open UART pipe/GPIO underneath.
	 */
	{
		char cmd[32];
		const int len = snprintk(cmd, sizeof(cmd), "AT+QISEND=0,%u\r\n",
					 (unsigned)(sizeof(EG916Q_HTTP_GET_REQUEST) - 1));

		eg916q_raw_write(config->uart, cmd, (size_t)len);
	}

	k_sleep(K_MSEC(300));

	err = eg916q_write_payload_and_finish(data, config->uart, EG916Q_HTTP_GET_REQUEST,
					      sizeof(EG916Q_HTTP_GET_REQUEST) - 1,
					      &eg916q_qisend_finish_chat_script);
	if (err != 0) {
		LOG_ERR("http get: HTTP GET not sent (no SEND OK): %d", err);
		goto close_and_deactivate;
	}

	/* See eg916q_network_selftest()'s doc comment on this same fixed wait
	 * (a bench sanity check, not a real HTTP client).
	 */
	k_sleep(K_SECONDS(2));

	err = modem_chat_run_script(&data->chat, &eg916q_qird_chat_script);
	if (err != 0) {
		LOG_ERR("http get: AT+QIRD failed: %d", err);
	} else if (data->last_qird_len <= 0) {
		LOG_ERR("http get: got no data back");
		err = -ENOTCONN;
	} else {
		LOG_INF("http get: got %d bytes back", data->last_qird_len);
		http_ok = true;
	}

close_and_deactivate:
	if (modem_chat_run_script(&data->chat, &eg916q_qiclose_chat_script) != 0) {
		LOG_WRN("http get: AT+QICLOSE failed or timed out");
	}
deactivate:
	if (modem_chat_run_script(&data->chat, &eg916q_qideact_chat_script) != 0) {
		LOG_WRN("http get: AT+QIDEACT failed or timed out");
	}

	if (http_ok) {
		/* See eg916q_ping()'s doc comment: leave the module powered on
		 * on success so a caller on a cadence reuses this session.
		 */
		return 0;
	}

teardown:
	eg916q_power_off_and_release(config, data, "http get");
	return err != 0 ? err : -ENOTCONN;
}

int eg916q_network_selftest(const struct device *dev, int min_rssi_dbm, uint32_t retry_interval_s)
{
	const struct eg916q_config *config = dev->config;
	struct eg916q_data *data = dev->data;
	int err;
	bool tests_ran = false;
	bool ping_ok = false;
	bool http_ok = false;

	data->last_qping_result = -1;
	data->last_qiopen_result = -1;
	data->last_qird_len = -1;

	err = eg916q_ensure_powered_on(dev);
	if (err != 0) {
		LOG_ERR("network self-test: power-on failed: %d", err);
		return err;
	}

	err = eg916q_wait_ready(data, "network self-test");
	if (err != 0) {
		goto teardown;
	}

	err = modem_chat_run_script(&data->chat, &eg916q_init_chat_script);
	if (err != 0) {
		LOG_ERR("network self-test: init chat script failed: %d", err);
		goto teardown;
	}

	err = eg916q_wait_sim_ready(data, "network self-test");
	if (err != 0) {
		goto teardown;
	}

	err = eg916q_wait_for_signal(data, min_rssi_dbm, retry_interval_s, 0);
	if (err != 0) {
		LOG_ERR("network self-test: signal check failed: %d", err);
		goto teardown;
	}

	err = eg916q_wait_registered(data);
	if (err != 0) {
		LOG_ERR("network self-test: network registration timed out: %d", err);
		goto teardown;
	}

	err = modem_chat_run_script(&data->chat, &eg916q_qicsgp_chat_script);
	if (err != 0) {
		LOG_ERR("network self-test: AT+QICSGP failed: %d", err);
		goto teardown;
	}

	err = modem_chat_run_script(&data->chat, &eg916q_qiact_chat_script);
	if (err != 0) {
		LOG_ERR("network self-test: AT+QIACT failed: %d", err);
		goto teardown;
	}

	/* --- Ping --- */
	tests_ran = true;
	err = modem_chat_run_script(&data->chat, &eg916q_qping_chat_script);
	if (err != 0) {
		LOG_ERR("network self-test: AT+QPING not acknowledged: %d", err);
	} else if (data->last_qping_result != 0) {
		LOG_ERR("network self-test: ping to www.google.com failed: finresult=%d",
			data->last_qping_result);
	} else {
		LOG_INF("network self-test: ping to www.google.com OK");
		ping_ok = true;
	}

	/* --- HTTP GET over a raw TCP socket --- */
	err = modem_chat_run_script(&data->chat, &eg916q_qiopen_chat_script);
	if (err != 0) {
		LOG_ERR("network self-test: AT+QIOPEN not acknowledged: %d", err);
		goto deactivate;
	}
	if (data->last_qiopen_result != 0) {
		LOG_ERR("network self-test: AT+QIOPEN reported failure: %d",
			data->last_qiopen_result);
		goto deactivate;
	}

	{
		char cmd[32];

		snprintk(cmd, sizeof(cmd), "AT+QISEND=0,%u",
			 (unsigned)(sizeof(EG916Q_HTTP_GET_REQUEST) - 1));
		err = eg916q_run_cmd(&data->chat, cmd, &eg916q_prompt_match, 10);
	}
	if (err != 0) {
		LOG_ERR("network self-test: no '>' prompt for AT+QISEND: %d", err);
		goto close_and_deactivate;
	}

	eg916q_raw_write(config->uart, EG916Q_HTTP_GET_REQUEST,
			 sizeof(EG916Q_HTTP_GET_REQUEST) - 1);

	err = modem_chat_run_script(&data->chat, &eg916q_qisend_finish_chat_script);
	if (err != 0) {
		LOG_ERR("network self-test: HTTP GET not sent (no SEND OK): %d", err);
		goto close_and_deactivate;
	}

	/* Give the server a moment to respond before polling for it -- this
	 * is a bench sanity check, not a real HTTP client, so a fixed wait
	 * instead of the +QIURC: "recv" URC (which would need unsol_matches
	 * wired up) is an acceptable simplification.
	 */
	k_sleep(K_SECONDS(2));

	err = modem_chat_run_script(&data->chat, &eg916q_qird_chat_script);
	if (err != 0) {
		LOG_ERR("network self-test: AT+QIRD failed: %d", err);
	} else if (data->last_qird_len <= 0) {
		LOG_ERR("network self-test: HTTP GET got no data back");
	} else {
		LOG_INF("network self-test: HTTP GET got %d bytes back", data->last_qird_len);
		http_ok = true;
	}

close_and_deactivate:
	if (modem_chat_run_script(&data->chat, &eg916q_qiclose_chat_script) != 0) {
		LOG_WRN("network self-test: AT+QICLOSE failed or timed out");
	}
deactivate:
	if (modem_chat_run_script(&data->chat, &eg916q_qideact_chat_script) != 0) {
		LOG_WRN("network self-test: AT+QIDEACT failed or timed out");
	}
teardown:
	eg916q_power_off_and_release(config, data, "network self-test");

	/* Only report -ENOTCONN for a genuine ping/HTTP failure. If setup never
	 * got that far (module not ready, registration, QIACT...), return that
	 * stage's own error instead -- otherwise e.g. an init-script timeout
	 * (-EAGAIN) surfaces in the app log as a misleading -ENOTCONN.
	 */
	if (!tests_ran) {
		return err;
	}
	if (!ping_ok || !http_ok) {
		return -ENOTCONN;
	}
	return 0;
}

/* Gives up between stages once the caller's session deadline has passed. A
 * stage already running is allowed to finish (each has its own AT timeout),
 * so the real overrun is at most one stage.
 */
#define EG916Q_CHECK_DEADLINE(label)                                                               \
	do {                                                                                       \
		if (eg916q_deadline_passed(params->deadline_ms)) {                                 \
			LOG_WRN("mqtt publish: session time limit reached; giving up");            \
			err = -ETIMEDOUT;                                                          \
			goto label;                                                                \
		}                                                                                  \
	} while (0)

int eg916q_mqtt_publish(const struct device *dev, const struct eg916q_mqtt_publish_params *params)
{
	const struct eg916q_config *config = dev->config;
	struct eg916q_data *data = dev->data;
	int err;

	data->last_qmtopen_result = -1;
	data->last_qmtconn_result = -1;
	data->last_qmtconn_ret_code = -1;
	data->last_qmtpubex_result = -1;

	err = eg916q_ensure_powered_on(dev);
	if (err != 0) {
		LOG_ERR("mqtt publish: power-on failed: %d", err);
		return err;
	}

	err = eg916q_wait_ready(data, "mqtt publish");
	if (err != 0) {
		goto teardown;
	}

	err = modem_chat_run_script(&data->chat, &eg916q_init_chat_script);
	if (err != 0) {
		LOG_ERR("mqtt publish: init chat script failed: %d", err);
		goto teardown;
	}

	err = eg916q_wait_sim_ready(data, "mqtt publish");
	if (err != 0) {
		goto teardown;
	}

	err = eg916q_wait_for_signal(data, params->min_rssi_dbm, params->signal_retry_interval_s,
				     params->deadline_ms);
	if (err != 0) {
		LOG_ERR("mqtt publish: signal check failed: %d", err);
		goto teardown;
	}

	err = eg916q_wait_registered(data);
	if (err != 0) {
		LOG_ERR("mqtt publish: network registration timed out: %d", err);
		goto teardown;
	}
	EG916Q_CHECK_DEADLINE(teardown);

	err = modem_chat_run_script(&data->chat, &eg916q_qicsgp_chat_script);
	if (err != 0) {
		LOG_ERR("mqtt publish: AT+QICSGP failed: %d", err);
		goto teardown;
	}

	err = modem_chat_run_script(&data->chat, &eg916q_qiact_chat_script);
	if (err != 0) {
		LOG_ERR("mqtt publish: AT+QIACT failed: %d", err);
		goto teardown;
	}
	EG916Q_CHECK_DEADLINE(deactivate);

	err = eg916q_qfupl(data, config->uart, "cacert.pem", params->ca_cert_pem,
			   strlen(params->ca_cert_pem));
	if (err != 0) {
		LOG_ERR("mqtt publish: CA cert upload failed: %d", err);
		goto deactivate;
	}

	err = modem_chat_run_script(&data->chat, &eg916q_qsslcfg_chat_script);
	if (err != 0) {
		LOG_ERR("mqtt publish: AT+QSSLCFG failed: %d", err);
		goto deactivate;
	}

	err = modem_chat_run_script(&data->chat, &eg916q_qmtcfg_ssl_chat_script);
	if (err != 0) {
		LOG_ERR("mqtt publish: AT+QMTCFG=\"ssl\" failed: %d", err);
		goto deactivate;
	}

	{
		char cmd[128];

		snprintk(cmd, sizeof(cmd), "AT+QMTOPEN=" STRINGIFY(EG916Q_MQTT_CLIENT_IDX) ",\"%s\",%u",
			 params->broker_host, params->broker_port);
		err = eg916q_run_cmd(&data->chat, cmd, &eg916q_ok_match, 10);
	}
	if (err != 0) {
		LOG_ERR("mqtt publish: AT+QMTOPEN not acknowledged: %d", err);
		goto deactivate;
	}
	err = modem_chat_run_script(&data->chat, &eg916q_qmtopen_wait_chat_script);
	if (err != 0) {
		LOG_ERR("mqtt publish: no +QMTOPEN result: %d", err);
		goto deactivate;
	}
	if (data->last_qmtopen_result != 0) {
		LOG_ERR("mqtt publish: AT+QMTOPEN reported failure: %d", data->last_qmtopen_result);
		err = -ENOTCONN;
		goto deactivate;
	}
	EG916Q_CHECK_DEADLINE(close_and_deactivate);

	{
		char cmd[160];

		snprintk(cmd, sizeof(cmd),
			 "AT+QMTCONN=" STRINGIFY(EG916Q_MQTT_CLIENT_IDX) ",\"%s\",\"%s\",\"%s\"",
			 params->client_id, params->username != NULL ? params->username : "",
			 params->password != NULL ? params->password : "");
		err = eg916q_run_cmd(&data->chat, cmd, &eg916q_ok_match, 10);
	}
	if (err != 0) {
		LOG_ERR("mqtt publish: AT+QMTCONN not acknowledged: %d", err);
		goto close_and_deactivate;
	}
	err = modem_chat_run_script(&data->chat, &eg916q_qmtconn_wait_chat_script);
	if (err != 0) {
		LOG_ERR("mqtt publish: no +QMTCONN result: %d", err);
		goto close_and_deactivate;
	}
	if (data->last_qmtconn_result != 0) {
		LOG_ERR("mqtt publish: AT+QMTCONN reported failure: %d", data->last_qmtconn_result);
		err = -ECONNREFUSED;
		goto close_and_deactivate;
	}
	if (data->last_qmtconn_ret_code != 0) {
		static const char *const reasons[] = {
			"accepted", "unacceptable protocol version", "client ID rejected",
			"server unavailable", "bad username or password", "not authorized",
		};
		const int rc = data->last_qmtconn_ret_code;

		LOG_ERR("mqtt publish: broker refused connection: CONNACK %d (%s)", rc,
			(rc >= 0 && rc < (int)ARRAY_SIZE(reasons)) ? reasons[rc] : "unknown");
		err = -ECONNREFUSED;
		goto close_and_deactivate;
	}
	EG916Q_CHECK_DEADLINE(disconnect_close_and_deactivate);

	/* AT+QMTPUBEX's '>' prompt has the same problem as AT+QISEND's (see
	 * eg916q_http_get()'s doc comment for the full explanation): it's a
	 * bare, undelimited character, so modem_chat can never actually
	 * observe it via eg916q_run_cmd()/modem_chat_run_script() -- confirmed
	 * on real hardware as the exact same 10s timeout ("-11"/-EAGAIN).
	 * Same fix: raw-write the command (replicating modem_chat's own
	 * "\r\n" send-delimiter), sleep briefly for the prompt, then hand off
	 * to eg916q_write_payload_and_finish() -- this reply's "OK" comes back
	 * almost instantly, which is what exposed the arm-before-write race
	 * that helper's doc comment describes.
	 */
	{
		char cmd[160];
		int len;

		/* QoS 1, retain 0, message ID 1 (only ever one publish in
		 * flight at a time, so a fixed ID is fine).
		 */
		len = snprintk(cmd, sizeof(cmd),
			       "AT+QMTPUBEX=" STRINGIFY(
				       EG916Q_MQTT_CLIENT_IDX) ",1,1,0,\"%s\",%u\r\n",
			       params->topic, params->payload_len);

		eg916q_raw_write(config->uart, cmd, (size_t)len);
	}

	k_sleep(K_MSEC(300));

	err = eg916q_write_payload_and_finish(data, config->uart, params->payload,
					      params->payload_len,
					      &eg916q_qmtpubex_finish_chat_script);
	if (err != 0) {
		LOG_ERR("mqtt publish: AT+QMTPUBEX not acknowledged: %d", err);
		goto disconnect_close_and_deactivate;
	}
	if (data->last_qmtpubex_result != 0) {
		LOG_ERR("mqtt publish: AT+QMTPUBEX reported failure: %d",
			data->last_qmtpubex_result);
		err = -EIO;
		goto disconnect_close_and_deactivate;
	}

	LOG_INF("mqtt publish: broker acknowledged publish");

disconnect_close_and_deactivate:
	if (modem_chat_run_script(&data->chat, &eg916q_qmtdisc_chat_script) != 0) {
		LOG_WRN("mqtt publish: AT+QMTDISC failed or timed out");
	}
close_and_deactivate:
	if (modem_chat_run_script(&data->chat, &eg916q_qmtclose_chat_script) != 0) {
		LOG_WRN("mqtt publish: AT+QMTCLOSE failed or timed out");
	}
deactivate:
	if (modem_chat_run_script(&data->chat, &eg916q_qideact_chat_script) != 0) {
		LOG_WRN("mqtt publish: AT+QIDEACT failed or timed out");
	}
teardown:
	/* On success the module is deliberately left powered on and registered, so
	 * the next publish skips the PWRKEY pulse, boot wait and network attach
	 * (~30-40 s). It is powered off by eg916q_power_down() before the MCU
	 * sleeps. After a failure, power it off: a clean power cycle is the only
	 * reliable way to clear whatever state the module was left in.
	 */
	if (err != 0) {
		eg916q_power_off_and_release(config, data, "mqtt publish");
	}

	return err;
}

/* ---------------------------------------------------------------------
 * Init
 * ------------------------------------------------------------------ */

static int eg916q_init(const struct device *dev)
{
	const struct eg916q_config *config = dev->config;
	struct eg916q_data *data = dev->data;
	int err;

	err = gpio_pin_configure_dt(&config->power_gpio, GPIO_OUTPUT_INACTIVE);
	if (err != 0) {
		LOG_ERR("failed to configure power gpio: %d", err);
		return err;
	}

	if (eg916q_gpio_is_enabled(&config->reset_gpio)) {
		err = gpio_pin_configure_dt(&config->reset_gpio, GPIO_OUTPUT_INACTIVE);
		if (err != 0) {
			LOG_ERR("failed to configure reset gpio: %d", err);
			return err;
		}
	}

	const struct modem_backend_uart_config uart_backend_config = {
		.uart = config->uart,
		.receive_buf = data->uart_backend_receive_buf,
		.receive_buf_size = sizeof(data->uart_backend_receive_buf),
		.transmit_buf = data->uart_backend_transmit_buf,
		.transmit_buf_size = sizeof(data->uart_backend_transmit_buf),
	};

	data->uart_pipe = modem_backend_uart_init(&data->uart_backend, &uart_backend_config);
	if (data->uart_pipe == NULL) {
		LOG_ERR("failed to init UART backend");
		return -ENODEV;
	}

	const struct modem_chat_config chat_config = {
		.user_data = data,
		.receive_buf = data->chat_receive_buf,
		.receive_buf_size = sizeof(data->chat_receive_buf),
		.delimiter = (uint8_t *)"\r\n",
		.delimiter_size = 2,
		.filter = NULL,
		.filter_size = 0,
		.argv = data->chat_argv,
		.argv_size = ARRAY_SIZE(data->chat_argv),
		.unsol_matches = eg916q_unsol_matches,
		.unsol_matches_size = ARRAY_SIZE(eg916q_unsol_matches),
	};

	err = modem_chat_init(&data->chat, &chat_config);
	if (err != 0) {
		LOG_ERR("failed to init chat: %d", err);
		return err;
	}

	/* No automatic power-on/resume here: every entry point in this file
	 * manages its own power via eg916q_ensure_powered_on()/
	 * eg916q_power_off_and_release() (eg916q_selftest() is the one that
	 * leaves the module on for the next call, rather than tearing down
	 * immediately -- see its doc comment). This device does not
	 * participate in Zephyr's PM_DEVICE framework at all (see the file
	 * header comment) even though CONFIG_PM_DEVICE is enabled -- that
	 * Kconfig option is needed by the in-tree GNSS driver instead (see
	 * gps_task.cpp's start()).
	 */
	return 0;
}

/* ---------------------------------------------------------------------
 * Instantiation
 * ------------------------------------------------------------------ */

#define EG916Q_INIT(inst)                                                                          \
	static const struct eg916q_config eg916q_config_##inst = {                                 \
		.uart = DEVICE_DT_GET(DT_INST_BUS(inst)),                                          \
		.power_gpio = GPIO_DT_SPEC_INST_GET(inst, mdm_power_gpios),                        \
		.reset_gpio = GPIO_DT_SPEC_INST_GET_OR(inst, mdm_reset_gpios, {0}),                \
	};                                                                                         \
                                                                                                   \
	static struct eg916q_data eg916q_data_##inst;                                              \
                                                                                                   \
	DEVICE_DT_INST_DEFINE(inst, eg916q_init, NULL, &eg916q_data_##inst,                        \
			      &eg916q_config_##inst, POST_KERNEL,                                  \
			      CONFIG_MODEM_QUECTEL_EG916Q_INIT_PRIORITY, NULL);

DT_INST_FOREACH_STATUS_OKAY(EG916Q_INIT)
