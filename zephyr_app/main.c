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
	printk("Hello Jailhouse, I'm: %s \n", CONFIG_BOARD);
}
