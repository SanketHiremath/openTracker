# Boot-time peripheral self-test

## Purpose

On wake-up, verify the MCU can actually talk to both onboard peripherals
before doing anything with them:

- **GPS** (NEO-6M on uart0): confirm NMEA traffic is arriving.
- **Cellular modem** (Quectel EG916Q-GL on uart1): confirm it answers `AT`
  with `OK`.

On success, blink the onboard LED (`led0`) at 1 Hz forever as a heartbeat.
On failure, blink fast (5 Hz) forever and leave the affected task(s) parked
so they never touch hardware that failed the check.

## Scope

Applies to the `prj.conf` (full) and `prj_gps_only.conf` builds — the two
configs where the peripherals under test are actually compiled in and no
other bring-up mode is already doing something incompatible with gating.

Explicitly **not** run in:
- `prj_gps_ble_test.conf` (`CONFIG_CAR_TRACKER_GPS_BLE_TEST`) — this build
  exists specifically to see raw GPS/BLE behavior with zero extra layers,
  and it already strips the supervisor/watchdog for the same reason.
- `prj_nogps.conf` (`CONFIG_CAR_TRACKER_NO_GPS`, which also sets
  `CONFIG_CAR_TRACKER_GPS_ONLY`) — neither peripheral is under the MCU's
  control in that config (GPS handed to an external UART adapter, cellular
  compiled out entirely), so there is nothing to self-test.

## GPS check: NMEA presence, not AT/OK

The NEO-6M is an NMEA/UBX device, not an AT-command modem — it will never
answer `AT` with `OK`. The in-tree `gnss-nmea-generic` driver already
unconditionally publishes on any GGA/RMC sentence via `GNSS_DATA_CALLBACK`,
regardless of fix validity (see `gnss_nmea0183_match.c`), and this driver is
active from early boot independent of `ct::gps::start()`. So: add a one-shot
semaphore given the first time `gps_task.cpp`'s existing `gnss_data_cb`
fires, and have the self-test wait on it with a timeout. Any sentence
(fix or no-fix) counts as proof the MCU↔GPS UART link is alive.

This is a separate, single-producer/single-consumer semaphore from GPS's
existing raw-sample queue (`gps_raw_queue`) that feeds the normal fix
pipeline — no interference between the two.

## Cellular check: real AT/OK, without a full attach

The modem driver's only existing entry point (`PM_DEVICE_ACTION_RESUME`) does
far more than send `AT`: it also runs `CFUN`, `CGDCONT`, polls network
registration (up to 60s), and dials PPP (up to 10s) — failing the whole
thing if there's no SIM or no signal, which would misreport a healthy modem
as "unreachable." So add a narrow new entry point to the driver:

```c
/* drivers/modem/quectel_eg916q.h (new) */
int eg916q_selftest(const struct device *dev);
```

Implementation mirrors the existing `eg916q_do_resume`/`eg916q_do_suspend`
structure but only runs a one-command chat script (`AT` -> `OK`), then powers
the modem back off — bypassing the PM_DEVICE action framework entirely, so
the PM device state stays `SUSPENDED` throughout and a later normal
`pm_device_action_run(PM_DEVICE_ACTION_RESUME)` from `modem_link.cpp` works
exactly as before.

## Gating task loops

Two independent one-shot semaphores, owned by the new `ct::selftest`
service:

- `wait_gps_ready()` — called once by `gps_task.cpp`'s thread entry, right
  before its `for (;;)` loop (only compiled when self-test is active — see
  Scope).
- `wait_cellular_ready()` — called once by `cellular_task.cpp`'s thread
  entry, right before its `for (;;)` loop.

Both task threads are still created immediately in `main()` as today —
only the loop body is gated. This means on failure the device still boots:
the affected task thread exists but sits blocked forever on its semaphore.

`ble_task` and `supervisor_task` are **not** gated:
- BLE is an independent identification aid, already started first and
  unconditionally (see `main.cpp`'s existing comment).
- Supervisor owns the watchdog and must never be delayed by anything else,
  per `CLAUDE.md`. It keeps feeding the watchdog regardless of self-test
  outcome, so a failed self-test never bricks the device via a watchdog
  reset loop.

## New files / changes

- `src/services/selftest.hpp` / `.cpp` (new) — `run()`, `wait_gps_ready()`,
  `wait_cellular_ready()`. Owns the `led0` GPIO and a heartbeat thread
  (1 Hz on success, 5 Hz on failure, forever).
- `drivers/modem/quectel_eg916q.h` (new) + `.c` — add `eg916q_selftest()`.
- `src/tasks/gps_task.cpp` / `.hpp` — one-shot "first sample" semaphore +
  accessor; call `ct::selftest::wait_gps_ready()` before the task loop.
- `src/tasks/cellular_task.cpp` — call `ct::selftest::wait_cellular_ready()`
  before the task loop.
- `src/main.cpp` — call `ct::selftest::run()` after `ct::ble::start()`,
  guarded by `!CONFIG_CAR_TRACKER_GPS_BLE_TEST && !CONFIG_CAR_TRACKER_NO_GPS`.
- `CMakeLists.txt` — compile `selftest.cpp` under the same guard.
- `src/app_config.hpp` — new constants: GPS self-test wait timeout, LED
  heartbeat/fail-blink half-periods, self-test LED thread stack size and
  priority.

## Concrete values

- GPS self-test wait timeout: 5s (NMEA sentences arrive ~1x/sec once
  powered; well clear of that).
- Cellular self-test chat script timeout: 5s (matches the existing
  shutdown script's timeout for a single command).
- LED heartbeat (success): toggle every 500ms (1 Hz blink).
- LED fail-blink: toggle every 100ms (5 Hz blink).

## Testing

No test suite in this repo (per `CLAUDE.md`). Validate by building
`build-full` and `build-gps-only`, and flashing to hardware:
- Both peripherals healthy -> LED blinks ~1 Hz forever.
- Modem PWRKEY/uart1 disconnected -> LED blinks fast forever, cellular task
  never executes a publish, GPS still works normally (in `build-full`,
  where GPS is unaffected).
- GPS uart0 disconnected -> LED blinks fast forever, GPS task never
  publishes fixes.
