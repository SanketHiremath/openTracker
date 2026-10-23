# EG916Q-GL Consolidation Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Replace the NEO-6M GNSS + SARA-R5 cellular + ANNA BLE-gate hardware with a single Quectel EG916Q-GL, per `docs/superpowers/specs/2026-09-22-eg916q-consolidation-design.md`.

**Architecture:** Delete the ANNA/uart_mux pin-sharing scheme and the u-blox-specific GPS standby/wake code (both made obsolete by the hardware change). Add a new, self-contained, synchronous EG916Q-GL modem driver under `drivers/modem/` in this module, built on Zephyr's public `modem_chat`/`modem_pipe`/`modem_ppp`/`modem_backend_uart` primitives — the same building blocks `zephyr/drivers/modem/modem_cellular.c` itself uses, but without touching that file or anything else under `zephyr/`. `modem_link.cpp` and `gps_task.cpp`'s NMEA path are unaffected, since both already reach hardware only through the `modem`/`gnss` devicetree aliases and standard Zephyr APIs.

**Tech Stack:** Zephyr RTOS modem subsystem (`zephyr/modem/{chat,pipe,ppp,backend/uart}.h`), devicetree, Kconfig, nRF52840 GPIO/UARTE.

**Verification method:** This repo has no unit test framework (see CLAUDE.md) — the equivalent of "run the test" in every task below is `west build` for the affected config(s), using the toolchain confirmed available in this environment:
```bash
export ZEPHYR_TOOLCHAIN_VARIANT=zephyr
export ZEPHYR_SDK_INSTALL_DIR=/Users/sankethiremath/zephyr-sdk-0.17.4
source /Users/sankethiremath/zephyrproject/.venv/bin/activate
cd /Users/sankethiremath/zephyrproject/car_tracker
```
Run this once per shell before any `west build` step below.

---

### Task 1: Remove the ANNA BLE-gate / uart_mux pin-sharing scheme

**Files:**
- Delete: `src/services/uart_mux.cpp`
- Delete: `src/services/uart_mux.hpp`
- Modify: `app.overlay`
- Modify: `Kconfig`
- Modify: `CMakeLists.txt`

`uart_mux` has no call sites anywhere in `src/` (confirmed: `grep -rn "uart_mux" src/` only matches its own two files), so this is pure deletion with no follow-on edits to task code.

- [ ] **Step 1: Delete the uart_mux service files**

```bash
rm src/services/uart_mux.cpp src/services/uart_mux.hpp
```

- [ ] **Step 2: Remove the ANNA pinctrl state and third pinctrl-name from app.overlay**

In `app.overlay`, remove the `pinctrl-2` line and the comment above it, and drop `"anna"` from `pinctrl-names`:

Change:
```
&uart0 {
	status = "okay";
	current-speed = <9600>;   /* NEO-6M factory default */

	/* Third pinctrl state routes this same UARTE peripheral to the ANNA
	 * BLE module instead of the GPS -- see the uart0_anna comment in
	 * &pinctrl below and src/services/uart_mux.cpp for the switch.
	 */
	pinctrl-2 = <&uart0_anna>;
	pinctrl-names = "default", "sleep", "anna";

	gnss: gnss-nmea-generic {
		compatible = "gnss-nmea-generic";
	};
};
```
to:
```
&uart0 {
	status = "okay";
	current-speed = <9600>;   /* EG916Q-GL GNSS UART default */
	pinctrl-names = "default", "sleep";

	gnss: gnss-nmea-generic {
		compatible = "gnss-nmea-generic";
	};
};
```

Then remove the entire `uart0_anna: uart0_anna { ... }` block from `&pinctrl`, including its leading comment block (the one starting `/* Alternate routing for uart0's UARTE peripheral...`). Leave `uart1_default` and `uart1_sleep` untouched — those stay in Task 3.

- [ ] **Step 3: Remove CAR_TRACKER_UART_MUX from Kconfig**

In `Kconfig`, delete this entire block:
```
config CAR_TRACKER_UART_MUX
	bool "Runtime uart0 pin routing between the GPS and the ANNA BLE module"
	default y
	select PINCTRL_NON_STATIC
	help
	  The nRF52840 has only two UARTE instances but this design needs
	  three UART peers: the GPS, the ANNA BLE module that gates power to
	  the cellular modem, and the modem itself. uart1 stays dedicated to
	  the modem; uart0's peripheral is time-shared by rewriting its pin
	  selection at runtime (see src/services/uart_mux.cpp and the
	  uart0_anna pinctrl state in app.overlay).

	  PINCTRL_NON_STATIC is required so uart0's pinctrl configuration is
	  reachable from outside the UART driver; it has no prompt of its own
	  and can only be turned on by a select like this one.

```

- [ ] **Step 4: Remove uart_mux.cpp and PINCTRL_STATE_ANNA from CMakeLists.txt**

In `CMakeLists.txt`, remove this comment+line:
```
# The "anna" pinctrl state on uart0 (app.overlay) resolves to this id.
# It has to be a global definition, not an app-local one: the UART driver's
# own translation unit is what expands PINCTRL_STATE_<NAME> when it builds
# uart0's pinctrl state table.
zephyr_compile_definitions(PINCTRL_STATE_ANNA=PINCTRL_STATE_PRIV_START)

```
and remove this line from the `target_sources(app PRIVATE ...)` list:
```
  src/services/uart_mux.cpp
```

- [ ] **Step 5: Build-verify with the GPS-only config**

```bash
west build -p always -b promicro_nrf52840/nrf52840/uf2 -d build-gps-only . -- -DFILE_SUFFIX=gps_only
```
Expected: build succeeds (this config never referenced uart_mux or ANNA, so this mainly confirms the overlay/pinctrl edit didn't break devicetree parsing).

- [ ] **Step 6: Commit**

```bash
git add -A src/services Kconfig CMakeLists.txt app.overlay
git commit -m "$(cat <<'EOF'
Remove ANNA BLE-gate / uart_mux pin-sharing scheme

The EG916Q-GL replaces both the NEO-6M and the SARA-R5 with a single
module, eliminating the third UART peer (ANNA) that uart_mux existed
to time-share uart0 with. uart_mux had no call sites in src/.

Co-Authored-By: Claude Sonnet 5 <noreply@anthropic.com>
EOF
)"
```

---

### Task 2: Remove u-blox-specific GPS standby/wake and its BLE test characteristic

**Files:**
- Modify: `src/tasks/gps_task.cpp`
- Modify: `src/tasks/gps_task.hpp`
- Modify: `src/tasks/ble_task.cpp`
- Modify: `src/app_config.hpp`

These send raw UBX-RXM-PMREQ bytes, meaningless to Quectel's GNSS engine (see design doc). Only the GPS+BLE bring-up test's command characteristic calls them; the fix-notify characteristic is untouched.

- [ ] **Step 1: Remove UBX code and standby()/wake() from gps_task.cpp**

Remove the `kGnssUartDev` declaration and its preceding comment (lines 38-45):
```cpp
/* The gnss-nmea-generic driver owns uart0 for NMEA *reception* (RX pipe/
 * chat), but that doesn't stop us writing to the same physical UART's TX
 * line directly -- uart_poll_out() just pushes a byte into the peripheral's
 * TX path and does not touch the driver's RX-side state, so the two are
 * safe to interleave. This is how standby()/wake() below reach the NEO-6M
 * with raw UBX bytes despite the driver only speaking NMEA.
 */
const struct device *const kGnssUartDev = DEVICE_DT_GET(DT_BUS(DT_ALIAS(gnss)));
```

Remove the entire UBX protocol block (the comment header, `kUbxSync1`/`kUbxSync2`/`kUbxClassRxm`/`kUbxIdPmreq` constants, `build_pmreq_backup_payload()`, and `ubx_send()` — everything between that removed block and `/* Raw samples handed from driver callback context to the GPS thread. */`).

Remove the `standby()` and `wake()` function definitions at the end of the file (from `void standby()` through the end of `wake()`'s closing brace), but keep the file's closing `} /* namespace ct::gps */` — check whether one exists after `wake()`; if `start()` is the last function before `standby()`, the namespace closing brace comes after `wake()`, so remove only the two function bodies and leave the brace.

Remove the now-unused `#include <zephyr/drivers/uart.h>` at the top (uart_poll_out was its only use).

- [ ] **Step 2: Remove standby()/wake() declarations from gps_task.hpp**

In `src/tasks/gps_task.hpp`, remove everything from the `/* Requests UBX backup mode...` comment through the end of `wake();`'s declaration and its preceding comment, leaving only:
```cpp
#pragma once

namespace ct::gps {

/* Verifies the GNSS device and starts the acquisition thread.
 * Returns 0 on success, -ENODEV if the GNSS device is not ready.
 */
int start();

} /* namespace ct::gps */
```

- [ ] **Step 3: Remove the GPS command characteristic from ble_task.cpp**

Remove the `#include "gps_task.hpp"` line (it's only used by `gps_cmd_write`).

Remove `kGpsCmdCharUuid`:
```cpp
bt_uuid_128 kGpsCmdCharUuid = BT_UUID_INIT_128(
	BT_UUID_128_ENCODE(0x6f524303, 0x1234, 0x5678, 0x9abc, 0x123456789abc));
```

Remove the entire `gps_cmd_write()` function and its preceding comment block (`/* Write-only command characteristic...`).

Change the `BT_GATT_SERVICE_DEFINE` from:
```cpp
BT_GATT_SERVICE_DEFINE(gps_svc, BT_GATT_PRIMARY_SERVICE(&kGpsSvcUuid),
			BT_GATT_CHARACTERISTIC(&kGpsFixCharUuid.uuid, BT_GATT_CHRC_NOTIFY,
						BT_GATT_PERM_NONE, nullptr, nullptr, nullptr),
			BT_GATT_CCC(fix_ccc_cfg_changed,
				    BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
			BT_GATT_CHARACTERISTIC(&kGpsCmdCharUuid.uuid, BT_GATT_CHRC_WRITE,
						BT_GATT_PERM_WRITE, nullptr, gps_cmd_write,
						nullptr));

/* attrs[0] = service, [1] = fix characteristic decl, [2] = fix value attr
 * (notify target), [3] = fix CCC descriptor, [4] = cmd characteristic decl,
 * [5] = cmd value attr (gps_cmd_write's attr) -- standard layout for this
 * macro sequence.
 */
```
to:
```cpp
BT_GATT_SERVICE_DEFINE(gps_svc, BT_GATT_PRIMARY_SERVICE(&kGpsSvcUuid),
			BT_GATT_CHARACTERISTIC(&kGpsFixCharUuid.uuid, BT_GATT_CHRC_NOTIFY,
						BT_GATT_PERM_NONE, nullptr, nullptr, nullptr),
			BT_GATT_CCC(fix_ccc_cfg_changed,
				    BT_GATT_PERM_READ | BT_GATT_PERM_WRITE));

/* attrs[0] = service, [1] = fix characteristic decl, [2] = fix value attr
 * (notify target), [3] = fix CCC descriptor.
 */
```

- [ ] **Step 4: Remove kGpsBackupMaxDurationMs from app_config.hpp**

In `src/app_config.hpp`, remove the `GPS power management` section entirely:
```cpp
/* ---------------------------------------------------------------------
 * GPS power management
 * ------------------------------------------------------------------ */

/* Upper bound on a single ct::gps::standby() request (UBX-RXM-PMREQ
 * duration field). GPS ephemeris is only valid for ~4 hours after
 * broadcast; once it expires, the next fix degrades from a hot start to a
 * warm/cold one. This is kept far under that limit so the receiver's own
 * PMREQ timer wakes it on schedule even if nothing ever sends an explicit
 * wake -- ephemeris stays fresh, and the tracker never gets stuck asleep.
 */
inline constexpr std::uint32_t kGpsBackupMaxDurationMs = 30U * 60U * 1000U; /* 30 min */

```

- [ ] **Step 5: Build-verify with the GPS+BLE-test config**

```bash
west build -p always -b promicro_nrf52840/nrf52840/uf2 -d build-gps-ble-test . -- -DFILE_SUFFIX=gps_ble_test
```
Expected: build succeeds. This config compiles both `gps_task.cpp` and `ble_task.cpp`'s BLE-test GATT service, so it's the one build that exercises every edit in this task.

- [ ] **Step 6: Build-verify the other three configs still compile (gps_task.cpp change affects all of them)**

```bash
west build -p always -b promicro_nrf52840/nrf52840/uf2 -d build-gps-only . -- -DFILE_SUFFIX=gps_only
west build -p always -b promicro_nrf52840/nrf52840 -d build-nogps . -- -DFILE_SUFFIX=nogps -DEXTRA_DTC_OVERLAY_FILE=no_gps.overlay
```
Expected: both succeed. (`build-full` is left for Task 5, once the modem driver exists — it would fail to link right now because `app.overlay` hasn't been touched yet, so this is unaffected either way at this point.)

- [ ] **Step 7: Commit**

```bash
git add -A src/tasks src/app_config.hpp
git commit -m "$(cat <<'EOF'
Remove u-blox UBX GPS standby/wake, unused on Quectel GNSS

RXM-PMREQ is u-blox-proprietary and meaningless to the EG916Q-GL's
GNSS engine. Only the GPS+BLE bring-up test's command characteristic
called it; production policy never did. Quectel GNSS power control
(AT+QGPS/AT+QGPSEND) is deferred to a future supervisor-owned policy
if needed -- see the design doc's "Removed capabilities" section.

Co-Authored-By: Claude Sonnet 5 <noreply@anthropic.com>
EOF
)"
```

---

### Task 3: EG916Q-GL devicetree binding and app.overlay rewiring

**Files:**
- Create: `dts/bindings/modem/quectel,eg916q.yaml`
- Modify: `app.overlay`

- [ ] **Step 1: Create the devicetree binding directory and file**

```bash
mkdir -p dts/bindings/modem
```

Create `dts/bindings/modem/quectel,eg916q.yaml`:
```yaml
description: |
  Quectel EG916Q-GL LTE Cat 1 + GNSS modem, AT/PPP UART.

  This binds only the module's AT/PPP UART (used for cellular). The
  module's separate, dedicated GNSS UART is unrelated to this binding --
  it streams plain NMEA-0183 and is described by the in-tree
  gnss-nmea-generic binding instead, same as it was for the NEO-6M this
  module replaced.

compatible: "quectel,eg916q"

include: uart-device.yaml

properties:
  mdm-power-gpios:
    type: phandle-array
    required: true
    description: |
      PWRKEY pin. Pulsed active for CONFIG_MODEM_QUECTEL_EG916Q_POWER_PULSE_MS
      to power the module on; driven active again on suspend to power it off.

  mdm-reset-gpios:
    type: phandle-array
    description: |
      Optional RESET_N pin for a hard reset. Not driven during normal
      power-on/power-off; the driver only configures it as an inactive
      output at init.
```

- [ ] **Step 2: Rewrite app.overlay's modem node**

Change:
```
&uart1 {
	compatible = "nordic,nrf-uarte";
	status = "okay";
	current-speed = <115200>;
	hw-flow-control;
	pinctrl-0 = <&uart1_default>;
	pinctrl-1 = <&uart1_sleep>;
	pinctrl-names = "default", "sleep";

	modem: modem {
		compatible = "u-blox,sara-r5"; /* switch to "u-blox,sara-r4" if that's the fitted part */
		mdm-power-gpios = <&gpio0 24 GPIO_ACTIVE_HIGH>;
		mdm-reset-gpios = <&gpio0 19 GPIO_ACTIVE_HIGH>;
		status = "okay";
	};
};
```
to:
```
&uart1 {
	compatible = "nordic,nrf-uarte";
	status = "okay";
	current-speed = <115200>;
	hw-flow-control;
	pinctrl-0 = <&uart1_default>;
	pinctrl-1 = <&uart1_sleep>;
	pinctrl-names = "default", "sleep";

	modem: modem {
		compatible = "quectel,eg916q";
		/* Same PWRKEY/RESET_N GPIO assignment as the SARA-R5 this
		 * replaces -- confirm against the EG916Q-GL carrier board's
		 * actual wiring before flashing (see this file's header
		 * comment on pin selection).
		 */
		mdm-power-gpios = <&gpio0 24 GPIO_ACTIVE_HIGH>;
		mdm-reset-gpios = <&gpio0 19 GPIO_ACTIVE_HIGH>;
		status = "okay";
	};
};
```

Also update the file's header comment block: change
```
 * Cellular modem (M2): SARA-R4/R5 on uart1, added below. uart1 is not
 * enabled by the board by default, so both the peripheral and its
 * pinctrl states are defined here.
```
to
```
 * Cellular + GNSS (M2): a single Quectel EG916Q-GL provides both. Its
 * AT/PPP UART is uart1, added below (not enabled by the board by
 * default, so both the peripheral and its pinctrl states are defined
 * here). Its separate, dedicated GNSS UART is uart0 above, wired
 * identically to how the NEO-6M was -- the EG916Q-GL's GNSS output is
 * plain NMEA-0183, same as the module it replaces.
```
and change the `gnss: gnss-nmea-generic` comment `/* NEO-6M factory default */` (already updated to `/* EG916Q-GL GNSS UART default */` in Task 1) — no further change needed there.

- [ ] **Step 3: Build-verify devicetree parses (expect a link failure, not a DT error)**

```bash
west build -p always -b promicro_nrf52840/nrf52840/uf2 -d build-gps-only . -- -DFILE_SUFFIX=gps_only
```
Expected: succeeds (this config compiles no cellular code, so it never references the `modem` alias — confirms the binding YAML is well-formed on its own).

```bash
west build -p always -b promicro_nrf52840/nrf52840 -d build-full .
```
Expected: **fails at the link step** with an undefined-reference error naming the EG916Q-GL device symbol (something like `undefined reference to '__device_dts_ord_N'`), because `modem_link.cpp` calls `DEVICE_DT_GET(DT_ALIAS(modem))` but no driver yet defines that device. This is the expected, informative failure for this step — it confirms devicetree correctly resolved `compatible = "quectel,eg916q"` against the new binding (if the binding were missing or malformed, this would instead fail earlier, at the CMake devicetree-generation step, with a "compatible with no matching binding" or similar error). Do not treat this link failure as a problem to fix here; Task 4 resolves it.

- [ ] **Step 4: Commit**

```bash
git add -A dts app.overlay
git commit -m "$(cat <<'EOF'
Add EG916Q-GL devicetree binding, rewire modem node

app.overlay's modem node now targets quectel,eg916q instead of
u-blox,sara-r5. No in-tree Zephyr binding exists for this part, so
this adds one under dts/bindings/modem/ in this module (wired in via
DTS_ROOT in the next commit). The GNSS node is untouched -- the
EG916Q-GL's dedicated GNSS UART still speaks plain NMEA-0183, same as
the NEO-6M it replaces.

Expected intermediate state: build-full fails to LINK until the driver
lands in the next commit (devicetree itself resolves cleanly).

Co-Authored-By: Claude Sonnet 5 <noreply@anthropic.com>
EOF
)"
```

---

### Task 4: EG916Q-GL modem driver

**Files:**
- Create: `drivers/modem/Kconfig`
- Create: `drivers/modem/CMakeLists.txt`
- Create: `drivers/modem/quectel_eg916q.c`
- Modify: `Kconfig` (top-level)
- Modify: `CMakeLists.txt` (top-level)

This driver is written from scratch against Zephyr's public `modem_chat`/`modem_pipe`/`modem_ppp`/`modem_backend_uart` APIs (headers under `zephyr/include/zephyr/modem/`) — the same primitives `zephyr/drivers/modem/modem_cellular.c` itself is built from, confirmed independent of `CONFIG_MODEM_CELLULAR`. It is deliberately simpler than that generic driver: this board has exactly one modem with one fixed lifecycle (attach fully over its AT/PPP UART, run PPP exclusively, detach), and its GNSS is already on a **separate** UART — so there is no need for `modem_cellular.c`'s CMUX multiplexing (which exists to split AT and PPP traffic over a *single* shared UART for modems that don't have a second channel). Power management is synchronous (blocks inside `PM_DEVICE_ACTION_RESUME`/`SUSPEND` until done or failed) rather than event/workqueue-driven, because the only caller (`modem_link.cpp`, from the dedicated `cellular_task` thread) is already designed to own long blocking I/O — see CLAUDE.md's Cellular section.

**Known open risk, carried from the design doc:** the AT command sequence below is modeled on the `quectel,eg800q` chat scripts in `modem_cellular.c` (the nearest in-tree relative), not verified against an EG916Q-GL AT command manual. It will very likely need adjustment during hardware bring-up. The scripts are isolated data (`MODEM_CHAT_SCRIPT_CMDS_DEFINE` tables), so such fixes are localized edits, not structural changes.

- [ ] **Step 1: Create the driver directory and Kconfig**

```bash
mkdir -p drivers/modem
```

Create `drivers/modem/Kconfig`:
```kconfig
# Quectel EG916Q-GL modem driver (AT/PPP UART only -- GNSS is handled by the
# in-tree gnss-nmea-generic driver on the module's separate GNSS UART).

config MODEM_QUECTEL_EG916Q
	bool "Quectel EG916Q-GL modem driver"
	select MODEM_MODULES
	select MODEM_CHAT
	select MODEM_PPP
	select MODEM_BACKEND_UART
	select RING_BUFFER
	select NET_L2_PPP_MGMT
	select GPIO
	depends on NET_L2_PPP
	help
	  Devicetree-bound driver (compatible = "quectel,eg916q") for the
	  EG916Q-GL's AT/PPP UART. Talks AT commands then dial-up PPP over a
	  single UART -- no CMUX, since this board's GNSS is already on a
	  separate UART and nothing needs concurrent AT access once PPP is
	  up. Built on Zephyr's public modem_chat/modem_pipe/modem_ppp/
	  modem_backend_uart primitives; independent of CONFIG_MODEM_CELLULAR.

if MODEM_QUECTEL_EG916Q

config MODEM_QUECTEL_EG916Q_APN
	string "APN"
	default "internet"
	help
	  Set to your SIM carrier's APN. Baked into the init chat script's
	  AT+CGDCONT command at compile time.

config MODEM_QUECTEL_EG916Q_UART_BUFFER_SIZE
	int "UART backend receive/transmit buffer size"
	default 512

config MODEM_QUECTEL_EG916Q_POWER_PULSE_MS
	int "PWRKEY pulse duration (ms)"
	default 1500
	help
	  Placeholder pending confirmation against the EG916Q-GL hardware
	  design manual's PWRKEY timing spec (see the driver's header
	  comment on the open AT-script verification risk).

config MODEM_QUECTEL_EG916Q_STARTUP_TIME_MS
	int "Wait after the power pulse before sending AT commands (ms)"
	default 10000

config MODEM_QUECTEL_EG916Q_REGISTER_TIMEOUT_S
	int "Network registration timeout (s)"
	default 60
	help
	  Upper bound on how long eg916q_wait_registered() polls AT+CGREG?
	  before PM_DEVICE_ACTION_RESUME gives up and returns -ETIMEDOUT.

config MODEM_QUECTEL_EG916Q_INIT_PRIORITY
	int "Driver init priority"
	default 90

endif # MODEM_QUECTEL_EG916Q
```

Create `drivers/modem/CMakeLists.txt`:
```cmake
zephyr_library()
zephyr_library_sources_ifdef(CONFIG_MODEM_QUECTEL_EG916Q quectel_eg916q.c)
```

- [ ] **Step 2: Write the driver**

Create `drivers/modem/quectel_eg916q.c`:
```c
/*
 * Quectel EG916Q-GL modem driver (AT/PPP UART).
 *
 * Written from scratch against Zephyr's public modem subsystem headers
 * (zephyr/modem/chat.h, pipe.h, ppp.h, backend/uart.h) -- the same
 * primitives zephyr/drivers/modem/modem_cellular.c is itself built from,
 * confirmed independent of CONFIG_MODEM_CELLULAR. Nothing under zephyr/ is
 * modified.
 *
 * Deliberately simpler than modem_cellular.c: this board wires exactly one
 * EG916Q-GL, whose GNSS output is on its OWN dedicated UART (see the gnss
 * node in app.overlay) -- so there is no need for CMUX to multiplex AT and
 * PPP traffic onto a single shared UART, the way modem_cellular.c must for
 * modems that only expose one channel. This UART carries AT chat commands
 * until the dial script succeeds, then is handed directly to PPP.
 *
 * Power management is synchronous: PM_DEVICE_ACTION_RESUME/SUSPEND block
 * the calling thread until the modem is fully up or down, rather than
 * kicking off an async workqueue state machine the way modem_cellular.c
 * does. That fits this codebase: the only caller (modem_link.cpp, invoked
 * from the dedicated cellular_task thread) already exists specifically to
 * own long blocking I/O -- see CLAUDE.md's Cellular section.
 *
 * OPEN RISK: the AT command sequence below is modeled on the quectel,eg800q
 * chat scripts in modem_cellular.c (the nearest in-tree relative), not
 * verified against an EG916Q-GL AT command manual. Expect adjustment during
 * hardware bring-up -- the scripts are isolated data tables below, so fixes
 * are localized. Enable CONFIG_MODEM_MODULES_LOG_LEVEL_DBG to see the raw
 * AT exchange while debugging.
 */

#define DT_DRV_COMPAT quectel_eg916q

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/modem/chat.h>
#include <zephyr/modem/ppp.h>
#include <zephyr/modem/pipe.h>
#include <zephyr/modem/backend/uart.h>
#include <zephyr/pm/device.h>

#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(modem_quectel_eg916q, CONFIG_MODEM_LOG_LEVEL);

#define EG916Q_CHAT_BUF_SIZE  128
#define EG916Q_CHAT_ARGV_SIZE 32

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

	struct modem_ppp *ppp;
};

static bool eg916q_gpio_is_enabled(const struct gpio_dt_spec *spec)
{
	return spec->port != NULL;
}

/* ---------------------------------------------------------------------
 * Chat matches and scripts
 * ------------------------------------------------------------------ */

MODEM_CHAT_MATCH_DEFINE(eg916q_ok_match, "OK", "", NULL);
MODEM_CHAT_MATCH_DEFINE(eg916q_connect_match, "CONNECT", "", NULL);

MODEM_CHAT_MATCHES_DEFINE(eg916q_abort_matches,
			  MODEM_CHAT_MATCH("ERROR", "", NULL),
			  MODEM_CHAT_MATCH_WILDCARD("+CME ERROR: *", "", NULL),
			  MODEM_CHAT_MATCH_WILDCARD("+CMS ERROR: *", "", NULL));

/* ATE0: disable echo. AT+CMEE=1: numeric error codes (needed for the abort
 * matches above to have something concrete to match against). AT+CFUN=1:
 * full functionality. AT+CGDCONT: set the PDP context APN. AT+CGREG=1 /
 * AT+CEREG=1: enable registration status reporting (polled explicitly by
 * eg916q_wait_registered(), not via unsolicited codes -- this driver has no
 * URC handling, by design; see the header comment on synchronous PM).
 */
MODEM_CHAT_SCRIPT_CMDS_DEFINE(
	eg916q_init_chat_script_cmds, MODEM_CHAT_SCRIPT_CMD_RESP("AT", eg916q_ok_match),
	MODEM_CHAT_SCRIPT_CMD_RESP("ATE0", eg916q_ok_match),
	MODEM_CHAT_SCRIPT_CMD_RESP("AT+CMEE=1", eg916q_ok_match),
	MODEM_CHAT_SCRIPT_CMD_RESP("AT+CFUN=1", eg916q_ok_match),
	MODEM_CHAT_SCRIPT_CMD_RESP("AT+CGDCONT=1,\"IP\",\"" CONFIG_MODEM_QUECTEL_EG916Q_APN "\"",
				   eg916q_ok_match),
	MODEM_CHAT_SCRIPT_CMD_RESP("AT+CGREG=1", eg916q_ok_match),
	MODEM_CHAT_SCRIPT_CMD_RESP("AT+CEREG=1", eg916q_ok_match));

MODEM_CHAT_SCRIPT_DEFINE(eg916q_init_chat_script, eg916q_init_chat_script_cmds,
			 eg916q_abort_matches, NULL, 10);

/* Registration poll: matches only the "registered" forms of +CGREG (home or
 * roaming, URC-reporting on or off). Anything else -- "0,0" (not
 * registered), "0,2" (searching) -- matches nothing, so the script times
 * out and modem_chat_run_script() returns -EAGAIN, which is exactly the
 * retry signal eg916q_wait_registered() wants; no argv parsing needed.
 */
MODEM_CHAT_MATCHES_DEFINE(eg916q_cgreg_registered_matches,
			  MODEM_CHAT_MATCH("+CGREG: 0,1", "", NULL),
			  MODEM_CHAT_MATCH("+CGREG: 0,5", "", NULL),
			  MODEM_CHAT_MATCH("+CGREG: 1,1", "", NULL),
			  MODEM_CHAT_MATCH("+CGREG: 1,5", "", NULL));

MODEM_CHAT_SCRIPT_CMDS_DEFINE(eg916q_cgreg_poll_chat_script_cmds,
			      MODEM_CHAT_SCRIPT_CMD_RESP_MULT(
				      "AT+CGREG?", eg916q_cgreg_registered_matches));

MODEM_CHAT_SCRIPT_DEFINE(eg916q_cgreg_poll_chat_script, eg916q_cgreg_poll_chat_script_cmds,
			 eg916q_abort_matches, NULL, 3);

/* ATD*99***1#: standard 3GPP dial string to bring up PPP on PDP context 1. */
MODEM_CHAT_SCRIPT_CMDS_DEFINE(eg916q_dial_chat_script_cmds,
			      MODEM_CHAT_SCRIPT_CMD_RESP("ATD*99***1#", eg916q_connect_match));

MODEM_CHAT_SCRIPT_DEFINE(eg916q_dial_chat_script, eg916q_dial_chat_script_cmds,
			 eg916q_abort_matches, NULL, 10);

/* AT+QPOWD=1: Quectel normal power-down command. Best-effort on suspend --
 * failure is logged but not fatal, since the GPIO power pulse below is the
 * fallback that guarantees the module actually powers off either way.
 */
MODEM_CHAT_SCRIPT_CMDS_DEFINE(eg916q_shutdown_chat_script_cmds,
			      MODEM_CHAT_SCRIPT_CMD_RESP("AT+QPOWD=1", eg916q_ok_match));

MODEM_CHAT_SCRIPT_DEFINE(eg916q_shutdown_chat_script, eg916q_shutdown_chat_script_cmds,
			 eg916q_abort_matches, NULL, 5);

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

static int eg916q_wait_registered(struct eg916q_data *data)
{
	const uint32_t attempts = CONFIG_MODEM_QUECTEL_EG916Q_REGISTER_TIMEOUT_S / 2U;

	for (uint32_t i = 0; i < attempts; i++) {
		if (modem_chat_run_script(&data->chat, &eg916q_cgreg_poll_chat_script) == 0) {
			return 0;
		}

		k_sleep(K_SECONDS(2));
	}

	return -ETIMEDOUT;
}

static int eg916q_do_resume(const struct device *dev)
{
	const struct eg916q_config *config = dev->config;
	struct eg916q_data *data = dev->data;
	int err;

	err = eg916q_power_on(config);
	if (err != 0) {
		LOG_ERR("power-on failed: %d", err);
		return err;
	}

	err = modem_pipe_open(data->uart_pipe, K_SECONDS(10));
	if (err != 0) {
		LOG_ERR("failed to open UART pipe: %d", err);
		gpio_pin_set_dt(&config->power_gpio, 0);
		return err;
	}

	modem_chat_attach(&data->chat, data->uart_pipe);

	err = modem_chat_run_script(&data->chat, &eg916q_init_chat_script);
	if (err != 0) {
		LOG_ERR("init chat script failed: %d", err);
		goto err_release_chat;
	}

	err = eg916q_wait_registered(data);
	if (err != 0) {
		LOG_ERR("network registration timed out: %d", err);
		goto err_release_chat;
	}

	err = modem_chat_run_script(&data->chat, &eg916q_dial_chat_script);
	if (err != 0) {
		LOG_ERR("dial chat script failed: %d", err);
		goto err_release_chat;
	}

	modem_chat_release(&data->chat);

	err = modem_ppp_attach(data->ppp, data->uart_pipe);
	if (err != 0) {
		LOG_ERR("failed to attach PPP: %d", err);
		modem_pipe_close(data->uart_pipe, K_SECONDS(5));
		gpio_pin_set_dt(&config->power_gpio, 0);
		return err;
	}

	LOG_INF("modem attached, PPP running");
	return 0;

err_release_chat:
	modem_chat_release(&data->chat);
	modem_pipe_close(data->uart_pipe, K_SECONDS(5));
	gpio_pin_set_dt(&config->power_gpio, 0);
	return err;
}

static int eg916q_do_suspend(const struct device *dev)
{
	const struct eg916q_config *config = dev->config;
	struct eg916q_data *data = dev->data;

	modem_ppp_release(data->ppp);

	modem_chat_attach(&data->chat, data->uart_pipe);
	if (modem_chat_run_script(&data->chat, &eg916q_shutdown_chat_script) != 0) {
		LOG_WRN("AT+QPOWD failed or timed out; powering off via GPIO anyway");
	}
	modem_chat_release(&data->chat);

	modem_pipe_close(data->uart_pipe, K_SECONDS(5));
	gpio_pin_set_dt(&config->power_gpio, 0);

	LOG_INF("modem powered off");
	return 0;
}

#ifdef CONFIG_PM_DEVICE
static int eg916q_pm_action(const struct device *dev, enum pm_device_action action)
{
	switch (action) {
	case PM_DEVICE_ACTION_RESUME:
		return eg916q_do_resume(dev);
	case PM_DEVICE_ACTION_SUSPEND:
		return eg916q_do_suspend(dev);
	default:
		return -ENOTSUP;
	}
}
#endif /* CONFIG_PM_DEVICE */

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
		.unsol_matches = NULL,
		.unsol_matches_size = 0,
	};

	err = modem_chat_init(&data->chat, &chat_config);
	if (err != 0) {
		LOG_ERR("failed to init chat: %d", err);
		return err;
	}

#ifdef CONFIG_PM_DEVICE
	pm_device_init_suspended(dev);
#else
	return eg916q_do_resume(dev);
#endif

	return 0;
}

/* ---------------------------------------------------------------------
 * Instantiation
 * ------------------------------------------------------------------ */

#define EG916Q_INIT(inst)                                                                        \
	static const struct eg916q_config eg916q_config_##inst = {                                \
		.uart = DEVICE_DT_GET(DT_INST_BUS(inst)),                                          \
		.power_gpio = GPIO_DT_SPEC_INST_GET(inst, mdm_power_gpios),                        \
		.reset_gpio = GPIO_DT_SPEC_INST_GET_OR(inst, mdm_reset_gpios, {0}),                \
	};                                                                                          \
                                                                                                   \
	static struct eg916q_data eg916q_data_##inst;                                              \
                                                                                                   \
	MODEM_PPP_DEFINE(eg916q_ppp_##inst, NULL, 98, 1500, 128);                                  \
                                                                                                   \
	static int eg916q_init_##inst(const struct device *dev)                                    \
	{                                                                                           \
		eg916q_data_##inst.ppp = &eg916q_ppp_##inst;                                       \
		return eg916q_init(dev);                                                            \
	}                                                                                           \
                                                                                                   \
	PM_DEVICE_DT_INST_DEFINE(inst, eg916q_pm_action);                                          \
                                                                                                   \
	DEVICE_DT_INST_DEFINE(inst, eg916q_init_##inst, PM_DEVICE_DT_INST_GET(inst),               \
			      &eg916q_data_##inst, &eg916q_config_##inst, POST_KERNEL,             \
			      CONFIG_MODEM_QUECTEL_EG916Q_INIT_PRIORITY, NULL);

DT_INST_FOREACH_STATUS_OKAY(EG916Q_INIT)
```

- [ ] **Step 3: Wire the driver into the top-level Kconfig**

In `Kconfig` (top-level, `car_tracker/Kconfig`), add before `source "Kconfig.zephyr"`:
```kconfig
rsource "drivers/modem/Kconfig"

```

- [ ] **Step 4: Wire the driver and DTS_ROOT into the top-level CMakeLists.txt**

In `CMakeLists.txt`, add the `DTS_ROOT` append **before** `find_package(Zephyr ...)`:
```cmake
cmake_minimum_required(VERSION 3.20.0)

# Extra devicetree bindings for out-of-tree compatibles (e.g.
# quectel,eg916q) live under dts/bindings/ in this module.
list(APPEND DTS_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(car_tracker CXX)
```

Add `add_subdirectory(drivers/modem)` after the `target_sources` block that always compiles (near the top, alongside `src/services/uart_mux.cpp`'s old spot is gone — place it right after the initial always-on `target_sources(app PRIVATE ...)` call):
```cmake
target_sources(app PRIVATE
  src/main.cpp
  src/channels.cpp
  src/tasks/ble_task.cpp
)

add_subdirectory(drivers/modem)

```
(This directory's own `CMakeLists.txt` gates the actual driver source on `CONFIG_MODEM_QUECTEL_EG916Q` via `zephyr_library_sources_ifdef`, so `add_subdirectory` is safe to call unconditionally — it contributes nothing when that Kconfig option is off.)

- [ ] **Step 5: Enable the driver in prj.conf and build-verify**

In `prj.conf`, change:
```
# --- Cellular modem (u-blox SARA-R5, PPP + CMUX) ---
CONFIG_MODEM=y
CONFIG_MODEM_CELLULAR=y
CONFIG_MODEM_CELLULAR_APN="internet"   # set to your SIM carrier's APN
CONFIG_PM_DEVICE=y
```
to:
```
# --- Cellular + GNSS (Quectel EG916Q-GL; GNSS side uses uart0/gnss-nmea-
# generic above, this is the AT/PPP side on uart1) ---
CONFIG_MODEM_QUECTEL_EG916Q=y
CONFIG_MODEM_QUECTEL_EG916Q_APN="internet"   # set to your SIM carrier's APN
CONFIG_PM_DEVICE=y
```

```bash
west build -p always -b promicro_nrf52840/nrf52840 -d build-full .
```
Expected: succeeds. If it doesn't, the error is almost certainly one of:
- a missing include or macro typo in `quectel_eg916q.c` (fix and rebuild — this file was hand-verified against the real headers in this Zephyr checkout but not previously compiled);
- a Kconfig dependency this task's `select` list is missing (compare against `zephyr/drivers/modem/Kconfig.cellular`'s `select` list for `MODEM_CELLULAR`, which this task's Kconfig was modeled on).

Do not proceed to Step 6 until this build is clean.

- [ ] **Step 6: Commit**

```bash
git add -A drivers Kconfig CMakeLists.txt prj.conf
git commit -m "$(cat <<'EOF'
Add EG916Q-GL modem driver

New out-of-tree driver at drivers/modem/quectel_eg916q.c, built on
Zephyr's public modem_chat/modem_pipe/modem_ppp/modem_backend_uart
primitives (independent of CONFIG_MODEM_CELLULAR, so nothing under
zephyr/ is touched). Simpler than modem_cellular.c by design: this
board's GNSS is on its own UART, so no CMUX is needed, and PM actions
run synchronously since the only caller already owns a dedicated
blocking-I/O thread (cellular_task).

modem_link.cpp is unchanged -- it only touches the modem through
DEVICE_DT_GET(DT_ALIAS(modem)), pm_device_action_run(), and the
standard PPP net_if API, all satisfied by this driver.

AT chat script content is modeled on quectel,eg800q (nearest in-tree
relative) and not yet verified against an EG916Q-GL AT command manual;
expect adjustment during hardware bring-up.

Co-Authored-By: Claude Sonnet 5 <noreply@anthropic.com>
EOF
)"
```

---

### Task 5: Documentation and full build-matrix verification

**Files:**
- Modify: `no_gps.overlay`
- Modify: `CLAUDE.md`

- [ ] **Step 1: Update no_gps.overlay's comment**

In `no_gps.overlay`, change:
```
/*
 * Overlay for probing the NEO-6M externally over a USB-to-UART adapter.
 *
 * Disables uart0 so the nRF52840 does not configure P0.09 as a UART TX
 * output. Without this, the nRF idles TX high and would contend with the
 * adapter's TX line on the GPS RX pin (two push-pull drivers on one net).
 * The gnss child node goes away with it, which is fine -- gps_task.cpp is
 * excluded from the build via CONFIG_CAR_TRACKER_NO_GPS.
 */
```
to:
```
/*
 * Overlay for probing the EG916Q-GL's GNSS UART externally over a
 * USB-to-UART adapter.
 *
 * Disables uart0 so the nRF52840 does not configure P0.09 as a UART TX
 * output. Without this, the nRF idles TX high and would contend with the
 * adapter's TX line on the GNSS RX pin (two push-pull drivers on one net).
 * The gnss child node goes away with it, which is fine -- gps_task.cpp is
 * excluded from the build via CONFIG_CAR_TRACKER_NO_GPS.
 */
```

- [ ] **Step 2: Update CLAUDE.md's hardware description**

In `CLAUDE.md`, update the "What this is" section's board description and the "Hardware/devicetree" paragraph to describe the EG916Q-GL instead of the NEO-6M + SARA-R5 + ANNA combination, and remove the "UART scarcity is a hard constraint" section's ANNA-specific content (the two-UARTE-instance fact stays true and worth keeping; the three-peer time-sharing scheme it used to justify no longer exists).

Change:
```
Hardware/devicetree: NEO-6M GPS on `uart0` (generic NMEA driver), u-blox
SARA-R5 cellular modem on `uart1` (PPP + CMUX) — both wired up in
[app.overlay](app.overlay) with pin-selection rationale documented inline.
`aliases { gnss = &gnss; modem = &modem; }` is how `src/tasks/gps_task.cpp`
and `src/services/modem_link.cpp` look up their devices via `DEVICE_DT_GET`.

**UART scarcity is a hard constraint.** The nRF52840 has exactly two UARTE
instances (`uart0`, `uart1` — see `zephyr/dts/arm/nordic/nrf52840.dtsi`);
there is no third, and no devicetree node can invent one. Both are already
committed. A third UART peer (the ANNA BLE module on the custom cellular
board, which gates power to the modem) is served by time-sharing `uart0`:
its UARTE's pin-select registers are rewritten at runtime to point at either
the GPS pins (`default` state) or the ANNA pins (`anna` state), both declared
in `app.overlay`. [src/services/uart_mux.cpp](src/services/uart_mux.cpp)
owns that switch. Three things make it work, and all three must stay in
place: `CONFIG_PINCTRL_NON_STATIC` (selected by `CAR_TRACKER_UART_MUX` in
`Kconfig`, since it has no prompt of its own) so the driver's pinctrl config
is reachable externally; a global `PINCTRL_STATE_ANNA` compile definition in
`CMakeLists.txt`, because the *UART driver's* translation unit is what
expands it; and disabling UARTE around the switch, since PSEL registers may
only change while the peripheral is off. Only one of GPS/ANNA is reachable
at a time, and in-flight RX is lost across a switch — so route to ANNA for
the modem power-up handshake before starting the GPS task, not while NMEA is
streaming.
```
to:
```
Hardware/devicetree: a single Quectel EG916Q-GL provides both cellular and
GNSS. Its dedicated GNSS UART is on `uart0` (generic NMEA driver, same as
the NEO-6M it replaces — the module's GNSS output is plain NMEA-0183), and
its AT/PPP UART is on `uart1` (`compatible = "quectel,eg916q"`, driven by
the out-of-tree driver at [drivers/modem/quectel_eg916q.c](drivers/modem/quectel_eg916q.c)).
Both are wired up in [app.overlay](app.overlay) with pin-selection rationale
documented inline. `aliases { gnss = &gnss; modem = &modem; }` is how
`src/tasks/gps_task.cpp` and `src/services/modem_link.cpp` look up their
devices via `DEVICE_DT_GET` — neither names the EG916Q-GL directly.

**UART scarcity is a hard constraint.** The nRF52840 has exactly two UARTE
instances (`uart0`, `uart1` — see `zephyr/dts/arm/nordic/nrf52840.dtsi`);
there is no third, and no devicetree node can invent one. Both are already
committed (GNSS on `uart0`, AT/PPP on `uart1`) — there is no runtime pin
remuxing in this design; each UARTE serves exactly one peer, full time.
```

Also update the "What this is" opening paragraph's board summary if it names the NEO-6M/SARA-R5 specifically (check current wording and adjust to "GPS + cellular via a single Quectel EG916Q-GL" or similar, keeping the rest of the paragraph — board name, no-bootloader/SWD flashing details — unchanged).

- [ ] **Step 3: Full build-matrix verification**

```bash
export ZEPHYR_TOOLCHAIN_VARIANT=zephyr
export ZEPHYR_SDK_INSTALL_DIR=/Users/sankethiremath/zephyr-sdk-0.17.4
source /Users/sankethiremath/zephyrproject/.venv/bin/activate
cd /Users/sankethiremath/zephyrproject/car_tracker

west build -p always -b promicro_nrf52840/nrf52840 -d build-full .
west build -p always -b promicro_nrf52840/nrf52840/uf2 -d build-gps-only . -- -DFILE_SUFFIX=gps_only
west build -p always -b promicro_nrf52840/nrf52840 -d build-nogps . -- -DFILE_SUFFIX=nogps -DEXTRA_DTC_OVERLAY_FILE=no_gps.overlay
west build -p always -b promicro_nrf52840/nrf52840/uf2 -d build-gps-ble-test . -- -DFILE_SUFFIX=gps_ble_test
```
Expected: all four succeed.

- [ ] **Step 4: Run the design doc's acceptance-criteria greps**

```bash
grep -ri "eg916\|quectel" src/ ; echo "--- (expect: no matches) ---"
grep -ri "anna\|uart_mux\|ubx\|neo-6m" src/ Kconfig CMakeLists.txt app.overlay ; echo "--- (expect: no matches) ---"
```
Expected: both commands print nothing before their `---` marker line. If either finds a match, that's a spec-conformance gap — fix it before committing.

- [ ] **Step 5: Commit**

```bash
git add -A no_gps.overlay CLAUDE.md
git commit -m "$(cat <<'EOF'
Update docs for EG916Q-GL hardware consolidation

Co-Authored-By: Claude Sonnet 5 <noreply@anthropic.com>
EOF
)"
```
