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
    static int unks_to_try[] = {
        // baseline (known working)
        0,

        // small flag-like values
        1,
        2,
        4,
        8,
        0x10,
        0xFF,
        0x100,

        // APP Scuffed - Find My: MAC 63:00:D2:84:80:DC
        0x6300D284,
        0x84D20063,
        0x0063D284,
        0x84D26300,
        0x80DC,
        0xDC80,
        0x000080DC,
        0x0000DC80,
        0x63,
        0xDC,
        0x630080DC,
        0xDC800063,

        // Ovaltine: MAC 1C:DB:26:97:A9:80
        0x1CDB2697,
        0x9726DB1C,
        0x80A9,
        0xA980,

        // junk
        0xDEADBEEF,
        0xCAFEBABE,
    };

    for (int i = 0; i < (int)(sizeof(unks_to_try) / sizeof(unks_to_try[0])); i++) {
        attempt(0, unks_to_try[i]);
    }
    LOG_DEBUG("End");

    EXIT_SYSCALL(state);
}
