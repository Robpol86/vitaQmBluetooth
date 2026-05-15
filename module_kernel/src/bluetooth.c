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

#include "common.h"
#include "log.h"
#include "vqmbt.h"

static SceBtRegisteredInfo paired_devices[VQMBT_MAX_DEVICES];

/**
 * Disconnect first bluetooth device if connected, and vice versa.
 *
 * PoC confirmed! I have verified this function connects and disconnects my AirPods Pro from the Quick Menu whilst
 * RetroArch was running.
 */
static void connect_or_disconnect(int device_index) {
    SceBtRegisteredInfo* device_info = &paired_devices[device_index];
    const unsigned char* mac = (const unsigned char*)&device_info->mac;
    unsigned int mac0 = (mac[3] << 24) | (mac[2] << 16) | (mac[1] << 8) | mac[0];
    unsigned int mac1 = (mac[5] << 8) | mac[4];

    // Sanity check.
    int ret;
    char name[0x79];
    ret = ksceBtGetDeviceName(mac0, mac1, name);
    LOG_DEBUG(0, "ksceBtGetDeviceName: ret=%d name=\"%s\" mac0=0x%08X mac1=0x%08X", ret, name, mac0, mac1);
    if (ret != 0) {
        LOG_ERROR("UNKNOWN DEVICE");
        return;
    }

    // Get current state
    LOG_DEBUG(0, "Reading state for \"%s\"", device_info->name);
    int state = ksceBtGetConnectingInfo(mac0, mac1);  // 1 == unknown/disconnected; 6 == connected
    LOG_DEBUG(0, "Got state: %d", state);

    // Connect or disconnect.
    if (state == 1) {
        // Fails if Settings is open in the Bluetooth Devices view.
        LOG_DEBUG(0, "Connecting \"%s\"", device_info->name);
        ret = ksceBtStartConnect(mac0, mac1);
        if (ret < 0) {
            LOG_ERROR("ksceBtStartConnect returned error: 0x%08X", ret);
        } else {
            LOG_DEBUG(0, "ksceBtStartConnect returned: %d", ret);
        }
    } else if (state == 6 || state == 5) {  // 6=Ovaltine 5=APPScuffed
        LOG_DEBUG(0, "Disconnecting \"%s\"", device_info->name);
        ret = ksceBtStartDisconnect(mac0, mac1);
        if (ret < 0) {
            LOG_ERROR("ksceBtStartDisconnect returned error: 0x%08X", ret);
        } else {
            LOG_DEBUG(0, "ksceBtStartDisconnect returned: %d", ret);
        }
    } else {
        LOG_ERROR("Unknown state");
    }
}

/**
 * Iterate through all paired bluetooth devices and log their information.
 *
 * TODO:
 * - Is 8 actually the max number of devices?
 * - Memory eficiency? deallocate?
 * - Log connection state (reflect settings app)
 */
void log_paired_devices(void) {
    uint32_t state SYSCALL_STATE = 0;
    ENTER_SYSCALL(state);

    // Zero the array to prevent ghost data.
    for (int i = 0; i < (int)sizeof(paired_devices); i++) ((unsigned char*)paired_devices)[i] = 0;

    // Populate the array with all currently paired devices.
    int mac0 = 0, mac1 = 0;
    int count = ksceBtGetRegisteredInfo(mac0, mac1, paired_devices, VQMBT_MAX_DEVICES);
    if (count < 0) {
        LOG_ERROR("ksceBtGetRegisteredInfo returned error: 0x%08X", count);
        return;
    }
    LOG_DEBUG(0, "count=%d max=%d", count, VQMBT_MAX_DEVICES);

    // Log each device.
    int conn_disconn_dev = 0;
    for (int idx = 0; idx < count; idx++) {
        SceBtRegisteredInfo* device_info = &paired_devices[idx];
        if (strncmp(device_info->name, "APP Scuffed", 11) == 0) {
            LOG_DEBUG(0, "Set conn_disconn_dev to %d for device \"%s\"", idx, device_info->name);
            conn_disconn_dev = idx;
        }

        // Log known device info fields.
        const unsigned char* mac = (const unsigned char*)&device_info->mac;
        LOG_DEBUG(50000, "idx=%d mac=%02X:%02X:%02X:%02X:%02X:%02X name=\"%s\" class=0x%08X vid=0x%04X pid=0x%04X", idx,
                  mac[0], mac[1], mac[2], mac[3], mac[4], mac[5], device_info->name, device_info->bt_class,
                  device_info->vid, device_info->pid);

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
