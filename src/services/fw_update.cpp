/*
 * Firmware update support for the A/B (direct-XIP) MCUboot layout.
 * See fw_update.hpp and ../../proMicro_bootloader/PLAN.md.
 */

#include <cerrno>

#include <zephyr/dfu/flash_img.h>
#include <zephyr/dfu/mcuboot.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/retention/bootmode.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/sys/reboot.h>

#include <bootutil/bootutil_public.h>

#include "../app_config.hpp"
#include "fw_update.hpp"

LOG_MODULE_REGISTER(ct_fwu, CT_LOG_LEVEL);

namespace {

constexpr int kSlotAId = FIXED_PARTITION_ID(slot0_partition);
constexpr int kSlotBId = FIXED_PARTITION_ID(slot1_partition);

/* The image is linked for exactly one slot, so which one is fixed at build
 * time: it is the partition named by zephyr,code-partition.
 */
constexpr bool kRunningFromA = (DT_REG_ADDR(DT_CHOSEN(zephyr_code_partition)) == FIXED_PARTITION_OFFSET(slot0_partition));
constexpr int kInactiveId = kRunningFromA ? kSlotBId : kSlotAId;

/* Single staging session at a time; static, no heap. */
flash_img_context stage_ctx;
bool stage_open;

} /* namespace */

namespace ct::fw_update {

Slot running_slot()
{
	return kRunningFromA ? Slot::A : Slot::B;
}

bool is_staging()
{
	return stage_open;
}

bool is_trial()
{
	return !boot_is_img_confirmed();
}

int confirm_if_trial()
{
	if (boot_is_img_confirmed()) {
		return 0;
	}

	const int err = boot_write_img_confirmed();

	if (err != 0) {
		LOG_ERR("failed to confirm image (%d); MCUboot will revert at next reset", err);
	} else {
		LOG_INF("trial image confirmed (running from slot %c)",
			kRunningFromA ? 'A' : 'B');
	}
	return err;
}

int stage_begin()
{
	const int err = flash_img_init_id(&stage_ctx, kInactiveId);

	if (err != 0) {
		LOG_ERR("cannot open inactive slot %c (%d)", kRunningFromA ? 'B' : 'A', err);
		stage_open = false;
		return err;
	}
	stage_open = true;
	return 0;
}

int stage_write(const std::uint8_t *data, std::size_t len)
{
	if (!stage_open) {
		return -EINVAL;
	}
	return flash_img_buffered_write(&stage_ctx, data, len, false);
}

int stage_finish()
{
	if (!stage_open) {
		return -EINVAL;
	}
	stage_open = false;

	/* Flush the tail of the write buffer. */
	int err = flash_img_buffered_write(&stage_ctx, nullptr, 0, true);

	if (err != 0) {
		LOG_ERR("flush of staged image failed (%d)", err);
		return err;
	}

	/* Mark the inactive slot as "boot once, then confirm or revert". */
	const struct flash_area *fa = nullptr;

	err = flash_area_open(kInactiveId, &fa);
	if (err != 0) {
		return err;
	}
	err = boot_set_next(fa, /*active=*/false, /*confirm=*/false);
	flash_area_close(fa);

	if (err != 0) {
		LOG_ERR("could not mark slot %c pending (%d)", kRunningFromA ? 'B' : 'A', err);
		return -EIO;
	}

	LOG_INF("staged %zu bytes into slot %c; will be tried on next boot",
		flash_img_bytes_written(&stage_ctx), kRunningFromA ? 'B' : 'A');
	return 0;
}

void reboot_to_recovery()
{
	LOG_WRN("rebooting into MCUboot USB serial recovery");
	bootmode_set(BOOT_MODE_TYPE_BOOTLOADER);
	sys_reboot(SYS_REBOOT_WARM);
	CODE_UNREACHABLE;
}

} /* namespace ct::fw_update */
