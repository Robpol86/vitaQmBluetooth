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
 * TODO rewrite this comment.
 *
 * TODO:
 * - Is 8 actually the max number of devices?
 * - Memory eficiency? deallocate paired_devices?
 * - Log connection state (reflect settings app)
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

    // Copy each record across the kernel/user boundary.
    for (int idx = 0; idx < count; idx++) {
        // Get kernel side.
        SceBtRegisteredInfo* sceDev = &paired_devices[idx];
        const unsigned char* mac = (const unsigned char*)&sceDev->mac;  // TODO rename?
        LOG_DEBUG(0, "idx=%d mac=%02X:%02X:%02X:%02X:%02X:%02X name=\"%s\" class=0x%08X vid=0x%04X pid=0x%04X", idx,
                  mac[0], mac[1], mac[2], mac[3], mac[4], mac[5], sceDev->name, sceDev->bt_class, sceDev->vid,
                  sceDev->pid);
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

        // Populate user side.
        VqmbtDeviceInfo dev;
        for (int j = 0; j < (int)sizeof(dev.name); j++) dev.name[j] = sceDev->name[j];  // TODO strncpy?
        dev.mac0 = ((unsigned int)mac[3] << 24) | ((unsigned int)mac[2] << 16) | ((unsigned int)mac[1] << 8) | mac[0];
        dev.mac1 = ((unsigned int)mac[5] << 8) | mac[4];

        // Export to user space.
        int ret = ksceKernelCopyToUser(&info[idx], &dev, sizeof(dev));
        if (ret < 0) {
            LOG_ERROR("ksceKernelCopyToUser failed at index %d: 0x%08X", idx, ret);
            return VQMBT_ERROR_KERNEL_SIDE;
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
void connect_or_disconnect(int device_index) {
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
