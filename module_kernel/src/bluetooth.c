/*
This file is part of vitaQmBluetooth.
Copyright © 2026 Robpol86

This program is free software: you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation, version 3 of the License.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
this program. If not, see <https://www.gnu.org/licenses/>.
*/

/******************************************************************************
 * @file
 * @brief Interface with the bluetooth subsystem.
 ******************************************************************************/

#include <psp2kern/bt.h>
#include <psp2kern/kernel/cpu.h>
#include <psp2kern/kernel/modulemgr.h>
#include <psp2kern/kernel/sysmem.h>
#include <psp2kern/kernel/threadmgr.h>

#include "log.h"

static unsigned char device_info_buf[0x400];

static void attempt(int device, int unk) {
    for (int i = 0; i < (int)sizeof(device_info_buf); i++) device_info_buf[i] = 0;
    SceBtRegisteredInfo* device_info = (SceBtRegisteredInfo*)device_info_buf;
    LOG_DEBUG("  in: device=%d unk=%d name=\"%s\"", device, unk, device_info->name);

    int ret = ksceBtGetRegisteredInfo(device, unk, device_info, sizeof(device_info_buf));
    LOG_DEBUG("out1: device=%d unk=%d name=\"%s\" ret=%d", device, unk, device_info->name, ret);

    for (int row = 0; row < 0x100; row += 16) {
        LOG_DEBUG("out2: buf[0x%03X]: %02X %02X %02X %02X %02X %02X %02X %02X  %02X %02X %02X %02X %02X %02X %02X %02X",
                  row, device_info_buf[row + 0], device_info_buf[row + 1], device_info_buf[row + 2],
                  device_info_buf[row + 3], device_info_buf[row + 4], device_info_buf[row + 5], device_info_buf[row + 6],
                  device_info_buf[row + 7], device_info_buf[row + 8], device_info_buf[row + 9], device_info_buf[row + 10],
                  device_info_buf[row + 11], device_info_buf[row + 12], device_info_buf[row + 13],
                  device_info_buf[row + 14], device_info_buf[row + 15]);
    }

    ksceKernelDelayThread(50000);
}

void log_paired_devices(void) {
    uint32_t state;
    ENTER_SYSCALL(state);

    LOG_DEBUG("Start");
    static int devices_to_try[] = {
        // small slot indices (already tested 0-7, but redo with zero-init)
        0,
        1,
        2,
        3,
        4,
        5,
        6,
        7,
        // larger powers/edges
        8,
        9,
        10,
        15,
        16,
        31,
        32,
        63,
        64,
        // common boundaries
        100,
        127,
        128,
        255,
        256,
        65535,
        // signed-vs-unsigned probes
        -1,
        0x80000000,
        0xFFFFFFFF,
        // bigger magnitudes
        0x100,
        0x1000,
        0x10000,
    };
    for (int i = 0; i < (int)(sizeof(devices_to_try) / sizeof(devices_to_try[0])); i++) {
        attempt(devices_to_try[i], 0);
    }
    LOG_DEBUG("End");

    EXIT_SYSCALL(state);
}
