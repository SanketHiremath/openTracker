#pragma once

#include <cstddef>
#include <cstdint>

namespace ct::fw_update {

/* Firmware update support for the A/B (direct-XIP) MCUboot layout -- see
 * ../../proMicro_bootloader/PLAN.md.
 *
 * Two slots hold two independently linked images; MCUboot runs whichever is
 * newest and valid, in place. The running app never writes its own slot, it
 * writes the *other* one and tells MCUboot to try it on the next boot.
 *
 * Nothing here is called from an ISR; all functions block on flash I/O.
 */

enum class Slot : std::uint8_t { A, B };

/* Slot this image was linked for / is executing from. */
Slot running_slot();

/* Confirms the running image to MCUboot. A freshly updated ("trial") image
 * that is not confirmed before the next reset is erased by MCUboot, which
 * then boots the previous image again. Call only once the app has shown it
 * is healthy (see main()). A no-op for an already-confirmed image, including
 * the one first flashed over SWD.
 *
 * Returns 0 on success (or nothing to do), negative errno on failure.
 */
int confirm_if_trial();

/* True if the running image is a trial that MCUboot will revert at the next
 * reset unless confirm_if_trial() succeeds.
 */
bool is_trial();

/* True between stage_begin() and stage_finish(): an image is being written to
 * the inactive slot. The power task will not sleep through this.
 */
bool is_staging();

/* --- Staging a new image into the inactive slot ---------------------------
 *
 * stage_begin() -> stage_write()* -> stage_finish(). The bytes must be the
 * *signed* image (header + payload + trailer TLVs, i.e. zephyr.signed.bin)
 * built for the inactive slot -- the slot-B variant while running from A and
 * vice versa. The inactive slot is erased progressively as data arrives.
 * stage_finish() marks the image to be tried once on the next boot; the
 * caller reboots when ready. An image that fails MCUboot's hash/ROM-address
 * checks is simply skipped and the current image keeps booting.
 */
int stage_begin();
int stage_write(const std::uint8_t *data, std::size_t len);
int stage_finish();

/* Reboots into MCUboot's USB serial recovery mode. Does not return. */
[[noreturn]] void reboot_to_recovery();

} /* namespace ct::fw_update */
