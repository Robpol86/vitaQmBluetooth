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

/**
 * TODO bluetooth interface.
 */

#include <psp2kern/bt.h>
#include <psp2kern/kernel/modulemgr.h>
#include <psp2kern/kernel/sysmem.h>
#include <psp2kern/kernel/threadmgr.h>

#include "log.h"

#define MAX_DEVICES 8  // Maximum number of bluetooth devices the PS Vita can be paired with.

/**
 * TODO just log.
 *
 * TODO:
 * - Try with two paired
 * - Pair three devices, then unpair the middle one. Will registered slots be contiguous?
 * - Memory eficiency? deallocate?
 * - Investigate why APP2 and APP1Scuffed caused boot lock. Remove app2 and will the app1 name cause it? Or is it n>1?
 * - Log connection state (reflect settings app)
 */
void log_paired_devices() {
    SceBtRegisteredInfo deviceInfo;
    int count = 0;
    unsigned int prev_mac_lo = 0;  // TODO needed or can it be 0?

    for (int i = 0; i < MAX_DEVICES; i++) {
        int ret = ksceBtGetRegisteredInfo(i, prev_mac_lo, &deviceInfo, sizeof(deviceInfo));

        // If slot is empty log and continue.
        if (ret != 1) {
            LOG_DEBUG("slot=%d ret=%d", i, ret);
            continue;
        }

        // Log known device info fields.
        const unsigned char* m = (const unsigned char*)&deviceInfo.mac;
        LOG_DEBUG("slot=%d ret=%d mac=%02X:%02X:%02X:%02X:%02X:%02X name=\"%s\" class=0x%08X vid=0x%04X pid=0x%04X", i,
                  ret, m[0], m[1], m[2], m[3], m[4], m[5], deviceInfo.name, deviceInfo.bt_class, deviceInfo.vid,
                  deviceInfo.pid);

        // Log unknown fields except unk5.
        LOG_DEBUG("       unk0=0x%04X unk1=0x%08X unk2=0x%08X unk3=0x%08X unk4=0x%08X", deviceInfo.unk0, deviceInfo.unk1,
                  deviceInfo.unk2, deviceInfo.unk3, deviceInfo.unk4);

        // Log unk5 in rows of 16 bytes.
        for (int row = 0; row < 0x60; row += 16) {
            LOG_DEBUG(
                "       unk5[0x%02X]=%02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X",
                row, deviceInfo.unk5[row + 0], deviceInfo.unk5[row + 1], deviceInfo.unk5[row + 2],
                deviceInfo.unk5[row + 3], deviceInfo.unk5[row + 4], deviceInfo.unk5[row + 5], deviceInfo.unk5[row + 6],
                deviceInfo.unk5[row + 7], deviceInfo.unk5[row + 8], deviceInfo.unk5[row + 9], deviceInfo.unk5[row + 10],
                deviceInfo.unk5[row + 11], deviceInfo.unk5[row + 12], deviceInfo.unk5[row + 13],
                deviceInfo.unk5[row + 14], deviceInfo.unk5[row + 15]);
        }

        prev_mac_lo = (m[2] << 24) | (m[3] << 16) | (m[4] << 8) | m[5];  // TODO remove?
        count++;
    }

    LOG_DEBUG("Found %d paired device(s)", count);
}
