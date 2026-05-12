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

static SceBtRegisteredInfo paired_devices[MAX_DEVICES];

/**
 * Iterate through all paired bluetooth devices and log their information.
 *
 * TODO:
 * - Merge in main branch until PR ready.
 * - Double check func args are right.
 * - Test with 0, 1, 2, more devices.
 * - See if extra data is in the buffer, such as battery or other stuff shown in Settings app.
 * - What is SCE_BT_REGISTERED_INFO_SIZE?
 * - Is 8 actually the max number of devices?
 * TODO:
 * - Try with two paired
 * - Pair three devices, then unpair the middle one. Will registered slots be contiguous?
 * - Memory eficiency? deallocate?
 * - Investigate why APP2 and APP1Scuffed caused boot lock. Remove app2 and will the app1 name cause it? Or is it n>1?
 * - Log connection state (reflect settings app)
 */
static void _log_paired_devices(void) {
    // Zero the array to detect what the kernel actually writes.
    for (int i = 0; i < (int)sizeof(paired_devices); i++) ((unsigned char*)paired_devices)[i] = 0;

    // The 4th argument is max_devices, NOT a byte size!
    int mac0 = 0;  // TODO test single device lookup to confirm params are actualy mac0/mac1.
    int mac1 = 0;
    int count = ksceBtGetRegisteredInfo(mac0, mac1, paired_devices, MAX_DEVICES);
    LOG_DEBUG(0, "ksceBtGetRegisteredInfo returned count=%d", count);

    if (count < 0) {
        LOG_DEBUG(0, "Error: 0x%08X", count);
        return;
    }

    for (int i = 0; i < count; i++) {
        SceBtRegisteredInfo* dev = &paired_devices[i];
        const unsigned char* m = (const unsigned char*)&dev->mac;
        LOG_DEBUG(0, "slot=%d mac=%02X:%02X:%02X:%02X:%02X:%02X name=\"%s\" class=0x%08X", i, m[0], m[1], m[2], m[3],
                  m[4], m[5], dev->name, dev->bt_class);
    }

    LOG_DEBUG(0, "Found %d paired device(s)", count);
}

/**
 * Iterate through all paired bluetooth devices and log their information.
 */
void log_paired_devices(void) {
    uint32_t state;
    ENTER_SYSCALL(state);
    _log_paired_devices();
    EXIT_SYSCALL(state);
}
