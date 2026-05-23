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
 * @brief Cross-module interface exposed by the kernel module.
 ******************************************************************************/

#ifndef VQMBT_H
#define VQMBT_H

#include <stdbool.h>
#include <vitasdk/build_utils.h>

#define VQMBT_MAX_DEVICES 8  // Maximum number of bluetooth devices the PS Vita can be paired with.

// Error codes.
#define VQMBT_ERROR_INVALID_ARGUMENT ((int)0x80690001)
#define VQMBT_ERROR_KERNEL_SIDE ((int)0x80690002)

typedef enum VqmbtBtEventId {
    VQMBT_BT_EVENT_INQUIRY_RESULT = 0x01,
    VQMBT_BT_EVENT_INQUIRY_STOP = 0x02,
    VQMBT_BT_EVENT_DISCONNECT = 0x06,
    VQMBT_BT_EVENT_ADD_REMOVE_DEVICE = 0x07,
    // AI
    VQMBT_BT_EVENT_LINK_KEY_REQUEST = 0x04,
    VQMBT_BT_EVENT_CONNECT_ACCEPTED = 0x05,
    VQMBT_BT_EVENT_CONNECT_REQUESTED = 0x08,
    VQMBT_BT_EVENT_CONNECT_UNPAIRED = 0x09,
    VQMBT_BT_EVENT_HID_REPLY_TYPE0 = 0x0A,
    VQMBT_BT_EVENT_HID_REPLY_TYPE1 = 0x0B,
} VqmbtBtEventId;

typedef struct VqmbtDeviceInfo {
    char name[128];
    unsigned int mac0;
    unsigned int mac1;
    int state;
} VqmbtDeviceInfo;
VITASDK_BUILD_ASSERT_EQ(0x8C, VqmbtDeviceInfo);

int kvqmbtGetPairedDevices(VqmbtDeviceInfo* info, int info_size);

void kvqmbtConnectDevice(unsigned int mac0, unsigned int mac1);

void kvqmbtDisconnectDevice(unsigned int mac0, unsigned int mac1);

#endif  // VQMBT_H
