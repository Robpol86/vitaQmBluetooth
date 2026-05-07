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

#define MAX_DEVICES 8  // Maximum number of bluetooth devices the PS Vita can be paired with. This is a failsafe.

/**
 * TODO just log.
 */
void log_paired_devices() {
    SceBtRegisteredInfo info;
    int count = 0;

    // First call: device=0, unk=0. Subsequent: walk via prev MAC.
    // Stop on SCE_BT_ERROR_REG_DELETE_NO_ENTRY (0x802F0102) or any error.
    unsigned int prev_mac_lo = 0;

    for (int i = 0; i < MAX_DEVICES; i++) {
        int ret = ksceBtGetRegisteredInfo(i, prev_mac_lo, &info, sizeof(info));
        if (ret < 0) {
            // TODO bad.
            LOG_DEBUG("ksceBtGetRegisteredInfo[%d] returned 0x%08X (end or error)", i, ret);
            break;
        }
        LOG_DEBUG("GOT RET: %d", ret);

        // The MAC in the struct is 6 bytes; print it and the name.
        const unsigned char* m = (const unsigned char*)&info.mac;
        LOG_DEBUG("Device %d: %02X:%02X:%02X:%02X:%02X:%02X  name=\"%s\"", i, m[0], m[1], m[2], m[3], m[4], m[5],
                  info.name);

        // Pack low word of MAC for next iteration.
        // NOTE: This is the conventional pattern. Verify on hardware!
        prev_mac_lo = (m[2] << 24) | (m[3] << 16) | (m[4] << 8) | m[5];
        count++;

        // TODO log name, connection state, mac, anthing else in settings app.
    }

    LOG_DEBUG("Enumerated %d paired device(s)", count);
}
