# Boot-time Peripheral Self-Test Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** On boot, verify the MCU can talk to the GPS (NMEA presence) and cellular modem (AT/OK) before those tasks do real work, and signal pass/fail via the onboard LED (1 Hz heartbeat on pass, 5 Hz on fail, forever).

**Architecture:** A new `ct::selftest` service runs once from `main()`, blocking, before `ct::gps::start()`/`ct::cellular::start()`. It waits on a one-shot semaphore fed by GPS's existing NMEA-parse callback, and calls a new narrow `eg916q_selftest()` entry point added to the modem driver (a stripped-down `AT`→`OK` check, distinct from the full network-attach path `PM_DEVICE_ACTION_RESUME` already does). On success it releases two per-task "ready" semaphores that `gps_task`/`cellular_task` each wait on once before entering their normal loop; on failure it releases neither, so the affected task(s) sit parked forever while the rest of the system (BLE, supervisor/watchdog) boots normally. Full rationale: `docs/superpowers/specs/2026-09-24-boot-selftest-design.md`.

**Tech Stack:** Zephyr RTOS (C/C++), `zephyr/kernel.h` semaphores/threads, `zephyr/drivers/gpio.h`, the existing out-of-tree `quectel,eg916q` modem driver and in-tree `gnss-nmea-generic` driver.

**Testing note:** This repo has no unit test framework, linter, or CI (see `CLAUDE.md`) — hardware-in-the-loop is the only ground truth. Every task below is verified by building the affected config(s) with `west build` and, for behavior that can't be checked by compilation alone, flashing to hardware and observing the LED/log over RTT, per the final task's manual test plan. There are no "write a failing test" steps for that reason — instead, each task ends with a `west build` invocation that must succeed.

---

### Task 1: Self-test tuning constants

**Files:**
- Modify: `src/app_config.hpp`

- [ ] **Step 1: Add the new constants section**

Add this new section at the end of the file, just before the closing `} /* namespace ct::config */`:

```cpp
/* ---------------------------------------------------------------------
 * Self-test (boot-time GPS/cellular liveness check)
 * ------------------------------------------------------------------ */

/* How long to wait for the first NMEA sentence from the GPS module before
 * declaring the GPS self-test check failed. Sentences arrive ~1x/sec once
 * the receiver is powered, so this is comfortably clear of that.
 */
inline constexpr k_timeout_t kSelftestGpsTimeout = K_SECONDS(5);

/* LED toggle interval on self-test success (1 Hz blink) and failure
 * (5 Hz blink). Each is a half-period: one toggle every interval gives a
 * blink at 1 / (2 * interval).
 */
inline constexpr k_timeout_t kSelftestHeartbeatHalfPeriod = K_MSEC(500);
inline constexpr k_timeout_t kSelftestFailBlinkHalfPeriod = K_MSEC(100);

inline constexpr int kSelftestLedPriority = 6;
inline constexpr std::size_t kSelftestLedStackSize = 512;
```

- [ ] **Step 2: Build to verify it compiles**

Run: `source /Users/sankethiremath/zephyrproject/.venv/bin/activate && cd /Users/sankethiremath/zephyrproject/car_tracker && west build -p always -b promicro_nrf52840/nrf52840 -d build-full .`
Expected: build succeeds (this file is header-only and unused so far, so nothing else changes).

- [ ] **Step 3: Commit**

No git repo is initialized in this directory (`car_tracker` is not currently a git repository) — skip commit steps for this plan. If a repo is initialized later, commit each task's diff separately with a message like `feat: add self-test tuning constants`.

---

### Task 2: Modem driver self-test entry point

**Files:**
- Create: `drivers/modem/quectel_eg916q.h`
- Modify: `drivers/modem/quectel_eg916q.c`

- [ ] **Step 1: Create the public header**

```c
/*
 * SPDX-License-Identifier: Apache-2.0
 *
 * Public entry points for the out-of-tree EG916Q-GL modem driver that fall
 * outside the standard Zephyr device/PM-device API (device_is_ready(),
 * pm_device_action_run()) already used by src/services/modem_link.cpp.
 */

#ifndef CAR_TRACKER_DRIVERS_MODEM_QUECTEL_EG916Q_H_
#define CAR_TRACKER_DRIVERS_MODEM_QUECTEL_EG916Q_H_

#include <zephyr/device.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * One-shot power-on + "AT"/"OK" liveness check, independent of the
 * PM_DEVICE_ACTION_RESUME path (which additionally attaches chat, polls
 * network registration for up to CONFIG_MODEM_QUECTEL_EG916Q_REGISTER_TIMEOUT_S,
 * and dials PPP) -- so a modem with no SIM or no signal isn't misreported as
 * unreachable. Powers the modem back off before returning either way, and
 * never touches the PM_DEVICE action framework, so the device's cached PM
 * state stays SUSPENDED throughout: a later normal
 * pm_device_action_run(PM_DEVICE_ACTION_RESUME) behaves exactly as if this
 * had never run.
 *
 * Returns 0 if the modem replied OK to AT, a negative errno otherwise
 * (e.g. -ETIMEDOUT if nothing replied in time).
 */
int eg916q_selftest(const struct device *dev);

#ifdef __cplusplus
}
#endif

#endif /* CAR_TRACKER_DRIVERS_MODEM_QUECTEL_EG916Q_H_ */
```

- [ ] **Step 2: Include the new header from the driver source**

In `drivers/modem/quectel_eg916q.c`, add the include alongside the existing local includes (there are none currently besides the log macro; add this right after the `#include <zephyr/pm/device.h>` line):

```c
#include <zephyr/pm/device.h>

#include "quectel_eg916q.h"
```

- [ ] **Step 3: Add the self-test chat script**

In `drivers/modem/quectel_eg916q.c`, immediately after the existing `eg916q_shutdown_chat_script` definition (the block ending `MODEM_CHAT_SCRIPT_DEFINE(eg916q_shutdown_chat_script, ...)`), add:

```c

/* AT/OK only -- see quectel_eg916q.h's eg916q_selftest() doc comment for why
 * this is a separate, narrower script than eg916q_init_chat_script.
 */
MODEM_CHAT_SCRIPT_CMDS_DEFINE(eg916q_selftest_chat_script_cmds,
			      MODEM_CHAT_SCRIPT_CMD_RESP("AT", eg916q_ok_match));

MODEM_CHAT_SCRIPT_DEFINE(eg916q_selftest_chat_script, eg916q_selftest_chat_script_cmds,
			 eg916q_abort_matches, NULL, 5);
```

- [ ] **Step 4: Add the `eg916q_selftest()` function**

In `drivers/modem/quectel_eg916q.c`, immediately after the `eg916q_do_suspend()` function (right before the `#ifdef CONFIG_PM_DEVICE` block), add:

```c

int eg916q_selftest(const struct device *dev)
{
	const struct eg916q_config *config = dev->config;
	struct eg916q_data *data = dev->data;
	int err;

	err = eg916q_power_on(config);
	if (err != 0) {
		LOG_ERR("self-test: power-on failed: %d", err);
		return err;
	}

	err = modem_pipe_open(data->uart_pipe, K_SECONDS(10));
	if (err != 0) {
		LOG_ERR("self-test: failed to open UART pipe: %d", err);
		gpio_pin_set_dt(&config->power_gpio, 0);
		return err;
	}

	modem_chat_attach(&data->chat, data->uart_pipe);

	err = modem_chat_run_script(&data->chat, &eg916q_selftest_chat_script);
	if (err != 0) {
		LOG_ERR("self-test: AT command failed: %d", err);
	} else {
		LOG_INF("self-test: modem replied OK");
	}

	modem_chat_release(&data->chat);
	modem_pipe_close(data->uart_pipe, K_SECONDS(5));
	gpio_pin_set_dt(&config->power_gpio, 0);

	return err;
}
```

- [ ] **Step 5: Build to verify it compiles**

Run: `source /Users/sankethiremath/zephyrproject/.venv/bin/activate && cd /Users/sankethiremath/zephyrproject/car_tracker && west build -p always -b promicro_nrf52840/nrf52840 -d build-full .`
Expected: build succeeds. `eg916q_selftest` is defined but not yet called from anywhere, so expect no new warnings about it being unused (it has external linkage).

---

### Task 3: GPS "first sample" self-test signal

**Files:**
- Modify: `src/tasks/gps_task.hpp`
- Modify: `src/tasks/gps_task.cpp`

- [ ] **Step 1: Add `wait_for_first_sample()` to the header**

In `src/tasks/gps_task.hpp`, add `#include <zephyr/kernel.h>` after `#pragma once`, then add this declaration inside `namespace ct::gps { ... }`, after `start()`:

```cpp
#pragma once

#include <zephyr/kernel.h>

namespace ct::gps {

/* Verifies the GNSS device and starts the acquisition thread.
 * Returns 0 on success, -ENODEV if the GNSS device is not ready.
 */
int start();

/* Blocks until the first NMEA sentence (GGA/RMC, fix or no-fix) has been
 * observed on uart0, or `timeout` elapses. Independent of start()/the
 * normal fix pipeline -- used by the boot self-test to verify the MCU can
 * talk to the GPS module before anything else touches it. Returns 0 if a
 * sentence arrived in time, -EAGAIN on timeout (see k_sem_take()).
 */
int wait_for_first_sample(k_timeout_t timeout);

/* ... standby()/wake() unchanged below ... */
```

(Only the two new lines plus the include are additions; leave `standby()`/`wake()`'s existing declarations and comments as they are.)

- [ ] **Step 2: Add the semaphore and give it from the NMEA callback**

In `src/tasks/gps_task.cpp`, inside the anonymous `namespace { ... }` block, immediately after the existing `K_MSGQ_DEFINE(gps_raw_queue, ...)` line, add:

```cpp
/* Given once, the first time gnss_data_cb below observes any NMEA sentence
 * (fix or no-fix) -- proof the MCU<->GPS UART link is alive. Consumed by
 * ct::gps::wait_for_first_sample(), which the boot self-test calls. This is
 * independent of gps_raw_queue above (which feeds the normal fix pipeline).
 */
K_SEM_DEFINE(gps_first_sample_sem, 0, 1);
```

Then, in `gnss_data_cb()`, add the give as the last line of the function body (after the existing `k_msgq_put`/overflow-handling block, still before the closing `}`):

```cpp
	if (k_msgq_put(&gps_raw_queue, &fix, K_NO_WAIT) != 0) {
		ct::GpsFix discarded{};
		(void)k_msgq_get(&gps_raw_queue, &discarded, K_NO_WAIT);
		(void)k_msgq_put(&gps_raw_queue, &fix, K_NO_WAIT);
		LOG_WRN("GPS queue full; dropped oldest sample");
	}

	/* Extra gives beyond the semaphore's limit of 1 are a documented no-op
	 * in Zephyr, so it's safe to call this on every sample, not just the
	 * first.
	 */
	k_sem_give(&gps_first_sample_sem);
}
```

- [ ] **Step 3: Implement `wait_for_first_sample()`**

In `src/tasks/gps_task.cpp`, inside `namespace ct::gps { ... }`, add the new function after `start()` and before `standby()`:

```cpp
int wait_for_first_sample(k_timeout_t timeout)
{
	return k_sem_take(&gps_first_sample_sem, timeout);
}
```

- [ ] **Step 4: Build to verify it compiles**

Run: `source /Users/sankethiremath/zephyrproject/.venv/bin/activate && cd /Users/sankethiremath/zephyrproject/car_tracker && west build -p always -b promicro_nrf52840/nrf52840 -d build-full .`
Expected: build succeeds. Also build the GPS-only and GPS+BLE-test configs since `gps_task.cpp` is shared by all three:

Run: `west build -p always -b promicro_nrf52840/nrf52840/uf2 -d build-gps-only . -- -DFILE_SUFFIX=gps_only`
Run: `west build -p always -b promicro_nrf52840/nrf52840/uf2 -d build-gps-ble-test . -- -DFILE_SUFFIX=gps_ble_test`
Expected: both succeed.

---

### Task 4: `ct::selftest` service

**Files:**
- Create: `src/services/selftest.hpp`
- Create: `src/services/selftest.cpp`

- [ ] **Step 1: Create the header**

```cpp
#pragma once

namespace ct::selftest {

/* Runs the boot-time self-test: verifies the MCU can talk to both the GPS
 * (NMEA presence on uart0 -- see ct::gps::wait_for_first_sample()) and the
 * cellular modem (AT/OK on uart1 -- see eg916q_selftest() in the modem
 * driver). Blocking; call once from main() before starting the GPS/
 * cellular tasks.
 *
 * On success: starts a 1 Hz LED heartbeat (forever) and releases both
 * wait_gps_ready() and wait_cellular_ready() below.
 *
 * On failure: logs which check(s) failed, starts a fast 5 Hz LED blink
 * (forever) instead, and releases neither -- so the corresponding task(s)
 * stay parked before doing any peripheral I/O. BLE and the supervisor
 * (watchdog) are unaffected either way; only run() by itself, before this
 * function returns.
 */
void run();

/* Blocks until run() has confirmed GPS communication is OK. Call once, at
 * the top of the GPS task's thread entry, before its main loop.
 */
void wait_gps_ready();

/* Blocks until run() has confirmed cellular modem communication is OK.
 * Call once, at the top of the cellular task's thread entry, before its
 * main loop.
 */
void wait_cellular_ready();

} /* namespace ct::selftest */
```

- [ ] **Step 2: Create the implementation**

```cpp
/*
 * Boot-time peripheral self-test.
 *
 * Verifies the MCU can talk to the GPS and cellular modules before the
 * tasks that depend on them do any real work, and gives a single visible
 * pass/fail signal via the onboard LED. See docs/superpowers/specs/
 * 2026-09-24-boot-selftest-design.md for the full rationale.
 */

#include <cstdint>

#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

#include "../app_config.hpp"
#include "../tasks/gps_task.hpp"
#include "selftest.hpp"

#if !defined(CONFIG_CAR_TRACKER_GPS_ONLY)
#include "../../drivers/modem/quectel_eg916q.h"
#endif

LOG_MODULE_REGISTER(ct_selftest, LOG_LEVEL_INF);

namespace {

const gpio_dt_spec kLed = GPIO_DT_SPEC_GET(DT_ALIAS(led0), gpios);

K_SEM_DEFINE(gps_ready_sem, 0, 1);
K_SEM_DEFINE(cellular_ready_sem, 0, 1);

K_THREAD_STACK_DEFINE(led_stack, ct::config::kSelftestLedStackSize);
k_thread led_thread_data;

void led_heartbeat_entry(void *ok_ptr, void *, void *)
{
	const bool ok = static_cast<bool>(reinterpret_cast<std::uintptr_t>(ok_ptr));
	const k_timeout_t half_period = ok ? ct::config::kSelftestHeartbeatHalfPeriod
					    : ct::config::kSelftestFailBlinkHalfPeriod;

	for (;;) {
		gpio_pin_toggle_dt(&kLed);
		k_sleep(half_period);
	}
}

} /* namespace */

namespace ct::selftest {

void run()
{
	int err = gpio_pin_configure_dt(&kLed, GPIO_OUTPUT_INACTIVE);

	if (err != 0) {
		LOG_ERR("failed to configure self-test LED gpio: %d", err);
	}

	LOG_INF("running boot self-test");

	const int gps_err = ct::gps::wait_for_first_sample(ct::config::kSelftestGpsTimeout);

	if (gps_err != 0) {
		LOG_ERR("self-test: no NMEA data from GPS (%d)", gps_err);
	} else {
		LOG_INF("self-test: GPS communication OK");
	}

#if !defined(CONFIG_CAR_TRACKER_GPS_ONLY)
	const struct device *const modem_dev = DEVICE_DT_GET(DT_ALIAS(modem));
	const int cellular_err =
		device_is_ready(modem_dev) ? eg916q_selftest(modem_dev) : -ENODEV;

	if (cellular_err != 0) {
		LOG_ERR("self-test: cellular modem not responding (%d)", cellular_err);
	} else {
		LOG_INF("self-test: cellular modem communication OK");
	}
#else
	const int cellular_err = 0;
#endif

	const bool ok = (gps_err == 0) && (cellular_err == 0);

	if (ok) {
		LOG_INF("self-test passed; releasing GPS/cellular tasks");
		k_sem_give(&gps_ready_sem);
#if !defined(CONFIG_CAR_TRACKER_GPS_ONLY)
		k_sem_give(&cellular_ready_sem);
#endif
	} else {
		LOG_ERR("self-test failed; affected task(s) will not run");
	}

	k_thread_create(&led_thread_data, led_stack, K_THREAD_STACK_SIZEOF(led_stack),
			led_heartbeat_entry, reinterpret_cast<void *>(static_cast<std::uintptr_t>(ok)),
			nullptr, nullptr, ct::config::kSelftestLedPriority, 0, K_NO_WAIT);
	k_thread_name_set(&led_thread_data, "selftest_led");
}

void wait_gps_ready()
{
	k_sem_take(&gps_ready_sem, K_FOREVER);
}

void wait_cellular_ready()
{
	k_sem_take(&cellular_ready_sem, K_FOREVER);
}

} /* namespace ct::selftest */
```

- [ ] **Step 3: Register the new source file in the build (needed before it will compile as part of the app)**

In `CMakeLists.txt`, add this block after the existing cellular-telemetry `if(NOT CONFIG_CAR_TRACKER_GPS_ONLY)` block (i.e. at the end of the file):

```cmake

# Boot-time self-test (GPS NMEA presence + cellular AT/OK) gates the GPS and
# cellular task loops behind an onboard-LED heartbeat. Skipped for the
# GPS+BLE bring-up test (wants zero extra layers ahead of GPS/BLE) and the
# no-GPS build (neither peripheral is under the MCU's control there: GPS is
# handed to an external UART adapter, cellular is compiled out).
if(NOT CONFIG_CAR_TRACKER_GPS_BLE_TEST AND NOT CONFIG_CAR_TRACKER_NO_GPS)
  target_sources(app PRIVATE src/services/selftest.cpp)
endif()
```

- [ ] **Step 4: Build to verify it compiles**

Run: `source /Users/sankethiremath/zephyrproject/.venv/bin/activate && cd /Users/sankethiremath/zephyrproject/car_tracker && west build -p always -b promicro_nrf52840/nrf52840 -d build-full .`
Expected: build succeeds. (Nothing calls `ct::selftest::run()`/`wait_*_ready()` yet, so this only proves the new files compile standalone; the linker won't complain about unused `void`-returning functions.)

Also run: `west build -p always -b promicro_nrf52840/nrf52840/uf2 -d build-gps-only . -- -DFILE_SUFFIX=gps_only`
Expected: succeeds (selftest.cpp is compiled here too, with the `CONFIG_CAR_TRACKER_GPS_ONLY` branch taken).

---

### Task 5: Wire the self-test into GPS and cellular task loops

**Files:**
- Modify: `src/tasks/gps_task.cpp`
- Modify: `src/tasks/cellular_task.cpp`

- [ ] **Step 1: Gate the GPS task loop**

In `src/tasks/gps_task.cpp`, add this include near the top, grouped with the other local includes (`#include "gps_task.hpp"` etc.), guarded so it's only pulled in for the two configs where self-test runs:

```cpp
#if !defined(CONFIG_CAR_TRACKER_GPS_BLE_TEST)
#include "../services/selftest.hpp"
#endif
```

Then, in `gps_thread_entry()`, add the wait call right after the startup log line and before the `for (;;)` loop:

```cpp
void gps_thread_entry(void *, void *, void *)
{
	LOG_INF("GPS task started");

#if !defined(CONFIG_CAR_TRACKER_GPS_BLE_TEST)
	ct::selftest::wait_gps_ready();
#endif

	for (;;) {
		ct::GpsFix fix{};
		/* ... unchanged ... */
```

- [ ] **Step 2: Gate the cellular task loop**

In `src/tasks/cellular_task.cpp`, add the include alongside the existing local includes:

```cpp
#include "../services/mqtt_publisher.hpp"
#include "../services/selftest.hpp"
#include "cellular_task.hpp"
```

Then, in `cellular_thread_entry()`, add the wait call right after the startup log line and before the `for (;;)` loop:

```cpp
void cellular_thread_entry(void *, void *, void *)
{
	LOG_INF("cellular task started");

	ct::selftest::wait_cellular_ready();

	for (;;) {
		const zbus_channel *chan = nullptr;
		/* ... unchanged ... */
```

(No `#if` guard needed here: `cellular_task.cpp` is only ever compiled when `!CONFIG_CAR_TRACKER_GPS_ONLY`, which is exactly the condition under which `selftest.cpp` is also compiled -- see Task 4's CMakeLists.txt guard.)

- [ ] **Step 3: Build to verify it compiles**

Run each of:
```bash
source /Users/sankethiremath/zephyrproject/.venv/bin/activate
cd /Users/sankethiremath/zephyrproject/car_tracker
west build -p always -b promicro_nrf52840/nrf52840 -d build-full .
west build -p always -b promicro_nrf52840/nrf52840/uf2 -d build-gps-only . -- -DFILE_SUFFIX=gps_only
west build -p always -b promicro_nrf52840/nrf52840/uf2 -d build-gps-ble-test . -- -DFILE_SUFFIX=gps_ble_test
west build -p always -b promicro_nrf52840/nrf52840 -d build-nogps . -- -DFILE_SUFFIX=nogps -DEXTRA_DTC_OVERLAY_FILE=no_gps.overlay
```
Expected: all four succeed. (`build-full`/`build-gps-only` now actually call into self-test; `build-gps-ble-test`/`build-nogps` should be completely unaffected since neither compiles `selftest.cpp` nor calls into it.)

---

### Task 6: Start the self-test from `main()`

**Files:**
- Modify: `src/main.cpp`

- [ ] **Step 1: Add the guarded include**

In `src/main.cpp`, add this after the existing `#include "tasks/supervisor_task.hpp"` line (still above the `#if !defined(CONFIG_CAR_TRACKER_NO_GPS)` block that includes `gps_task.hpp`):

```cpp
#include "tasks/ble_task.hpp"
#include "tasks/supervisor_task.hpp"

#if !defined(CONFIG_CAR_TRACKER_GPS_BLE_TEST) && !defined(CONFIG_CAR_TRACKER_NO_GPS)
#include "services/selftest.hpp"
#endif

#if !defined(CONFIG_CAR_TRACKER_NO_GPS)
#include "tasks/gps_task.hpp"
#endif
```

- [ ] **Step 2: Call `run()` before starting GPS/cellular**

In `main()`, add the call right after the existing BLE-start block and before the `#if defined(CONFIG_CAR_TRACKER_NO_GPS)` block:

```cpp
	if (const int err = ct::ble::start(); err != 0) {
		LOG_WRN("BLE unavailable (%d); continuing", err);
	}

#if !defined(CONFIG_CAR_TRACKER_GPS_BLE_TEST) && !defined(CONFIG_CAR_TRACKER_NO_GPS)
	/* Blocks until the GPS/cellular liveness check finishes; releases
	 * their task loops (or not, on failure -- see selftest.cpp). Must run
	 * before ct::gps::start()/ct::cellular::start() below, since their
	 * task loops wait on the gate this sets up.
	 */
	ct::selftest::run();
#endif

#if defined(CONFIG_CAR_TRACKER_NO_GPS)
	/* ... unchanged ... */
```

- [ ] **Step 3: Update the header comment**

In the file's top header comment block, update the paragraph describing `CONFIG_CAR_TRACKER_GPS_BLE_TEST` to mention the new self-test skip, so a reader of the architecture summary isn't surprised by the `#if` in `main()`. Change:

```
 * CONFIG_CAR_TRACKER_GPS_BLE_TEST goes further (implies GPS_ONLY): it also
 * compiles out the supervisor task, leaving just GPS + BLE. Fixes are pushed
 * to a connected phone over a BLE GATT notify characteristic (see
 * ble_task.cpp) instead of/alongside the log, to verify the onboard NEO-6M
 * wiring on uart0 without an external USB-to-UART adapter.
 */
```

to:

```
 * CONFIG_CAR_TRACKER_GPS_BLE_TEST goes further (implies GPS_ONLY): it also
 * compiles out the supervisor task, leaving just GPS + BLE. Fixes are pushed
 * to a connected phone over a BLE GATT notify characteristic (see
 * ble_task.cpp) instead of/alongside the log, to verify the onboard NEO-6M
 * wiring on uart0 without an external USB-to-UART adapter.
 *
 * A boot-time self-test (src/services/selftest.cpp) verifies the MCU can
 * talk to the GPS (NMEA presence) and cellular modem (AT/OK) before their
 * tasks do real work, signaling pass/fail on the onboard LED. It is skipped
 * for CONFIG_CAR_TRACKER_GPS_BLE_TEST (wants zero extra layers) and
 * CONFIG_CAR_TRACKER_NO_GPS (neither peripheral is under the MCU's control
 * in that build).
 */
```

- [ ] **Step 4: Build all four configs**

Run each of:
```bash
source /Users/sankethiremath/zephyrproject/.venv/bin/activate
cd /Users/sankethiremath/zephyrproject/car_tracker
west build -p always -b promicro_nrf52840/nrf52840 -d build-full .
west build -p always -b promicro_nrf52840/nrf52840/uf2 -d build-gps-only . -- -DFILE_SUFFIX=gps_only
west build -p always -b promicro_nrf52840/nrf52840/uf2 -d build-gps-ble-test . -- -DFILE_SUFFIX=gps_ble_test
west build -p always -b promicro_nrf52840/nrf52840 -d build-nogps . -- -DFILE_SUFFIX=nogps -DEXTRA_DTC_OVERLAY_FILE=no_gps.overlay
```
Expected: all four succeed with no warnings about unused functions/includes.

---

### Task 7: Flash and manually verify on hardware

**Files:** none (hardware verification only)

- [ ] **Step 1: Flash `build-full`**

```bash
cat > /tmp/flash.jlink <<'EOF'
r
h
loadfile /Users/sankethiremath/zephyrproject/car_tracker/build-full/zephyr/zephyr.hex
r
g
qc
EOF
/Applications/SEGGER/JLink_V956/JLinkExe -nogui 1 -if swd -speed 4000 -device nRF52840_xxAA -CommanderScript /tmp/flash.jlink
```
Expected: `O.K.` after the flash download, exit code 0.

- [ ] **Step 2: Verify the success path**

With both the GPS module and cellular modem correctly wired, open an RTT viewer (`JLinkRTTClient`) and power-cycle or reset the board. Confirm in the log:
```
running boot self-test
self-test: GPS communication OK
self-test: modem replied OK
self-test: cellular modem communication OK
self-test passed; releasing GPS/cellular tasks
GPS task started
cellular task started
```
Confirm the onboard red LED blinks at approximately 1 Hz (on/off every ~500ms) continuously afterward.

- [ ] **Step 3: Verify the GPS-failure path**

Disconnect the NEO-6M's TX line from the nRF (or power it off) and reset. Confirm in the log:
```
running boot self-test
self-test: no NMEA data from GPS (-11)
```
(`-11` is `-EAGAIN` from the timed-out `k_sem_take()`.) Confirm the LED blinks fast (~5 Hz) continuously, and that no `"fix:"`/`"no fix"` GPS log lines ever appear (the GPS task loop never runs). Reconnect the GPS module afterward.

- [ ] **Step 4: Verify the cellular-failure path**

Disconnect the modem's PWRKEY or UART wiring and reset. Confirm in the log:
```
self-test: GPS communication OK
self-test: cellular modem not responding (<nonzero>)
```
and the LED blinks fast continuously, with no `"cellular task started"`... actually `"cellular task started"` *does* print (it's logged before the gate), but confirm no `"publish #"` log lines ever appear even if a publish is later requested (the cellular task loop is parked before reaching `zbus_sub_wait`). Reconnect the modem afterward.

- [ ] **Step 5: Re-flash the known-good build**

Repeat Step 1 once both modules are reconnected, to leave the board in the normal working state.
