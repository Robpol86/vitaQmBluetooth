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

static void attempt(int device, int unk) {
    LOG_DEBUG("  in: device=%d unk=%d", device, unk);

    static unsigned char device_info_buf[0x400];
    SceBtRegisteredInfo* device_info = (SceBtRegisteredInfo*)device_info_buf;
    int ret = ksceBtGetRegisteredInfo(device, unk, device_info, sizeof(device_info_buf));

    LOG_DEBUG("out1: device=%d unk=%d ret=%d name=\"%s\"", device, unk, ret, device_info->name);
    ksceKernelDelayThread(50000);

    for (int row = 0; row < 0x100; row += 16) {
        LOG_DEBUG("out2: buf[0x%03X]: %02X %02X %02X %02X %02X %02X %02X %02X  %02X %02X %02X %02X %02X %02X %02X %02X", row,
                  device_info_buf[row + 0], device_info_buf[row + 1], device_info_buf[row + 2], device_info_buf[row + 3],
                  device_info_buf[row + 4], device_info_buf[row + 5], device_info_buf[row + 6], device_info_buf[row + 7],
                  device_info_buf[row + 8], device_info_buf[row + 9], device_info_buf[row + 10],
                  device_info_buf[row + 11], device_info_buf[row + 12], device_info_buf[row + 13],
                  device_info_buf[row + 14], device_info_buf[row + 15]);
    }
    ksceKernelDelayThread(50000);
}

void log_paired_devices(void) {
    uint32_t state;
    ENTER_SYSCALL(state);

    LOG_DEBUG("Start");
    attempt(0, 0);
    attempt(1, 0);
    LOG_DEBUG("End");

    EXIT_SYSCALL(state);
}
