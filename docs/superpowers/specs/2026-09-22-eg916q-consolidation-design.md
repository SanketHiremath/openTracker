# Design: consolidate GPS + cellular onto a Quectel EG916Q-GL

Date: 2026-09-22
Status: approved for planning

## Problem

The tracker currently uses three separate UART peers on an nRF52840 that has
only two UARTE instances:

- u-blox NEO-6M GNSS receiver on `uart0` (NMEA)
- u-blox SARA-R5 cellular modem on `uart1` (AT + PPP + CMUX)
- an ANNA BLE module that gates power to the SARA-R5, time-sharing `uart0`
  with the GNSS by rewriting UARTE PSEL registers at runtime
  (`src/services/uart_mux.cpp`)

The hardware is changing to a single Quectel EG916Q-GL that provides both LTE
and GNSS. The EG916Q-GL exposes a dedicated GNSS UART that streams standard
NMEA-0183, plus a separate AT/PPP UART.

This collapses three UART peers into two, which exactly matches the two UARTE
instances available. The runtime pin-muxing that existed solely to solve the
three-peers-on-two-UARTEs problem is no longer needed.

## Goals

1. Drive the EG916Q-GL's GNSS UART and AT/PPP UART from `uart0` and `uart1`.
2. Keep application-layer code (`src/`) free of any EG916Q-specific symbol, so
   a future modem change is a devicetree edit rather than an application edit.
3. Delete the code paths made dead by the hardware change rather than leaving
   them compiled-out or shimmed.
4. Keep all four build configurations (`prj.conf`, `prj_gps_only.conf`,
   `prj_nogps.conf`, `prj_gps_ble_test.conf`) building.

## Non-goals

- GNSS power management (standby/wake). See "Removed capabilities".
- TLS for MQTT. Still a separate future milestone.
- Deduplicating the four standalone `prj*.conf` files. See "Deferred".
- Any change to the zbus channel topology, message structs, task boundaries,
  or the supervisor's policy.

## Architecture

### Physical mapping

```
                 +---------------------------+
                 |     Quectel EG916Q-GL     |
                 |                           |
   uart0  <----->| GNSS UART   (NMEA-0183)   |
   uart1  <----->| AT/PPP UART (AT + PPP)    |
   GPIOs  ------>| PWRKEY, RESET_N           |
                 +---------------------------+
```

`uart0` is GNSS-only, full time. No runtime pin remux. `uart1` is the modem's
AT/PPP port, unchanged in role from the SARA-R5.

### Software layering

The central architectural decision is that the EG916Q-GL driver is a **Zephyr
driver**, not an application service. It lives at `drivers/modem/` in this
module — the same relative path Zephyr itself uses — and is reachable only
through standard Zephyr interfaces.

```
  src/tasks/cellular_task.cpp        application: execution, no policy
           |
  src/services/modem_link.cpp        app-level wrapper -- UNCHANGED
           |
           |  DEVICE_DT_GET(DT_ALIAS(modem))
           |  pm_device_action_run(RESUME / SUSPEND)
           |  net_if_up / net_if_down on the PPP L2 iface
           v
  drivers/modem/quectel_eg916q.c     devicetree-bound driver (new)
           |
           |  zephyr/modem/{chat,pipe,ppp,cmux}.h
           |  zephyr/modem/backend/uart.h
           v
  uart1
```

The three API calls in the middle box are the entire contract between the
application and the modem. They are all standard Zephyr APIs, none of them
name a vendor or a part number. This is why `modem_link.cpp` needs no change
at all, and why the next modem swap is a devicetree edit.

The GNSS path is unchanged in both structure and code:

```
  src/tasks/gps_task.cpp
           |  DEVICE_DT_GET(DT_ALIAS(gnss)), gnss_set_data_callback()
           v
  zephyr gnss_nmea_generic driver  (in-tree, compatible = "gnss-nmea-generic")
           |
           v
  uart0
```

`gps_task.cpp` already talks to the GNSS only through the `gnss` alias and the
standard GNSS API, so swapping the physical receiver costs it zero changes
apart from the removal described below. No new GNSS abstraction layer is
introduced; adding one would be speculation with no second implementation to
justify it.

### Why a new out-of-tree driver

Zephyr's generic `modem_cellular.c` dispatches on devicetree compatible with
hard-coded per-model chat scripts. The in-tree list covers `quectel,bg9x`,
`quectel,bg95`, `quectel,bg96`, `quectel,eg25-g`, `quectel,eg800q`,
`u-blox,sara-r4/r5`, `u-blox,lara-r6`, and others. There is no EG916Q-GL entry.

Adding one there means editing `zephyr/drivers/modem/modem_cellular.c`, which
sits outside this module and is managed by the west manifest — those edits are
lost on `west update` unless carried as a fork or a patch series.

The reusable primitives that `modem_cellular.c` is itself built from are
public and independent of it:

| Header | Kconfig | Independent of `MODEM_CELLULAR`? |
| --- | --- | --- |
| `zephyr/modem/chat.h` | `MODEM_CHAT` | yes |
| `zephyr/modem/pipe.h` | `MODEM_PIPE` | yes |
| `zephyr/modem/ppp.h` | `MODEM_PPP` | yes |
| `zephyr/modem/cmux.h` | `MODEM_CMUX` | yes |
| `zephyr/modem/backend/uart.h` | `MODEM_BACKEND_UART` | yes |

All five are declared under `subsys/modem/Kconfig` beneath `MODEM_MODULES`,
with no dependency on `MODEM_CELLULAR`. The new driver builds on these, so
nothing inside `zephyr/` is modified.

### Driver internal structure

The driver follows `modem_cellular.c`'s own proven separation, which is what
makes it extensible to sibling parts:

1. **Chat scripts as data.** AT sequences are
   `MODEM_CHAT_SCRIPT_CMDS_DEFINE` / `MODEM_CHAT_SCRIPT_DEFINE` tables
   (init, dial, periodic), containing no logic.
2. **Timings as macro parameters.** Power-on hold, reset pulse, startup and
   shutdown timeouts are arguments to a single instance-definition macro,
   mirroring `MODEM_CELLULAR_DEFINE_INSTANCE(inst, power_ms, reset_ms,
   startup_ms, shutdown_ms, ...)`.
3. **One state machine.** The pipe -> chat -> PPP bring-up sequence is shared
   logic that reads the tables and timings.

Consequence: supporting another Quectel part later is a new script table plus
a new timing row, not new control flow.

4. **Devicetree is the single source of truth for hardware facts.** PWRKEY and
   RESET_N are `mdm-power-gpios` / `mdm-reset-gpios` phandles resolved with
   `GPIO_DT_SPEC_INST_GET`; the UART is `DEVICE_DT_GET(DT_INST_BUS(...))`.
   No pin numbers or peripheral addresses appear as `#define`s in C.

### Devicetree binding

New binding `dts/bindings/modem/quectel,eg916q.yaml` in this module,
modeled on `quectel,eg800q.yaml`:

```yaml
description: Quectel EG916Q-GL modem
compatible: "quectel,eg916q"
include: uart-device.yaml
properties:
  mdm-power-gpios:
    type: phandle-array
    required: true
  mdm-reset-gpios:
    type: phandle-array
```

Discovered by the build via `DTS_ROOT` appended in `CMakeLists.txt` before
`find_package(Zephyr)`. The driver source is pulled in with
`add_subdirectory(drivers/modem)` using `zephyr_library()`, and its Kconfig
with `rsource "drivers/modem/Kconfig"` from the module's root `Kconfig`.

This keeps the module an ordinary Zephyr *application* (no `zephyr/module.yml`,
no west manifest change) while using the upstream directory layout, so
promoting it to a real Zephyr module later is a metadata addition, not a file
move.

## Removed capabilities

### ANNA BLE power gate and `uart_mux`

Deleted outright: `src/services/uart_mux.{cpp,hpp}`, the `uart0_anna` pinctrl
state and `pinctrl-2`/third `pinctrl-names` entry in `app.overlay`, the
`PINCTRL_STATE_ANNA` compile definition in `CMakeLists.txt`, and the
`CAR_TRACKER_UART_MUX` Kconfig symbol (which existed only to `select
PINCTRL_NON_STATIC`).

`uart_mux` has no call sites anywhere in `src/` today — the power-up handshake
that would have used it was never implemented — so removal touches no task.

### GNSS standby / wake

`ct::gps::standby()` and `ct::gps::wake()` send raw u-blox UBX-RXM-PMREQ
frames on the GNSS UART via `uart_poll_out()`. UBX is u-blox-proprietary and
means nothing to a Quectel GNSS engine, which is controlled by `AT+QGPS` /
`AT+QGPSEND` on the module's AT port instead.

Reimplementing over the AT port would create a new GPS-task-to-modem-driver
dependency that does not exist today, for a capability no production code path
uses — `standby()`/`wake()` are called only from the GPS+BLE bring-up test's
GATT command characteristic.

Deleted: `standby()`, `wake()`, the UBX helpers (`build_pmreq_backup_payload`,
`ubx_send`, the UBX constants), the `kGnssUartDev` handle, the
`kGpsBackupMaxDurationMs` constant in `app_config.hpp`, and the
`gps_cmd_write` characteristic plus its UUID in `ble_task.cpp`.

The bring-up test's fix-notify characteristic is unaffected and stays.

If GNSS power saving is wanted later, it should be designed as a proper
supervisor-owned policy expressed over a zbus channel and executed by the
cellular task (which owns the AT port), consistent with the existing
policy/execution split.

## Changes by file

| File | Change |
| --- | --- |
| `app.overlay` | `modem` node -> `compatible = "quectel,eg916q"` with power/reset GPIOs; delete `uart0_anna` pinctrl state and uart0's `pinctrl-2`/third pinctrl name; rewrite header comment for the single-module hardware; `gnss` node and both aliases unchanged |
| `Kconfig` | Delete `CAR_TRACKER_UART_MUX`; add `rsource "drivers/modem/Kconfig"` |
| `CMakeLists.txt` | Remove `src/services/uart_mux.cpp` and the `PINCTRL_STATE_ANNA` definition; append `DTS_ROOT`; `add_subdirectory(drivers/modem)` |
| `drivers/modem/quectel_eg916q.c` (new) | Driver: chat script tables, timing-parameterized instance macro, pipe/chat/PPP state machine, PM actions |
| `drivers/modem/CMakeLists.txt` (new) | `zephyr_library()` gated on `CONFIG_MODEM_QUECTEL_EG916Q` |
| `drivers/modem/Kconfig` (new) | `MODEM_QUECTEL_EG916Q`, selecting `MODEM_CHAT`/`MODEM_PPP`/`MODEM_CMUX`/`MODEM_BACKEND_UART` |
| `dts/bindings/modem/quectel,eg916q.yaml` (new) | Devicetree binding |
| `src/tasks/gps_task.cpp` | Delete UBX code, `standby()`, `wake()`, `kGnssUartDev`, the now-unused `drivers/uart.h` include; update header comment (NEO-6M -> EG916Q-GL GNSS) |
| `src/tasks/gps_task.hpp` | Delete `standby()`/`wake()` declarations and their comments |
| `src/tasks/ble_task.cpp` | Delete `gps_cmd_write`, `kGpsCmdCharUuid`, the cmd characteristic in `BT_GATT_SERVICE_DEFINE`, the `gps_task.hpp` include, and the now-stale attrs-index comment |
| `src/app_config.hpp` | Delete `kGpsBackupMaxDurationMs` |
| `src/main.cpp` | Header comment only: NEO-6M -> EG916Q-GL |
| `src/services/uart_mux.{cpp,hpp}` | Deleted |
| `prj.conf` | `CONFIG_MODEM_CELLULAR` -> `CONFIG_MODEM_QUECTEL_EG916Q`; `CONFIG_MODEM_CELLULAR_APN="internet"` -> `CONFIG_MODEM_QUECTEL_EG916Q_APN="internet"` (defined in the new `drivers/modem/Kconfig`); drop `CONFIG_PINCTRL_NON_STATIC` if present; update section comments |
| `prj_gps_only.conf`, `prj_nogps.conf`, `prj_gps_ble_test.conf` | Drop `CONFIG_PINCTRL_NON_STATIC` and any ANNA-related options; update comments. These three compile no cellular code, so no modem Kconfig is added |
| `no_gps.overlay` | Verify it still only disables `uart0`; adjust comment if it references the mux |
| `CLAUDE.md` | Update hardware description, build-config notes, and the "UART scarcity" section: the two-UARTE limit is still a real constraint and stays documented, but the three-peer time-sharing scheme and its three enabling mechanisms are gone |

## Acceptance criteria

1. All four configurations build clean:
   - `west build -p always -b promicro_nrf52840/nrf52840 -d build-full .`
   - `west build -p always -b promicro_nrf52840/nrf52840/uf2 -d build-gps-only . -- -DFILE_SUFFIX=gps_only`
   - `west build -p always -b promicro_nrf52840/nrf52840 -d build-nogps . -- -DFILE_SUFFIX=nogps -DEXTRA_DTC_OVERLAY_FILE=no_gps.overlay`
   - `west build -p always -b promicro_nrf52840/nrf52840/uf2 -d build-gps-ble-test . -- -DFILE_SUFFIX=gps_ble_test`
2. `grep -ri "eg916\|quectel" src/` returns nothing. The application layer
   names no vendor or part.
3. `src/services/modem_link.cpp` is not edited at all. If the implementation
   finds it needs a change, the driver is failing to honor the standard PM /
   `net_if` contract and the driver should be fixed instead.
4. `grep -ri "anna\|uart_mux\|ubx\|neo-6m" src/ Kconfig CMakeLists.txt app.overlay`
   returns nothing except intentional historical notes in `CLAUDE.md`.
5. Nothing under `/Users/sankethiremath/zephyrproject/zephyr/` is modified.
6. The generated devicetree binds the `modem` alias to the new compatible:
   confirm `quectel_eg916q` appears in `build-full/zephyr/include/generated/`
   devicetree headers and that the driver's instance is defined.

Hardware validation (bench, after flashing) is a separate step, gated on the
open risk below.

## Open risk

The AT chat script content — exact init sequence, registration polling,
response matchers, and timings — is modeled on `quectel,eg800q` as the nearest
published relative in the Zephyr tree. It is **not** verified against an
EG916Q-GL AT command manual, which was not available when this was written.

Expect the init script to need adjustment on first bench bring-up. Mitigation:
the scripts are isolated data tables (see "Driver internal structure"), so
corrections are localized edits with no structural impact. Enabling
`CONFIG_MODEM_MODULES_LOG_LEVEL_DBG` and `CONFIG_MODEM_CHAT_LOG_BUFFER_SIZE`
during bring-up will show the actual AT exchange.

Validating the script against the real AT manual before the first flash would
retire this risk cheaply.

## Deferred

- **`prj*.conf` deduplication.** This change edits all four standalone config
  files, which is precisely the duplication cost they incur. A shared
  `common.conf` fragment would remove it, but the standalone-ness is a
  deliberate, documented decision in `CLAUDE.md`, and changing it is unrelated
  to this hardware swap. Worth revisiting on its own.
- **GNSS power management** over `AT+QGPS`/`AT+QGPSEND`, as supervisor policy.
- **TLS for MQTT** (pre-existing milestone, unaffected).
