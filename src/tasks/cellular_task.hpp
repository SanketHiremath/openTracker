#pragma once

namespace ct::cellular {

/* Starts the cellular worker thread. It then ticks on its own publish
 * schedule (see cellular_task.cpp's PublishScheduler), so this is cheap and
 * cannot fail.
 */
void start();

} /* namespace ct::cellular */
