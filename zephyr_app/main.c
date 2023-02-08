/*
 * Copyright 2023 Linaro.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <zephyr/device.h>
#include <zephyr/kernel.h>
#include <string.h>

void main(void)
{
	while(1) {
		printk("[%d]: Hello Jailhouse, I'm: %s \n", k_uptime_get_32(),CONFIG_BOARD);
		k_msleep(500);
	}
}
