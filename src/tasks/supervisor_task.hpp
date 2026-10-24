#pragma once

#include <cstdint>

namespace ct::supervisor {

/* Arms the watchdog and starts the supervisory thread, which feeds its own
 * channel and does nothing else -- see supervisor_task.cpp's header comment.
 * Must be called before any other task is started, because those register
 * their channels with watch() as they come up.
 */
void start();

/* Registers the calling task with the task watchdog: unless it calls feed()
 * with the returned channel within timeout_ms, the SoC is reset. Call it from
 * the task's own thread once it has reached its main loop (not while parked on
 * a start gate), and feed from the same thread. Returns the channel, or a
 * negative errno if the watchdog is disabled in this build or out of channels;
 * feed() ignores a negative channel, so callers need not special-case it.
 */
int watch(std::uint32_t timeout_ms);

/* Restarts the channel's timeout. Cheap; safe to call from any thread. */
void feed(int channel);

} /* namespace ct::supervisor */
