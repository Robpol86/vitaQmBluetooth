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
#include <psp2kern/kernel/sysclib.h>
#include <psp2kern/kernel/sysmem.h>
#include <psp2kern/kernel/threadmgr.h>

#include "log.h"

#define MAX_DEVICES 8  // Maximum number of bluetooth devices the PS Vita can be paired with.

static SceBtRegisteredInfo paired_devices[MAX_DEVICES];

/**
 * Disconnect first bluetooth device if connected, and vice versa.
 *
 * PoC confirmed! I have verified this function connects and disconnects my AirPods Pro from the Quick Menu whilst
 * RetroArch was running.
 */
static void connect_or_disconnect(int device_index) {
    SceBtRegisteredInfo* device_info = &paired_devices[device_index];
    const unsigned char* m = (const unsigned char*)&device_info->mac;
    unsigned int mac0 = (m[3] << 24) | (m[2] << 16) | (m[1] << 8) | m[0];
    unsigned int mac1 = (m[5] << 8) | m[4];

    // Sanity check.
    int ret;
    char name[0x79];
    ret = ksceBtGetDeviceName(mac0, mac1, name);
    LOG_DEBUG(0, "Got name: ret=%d name=\"%s\"", ret, name);
    if (ret != 0) {
        LOG_DEBUG(0, "UNKNOWN DEVICE");
        return;
    }

    // Get current state
    LOG_DEBUG(0, "Reading state for \"%s\"", device_info->name);
    int state = ksceBtGetConnectingInfo(mac0, mac1);  // 1 == unknown/disconnected; 6 == connected
    LOG_DEBUG(0, "Got state: %d", state);

    // Connect or disconnect.
    if (state == 1) {
        // TODO does not work if Settings is open in the Bluetooth Devices view.
        LOG_DEBUG(0, "Connecting \"%s\"", device_info->name);
        ret = ksceBtStartConnect(mac0, mac1);
        if (ret < 0) {
            LOG_DEBUG(0, "ksceBtStartConnect returned error: 0x%08X", ret);
        } else {
            LOG_DEBUG(0, "ksceBtStartConnect returned: %d", ret);
        }
    } else if (state == 6 || state == 5) {  // 6=Ovaltine 5=APPScuffed
        LOG_DEBUG(0, "Disconnecting \"%s\"", device_info->name);
        ret = ksceBtStartDisconnect(mac0, mac1);
        if (ret < 0) {
            LOG_DEBUG(0, "ksceBtStartDisconnect returned error: 0x%08X", ret);
        } else {
            LOG_DEBUG(0, "ksceBtStartDisconnect returned: %d", ret);
        }
        LOG_DEBUG(0, "Disconnect ret=%d", ret);
    } else {
        LOG_DEBUG(0, "Unknown state");
    }
}

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
    // Zero the array to prevent ghost data.
    for (int i = 0; i < (int)sizeof(paired_devices); i++) ((unsigned char*)paired_devices)[i] = 0;

    // Populate the array with all currently paired devices.
    int mac0 = 0;  // TODO test single device lookup to confirm params are actualy mac0/mac1.
    int mac1 = 0;
    int count = ksceBtGetRegisteredInfo(mac0, mac1, paired_devices, MAX_DEVICES);
    if (count < 0) {
        LOG_DEBUG(0, "ksceBtGetRegisteredInfo returned error: 0x%08X", count);
        return;
    }
    LOG_DEBUG(0, "count=%d max=%d", count, MAX_DEVICES);

    // Log each device.
    int conn_disconn_dev = 0;
    for (int i = 0; i < count; i++) {
        SceBtRegisteredInfo* device_info = &paired_devices[i];
        if (strncmp(device_info->name, "APP Scuffed", 11) == 0) {
            LOG_DEBUG(0, "Set conn_disconn_dev to %d for device \"%s\"", i, device_info->name);
            conn_disconn_dev = i;
        }

        // Log known device info fields.
        const unsigned char* m = (const unsigned char*)&device_info->mac;
        LOG_DEBUG(50000, "num=%d mac=%02X:%02X:%02X:%02X:%02X:%02X name=\"%s\" class=0x%08X vid=0x%04X pid=0x%04X", i,
                  m[0], m[1], m[2], m[3], m[4], m[5], device_info->name, device_info->bt_class, device_info->vid,
                  device_info->pid);

        // Log unknown fields except unk5.
        LOG_DEBUG(0, "      unk0=0x%04X unk1=0x%08X unk2=0x%08X unk3=0x%08X unk4=0x%08X", device_info->unk0,
                  device_info->unk1, device_info->unk2, device_info->unk3, device_info->unk4);

        // Log unk5 in rows of 16 bytes.
        for (int row = 0; row < 0x60; row += 16) {
            LOG_DEBUG(
                0, "      unk5[0x%02X]=%02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X",
                row, device_info->unk5[row + 0], device_info->unk5[row + 1], device_info->unk5[row + 2],
                device_info->unk5[row + 3], device_info->unk5[row + 4], device_info->unk5[row + 5],
                device_info->unk5[row + 6], device_info->unk5[row + 7], device_info->unk5[row + 8],
                device_info->unk5[row + 9], device_info->unk5[row + 10], device_info->unk5[row + 11],
                device_info->unk5[row + 12], device_info->unk5[row + 13], device_info->unk5[row + 14],
                device_info->unk5[row + 15]);
        }
    }

    // Connect or disconnect first device, depending on current state.
    if (count > 0) {
        LOG_DEBUG(0, "Connecting or disconnecting device %d", conn_disconn_dev);
        connect_or_disconnect(conn_disconn_dev);
    }
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
