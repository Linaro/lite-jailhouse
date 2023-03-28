/*
 * Copyright 2023 Linaro.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <zephyr/device.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/sys_io.h>

#define XUARTPS_BASE_REG    0xFF010000
#define XUARTPS_SR_OFFSET	0x002CU
#define XUARTPS_SR_TXEMPTY	0x00000008U
#define XUARTPS_FIFO_OFFSET	0x0030U

int arch_printk_char_out(int c)
{
	uintptr_t reg_base = XUARTPS_BASE_REG;
	uint32_t reg_val;

	/* wait for transmitter to ready to accept a character */
	do {
		reg_val = sys_read32(reg_base + XUARTPS_SR_OFFSET);
	} while ((reg_val & XUARTPS_SR_TXEMPTY) == 0);

	sys_write32((uint32_t)(c & 0xFF), reg_base + XUARTPS_FIFO_OFFSET);

	do {
		reg_val = sys_read32(reg_base + XUARTPS_SR_OFFSET);
	} while ((reg_val & XUARTPS_SR_TXEMPTY) == 0);

	return 0;
}
