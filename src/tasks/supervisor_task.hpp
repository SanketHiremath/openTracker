#pragma once

namespace ct::supervisor {

/* Starts the supervisory thread, which arms the watchdog and does nothing
 * else -- see supervisor_task.cpp's header comment. Has no dependency on
 * any other task's start order.
 */
void start();

} /* namespace ct::supervisor */
