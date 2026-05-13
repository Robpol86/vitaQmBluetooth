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
 * Enumerate paired bluetooth devices and copy summarized info into the user-supplied array.
 *
 * Acts as a syscall: marshals data across the kernel/user address boundary using
 * ksceKernelCopyToUser, since the kernel cannot dereference user pointers directly
 * (DACR enforcement).
 *
 * @param info User-space pointer to an array of VqmbtDeviceInfo records.
 * @param info_size Capacity of the user array, in records.
 * @return Number of records written on success (0 to min(info_size, MAX_DEVICES, registered)),
 *         or a negative error code on failure.
 */
int kvqmbtGetPairedDevices(VqmbtDeviceInfo* info, int info_size) {
    uint32_t state SYSCALL_STATE = 0;
    ENTER_SYSCALL(state);

    // Validate user inputs.
    if (info == NULL || info_size <= 0) {
        LOG_DEBUG(0, "Invalid args: info=%p info_size=%d", info, info_size);
        return -1;
    }

    // Clamp to local buffer capacity. Querying more than VQMBT_MAX_DEVICES would overflow paired_devices.
    int cap = info_size;
    if (cap > VQMBT_MAX_DEVICES) cap = VQMBT_MAX_DEVICES;

    // Zero the kernel-side array to prevent ghost data from a previous call.
    for (int i = 0; i < (int)sizeof(paired_devices); i++) ((unsigned char*)paired_devices)[i] = 0;

    // Enumerate registered devices into the kernel-side array.
    int count = ksceBtGetRegisteredInfo(0, 0, paired_devices, cap);
    if (count < 0) {
        LOG_DEBUG(0, "ksceBtGetRegisteredInfo returned error: 0x%08X", count);
        return count;
    }
    LOG_DEBUG(0, "count=%d cap=%d", count, cap);

    // Marshal each record across the kernel/user boundary.
    for (int i = 0; i < count; i++) {
        SceBtRegisteredInfo* src = &paired_devices[i];
        VqmbtDeviceInfo entry;

        // Copy the device name. SceBtRegisteredInfo.name is 0x4F bytes; VqmbtDeviceInfo.name is 128.
        // Always NUL-terminate defensively in case the source isn't terminated.
        for (int j = 0; j < (int)sizeof(entry.name); j++) entry.name[j] = 0;
        strncpy(entry.name, src->name, sizeof(entry.name) - 1);

        // Pack the MAC bytes into mac0/mac1 using the SceBt convention (LE bytes 0..3 into mac0,
        // LE bytes 4..5 in the low 16 bits of mac1).
        const unsigned char* m = (const unsigned char*)&src->mac;
        entry.mac0 = ((unsigned int)m[3] << 24) | ((unsigned int)m[2] << 16) | ((unsigned int)m[1] << 8) | m[0];
        entry.mac1 = ((unsigned int)m[5] << 8) | m[4];

        // Copy this entry into the user buffer slot.
        int ret = ksceKernelCopyToUser(&info[i], &entry, sizeof(entry));
        if (ret < 0) {
            LOG_DEBUG(0, "ksceKernelCopyToUser failed at index %d: 0x%08X", i, ret);
            return ret;
        }
    }

    return count;
}

/**
 * TODO
 */
int kvqmbtConnectDevice(unsigned int mac0, unsigned int mac1) {
    uint32_t state SYSCALL_STATE = 0;
    ENTER_SYSCALL(state);

    LOG_DEBUG(0, "TODO");
    return 0;
}

/**
 * TODO
 */
int kvqmbtDisconnectDevice(unsigned int mac0, unsigned int mac1) {
    uint32_t state SYSCALL_STATE = 0;
    ENTER_SYSCALL(state);

    LOG_DEBUG(0, "TODO");
    return 0;
}

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
    LOG_DEBUG(0, "ksceBtGetDeviceName: ret=%d name=\"%s\" mac0=0x%08X mac1=0x%04X", ret, name, mac0, mac1);
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
        // Fails if Settings is open in the Bluetooth Devices view.
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
    } else {
        LOG_DEBUG(0, "Unknown state");
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
        LOG_DEBUG(0, "ksceBtGetRegisteredInfo returned error: 0x%08X", count);
        return;
    }
    LOG_DEBUG(0, "count=%d max=%d", count, VQMBT_MAX_DEVICES);

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
