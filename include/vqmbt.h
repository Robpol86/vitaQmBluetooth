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

#include <psp2common/types.h>
#include <stdbool.h>
#include <vitasdk/build_utils.h>

#define VQMBT_MAX_DEVICES 8  // Maximum number of bluetooth devices the PS Vita can be paired with.

// Error codes.
#define VQMBT_ERROR_INVALID_ARGUMENT ((int)0x80690001)
#define VQMBT_ERROR_KERNEL_SIDE ((int)0x80690002)
#define VQMBT_ERROR_CB_OVERFLOW ((int)0x80690003)

// Events used for inter-module communication.
typedef enum VqmbtEventId : unsigned int {
    VQMBT_EVENT_NONE = 0x00,
    VQMBT_EVENT_BLUETOOTH_ENABLED = 0x01,
    VQMBT_EVENT_BLUETOOTH_DISABLED = 0x02,
} VqmbtEventId;
typedef struct VqmbtEvent {
    VqmbtEventId id;
} VqmbtEvent;
VITASDK_BUILD_ASSERT_EQ(0x04, VqmbtEvent);

typedef struct VqmbtDeviceInfo {
    char name[128];
    unsigned int mac0;
    unsigned int mac1;
    int state;
} VqmbtDeviceInfo;
VITASDK_BUILD_ASSERT_EQ(0x8C, VqmbtDeviceInfo);

// bluetooth.c syscalls.
int kvqmbt_get_paired_devices(VqmbtDeviceInfo* info, int info_size);
bool kvqmbt_is_connected(unsigned int mac0, unsigned int mac1);
void kvqmbt_connect_device(unsigned int mac0, unsigned int mac1);
void kvqmbt_disconnect_device(unsigned int mac0, unsigned int mac1);

// umod_callback.c syscalls.
int kvqmbt_register_callback(SceUID cb);
void kvqmbt_unregister_callback(void);
int kvqmbt_read_event(VqmbtEvent* event);

#endif  // VQMBT_H
