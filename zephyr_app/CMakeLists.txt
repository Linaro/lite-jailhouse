# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.20.0)

set(SOC_ROOT ${CMAKE_CURRENT_LIST_DIR})
set(BOARD_ROOT ${CMAKE_CURRENT_LIST_DIR})
set(BOARD zynqmp_zcu102)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(zephyr_jailhouse)

target_sources(app PRIVATE main.c)