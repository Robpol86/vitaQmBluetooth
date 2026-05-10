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

/**
 * Iterate through all paired bluetooth devices and log their information.
 *
 * TODO:
 * - Try with two paired
 * - Pair three devices, then unpair the middle one. Will registered slots be contiguous?
 * - Memory eficiency? deallocate?
 * - Investigate why APP2 and APP1Scuffed caused boot lock. Remove app2 and will the app1 name cause it? Or is it n>1?
 * - Log connection state (reflect settings app)
 */
void log_paired_devices(void) {
    uint32_t state;
    ENTER_SYSCALL(state);

    SceBtRegisteredInfo device_info;
    int count = 0;
    unsigned int prev_mac0 = 0;  // TODO needed or can it be 0? See comment below (same variable).

    for (int i = 0; i < MAX_DEVICES; i++) {
        int ret = ksceBtGetRegisteredInfo(i, prev_mac0, &device_info, sizeof(device_info));

        // If slot is empty log and continue.
        if (ret != 1) {
            LOG_DEBUG("slot=%d ret=%d", i, ret);
            continue;
        }

        // Log known device info fields.
        const unsigned char* m = (const unsigned char*)&device_info.mac;
        LOG_DEBUG("slot=%d ret=%d mac=%02X:%02X:%02X:%02X:%02X:%02X name=\"%s\" class=0x%08X vid=0x%04X pid=0x%04X", i,
                  ret, m[0], m[1], m[2], m[3], m[4], m[5], device_info.name, device_info.bt_class, device_info.vid,
                  device_info.pid);

        prev_mac0 = (m[3] << 24) | (m[2] << 16) | (m[1] << 8) | m[0];
        count++;
    }

    LOG_DEBUG("Found %d paired device(s)", count);

    EXIT_SYSCALL(state);
}
