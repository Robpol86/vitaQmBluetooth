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

    // Validate.
    if (info == NULL || info_size < 1 || info_size > VQMBT_MAX_DEVICES) {
        LOG_ERROR("Invalid argument: info=%p info_size=%d", info, info_size);
        return VQMBT_ERROR_INVALID_ARGUMENT;
    }

    // Zero the kernel-side array to prevent ghost data.
    for (int i = 0; i < (int)sizeof(paired_devices); i++) ((unsigned char*)paired_devices)[i] = 0;

    // Populate file-scoped array with all currently paired devices.
    int mac0 = 0, mac1 = 0;
    int count = ksceBtGetRegisteredInfo(mac0, mac1, paired_devices, info_size);
    if (count < 0) {
        LOG_ERROR("ksceBtGetRegisteredInfo returned error: 0x%08X", count);
        return VQMBT_ERROR_KERNEL_SIDE;
    }
    LOG_DEBUG(0, "ksceBtGetRegisteredInfo returned count=%d info_size=%d max=%d", count, info_size, VQMBT_MAX_DEVICES);

    // Marshal each record across the kernel/user boundary.
    for (int idx = 0; idx < count; idx++) {
        // Get kernel side.
        SceBtRegisteredInfo* sceDev = &paired_devices[idx];
        const unsigned char* m = (const unsigned char*)&sceDev->mac;  // TODO rename?
        LOG_DEBUG(0, "idx=%d mac=%02X:%02X:%02X:%02X:%02X:%02X name=\"%s\" class=0x%08X vid=0x%04X pid=0x%04X", idx, m[0],
                  m[1], m[2], m[3], m[4], m[5], sceDev->name, sceDev->bt_class, sceDev->vid, sceDev->pid);
        LOG_DEBUG(0, "      unk0=0x%04X unk1=0x%08X unk2=0x%08X unk3=0x%08X unk4=0x%08X", sceDev->unk0, sceDev->unk1,
                  sceDev->unk2, sceDev->unk3, sceDev->unk4);
        for (int row = 0; row < 0x60; row += 16) {
            LOG_DEBUG(
                10000,
                "      unk5[0x%02X]=%02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X", row,
                sceDev->unk5[row + 0], sceDev->unk5[row + 1], sceDev->unk5[row + 2], sceDev->unk5[row + 3],
                sceDev->unk5[row + 4], sceDev->unk5[row + 5], sceDev->unk5[row + 6], sceDev->unk5[row + 7],
                sceDev->unk5[row + 8], sceDev->unk5[row + 9], sceDev->unk5[row + 10], sceDev->unk5[row + 11],
                sceDev->unk5[row + 12], sceDev->unk5[row + 13], sceDev->unk5[row + 14], sceDev->unk5[row + 15]);
        }

        // HERE
        VqmbtDeviceInfo dev;

        // TODO remove
        LOG_DEBUG(0, "name=\"%s\" name[127]=0x%02X (0=terminated, else=not)", sceDev->name,
                  (unsigned char)sceDev->name[sizeof(sceDev->name) - 1]);

        // Copy the device name. SceBtRegisteredInfo.name is 0x4F bytes; VqmbtDeviceInfo.name is 128.
        // Byte loop instead of memcpy/strncpy to satisfy clang-analyzer-security.insecureAPI.
        for (int j = 0; j < (int)sizeof(dev.name); j++) {
            dev.name[j] = sceDev->name[j];
        }

        // Pack the MAC bytes into mac0/mac1 using the SceBt convention (LE bytes 0..3 into mac0,
        // LE bytes 4..5 in the low 16 bits of mac1).
        dev.mac0 = ((unsigned int)m[3] << 24) | ((unsigned int)m[2] << 16) | ((unsigned int)m[1] << 8) | m[0];
        dev.mac1 = ((unsigned int)m[5] << 8) | m[4];

        // Copy this entry into the user buffer slot.
        int ret = ksceKernelCopyToUser(&info[idx], &dev, sizeof(dev));
        if (ret < 0) {
            LOG_DEBUG(0, "ksceKernelCopyToUser failed at index %d: 0x%08X", idx, ret);
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
