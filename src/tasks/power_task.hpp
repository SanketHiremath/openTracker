#pragma once

namespace ct::power {

/* Starts the idle-sleep task. Only compiled and called when
 * CONFIG_CAR_TRACKER_IDLE_SLEEP is on. Never fails.
 */
void start();

} /* namespace ct::power */
