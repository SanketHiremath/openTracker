#pragma once

namespace ct::ble {

/* Enables the Bluetooth stack, starts advertising, and launches the status
 * thread. Returns 0 on success, negative errno otherwise.
 *
 * BLE is an independent identification aid: a failure here is reported but
 * must not prevent the tracking/telemetry path from running.
 */
int start();

} /* namespace ct::ble */
