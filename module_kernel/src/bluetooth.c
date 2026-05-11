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

#define MAX_DEVICES 8  // Maximum number of bluetooth devices the PS Vita can be paired with.
#define DEVICE_RECORD_SIZE 0x100

static unsigned char paired_devices_buf[MAX_DEVICES * DEVICE_RECORD_SIZE];  // 2 KiB

/**
 * Iterate through all paired bluetooth devices and log their information.
 *
 * TODO:
 * - Merge in main branch until PR ready.
 * - Double check func args are right.
 * - Test with 0, 1, 2, more devices.
 * - See if extra data is in the buffer, such as battery or other stuff shown in Settings app.
 * - What is DEVICE_RECORD_SIZE?
 * - Is 8 actually the max number of devices?
 * TODO:
 * - Try with two paired
 * - Pair three devices, then unpair the middle one. Will registered slots be contiguous?
 * - Memory eficiency? deallocate?
 * - Investigate why APP2 and APP1Scuffed caused boot lock. Remove app2 and will the app1 name cause it? Or is it n>1?
 * - Log connection state (reflect settings app)
 */
void log_paired_devices(void) {
    uint32_t state;
    ENTER_SYSCALL(state);  // TODO remove?

    // Zero the buffer to detect what the kernel actually writes.
    for (int i = 0; i < (int)sizeof(paired_devices_buf); i++) paired_devices_buf[i] = 0;

    // The 4th argument is max_devices, NOT a byte size!
    int count = ksceBtGetRegisteredInfo(0, 0, (SceBtRegisteredInfo*)paired_devices_buf, MAX_DEVICES);
    LOG_DEBUG("ksceBtGetRegisteredInfo returned count=%d", count);

    if (count < 0) {
        LOG_DEBUG("Error: 0x%08X", count);
        EXIT_SYSCALL(state);
        return;
    }

    for (int i = 0; i < count; i++) {
        SceBtRegisteredInfo* dev = (SceBtRegisteredInfo*)(paired_devices_buf + (i * DEVICE_RECORD_SIZE));
        const unsigned char* m = (const unsigned char*)&dev->mac;
        LOG_DEBUG("slot=%d mac=%02X:%02X:%02X:%02X:%02X:%02X name=\"%s\" class=0x%08X", i, m[0], m[1], m[2], m[3], m[4],
                  m[5], dev->name, dev->bt_class);
    }

    LOG_DEBUG("Found %d paired device(s)", count);
    EXIT_SYSCALL(state);
}
