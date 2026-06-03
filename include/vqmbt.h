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

#define VQMBT_MAX_DEVICES 8        // Maximum number of bluetooth devices the PS Vita can be paired with.
#define VQMBT_DEVICE_NAME_MAX 128  // TODO reduce to something realistic + buffer.

// Error codes.
#define VQMBT_ERROR_INVALID_ARGUMENT ((int)0x80690001)
#define VQMBT_ERROR_KERNEL_SIDE ((int)0x80690002)
#define VQMBT_ERROR_CB_OVERFLOW ((int)0x80690003)
#define VQMBT_ERROR_NOT_READY ((int)0x80690004)

// Device info.
typedef struct VqmbtDeviceInfo {
    char name[VQMBT_DEVICE_NAME_MAX];
    unsigned int mac0;
    unsigned int mac1;
    int state;  // TODO switch to enum
} VqmbtDeviceInfo;
VITASDK_BUILD_ASSERT_EQ(0x8C, VqmbtDeviceInfo);

// Inter-module events.
typedef enum VqmbtEventId : unsigned int {
    VQMBT_EVENT_NONE = 0,
    VQMBT_EVENT_DROPPED_EVENTS,
    VQMBT_EVENT_BLUETOOTH_ENABLED,
    VQMBT_EVENT_BLUETOOTH_DISABLED,
    VQMBT_EVENT_DEVICE_ADDED_REMOVED_CONNECTING,
    VQMBT_EVENT_DEVICE_DISCONNECTED,
    VQMBT_EVENT_DEVICE_CONNECT_SUCCESS,
    VQMBT_EVENT_DEVICE_CONNECT_FAILED,
    VQMBT_EVENT_DEVICE_CONNECT_ABORTED,
    VQMBT_EVENT_DEVICE_CONNECT_CANCELLED,
} VqmbtEventId;

typedef struct VqmbtEvent {
    VqmbtEventId id;
    unsigned int mac0;
    unsigned int mac1;
} VqmbtEvent;
VITASDK_BUILD_ASSERT_EQ(0x0C, VqmbtEvent);

// bluetooth.c syscalls.
bool kvqmbt_is_connected(unsigned int mac0, unsigned int mac1);
void kvqmbt_connect_device(unsigned int mac0, unsigned int mac1);
void kvqmbt_disconnect_device(unsigned int mac0, unsigned int mac1);
int kvqmbt_get_paired_devices(VqmbtDeviceInfo* info, int info_size);

// umod_callback.c syscalls.
int kvqmbt_read_event(VqmbtEvent* event, int num_events);
SceUID kvqmbt_get_wrapped_event_flag(void);
void kvqmbt_unwrap_event_flag(void);

#endif  // VQMBT_H
