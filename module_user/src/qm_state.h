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

#ifndef QM_STATE_H
#define QM_STATE_H

#include <stdbool.h>

#include "vqmbt.h"

// Button IDs.
const char* const ID_BUTTONS[VQMBT_MAX_DEVICES] = {
    MODULE_NAME "Button0", MODULE_NAME "Button1", MODULE_NAME "Button2", MODULE_NAME "Button3",
    MODULE_NAME "Button4", MODULE_NAME "Button5", MODULE_NAME "Button6", MODULE_NAME "Button7",
};
_Static_assert(sizeof(ID_BUTTONS) / sizeof(ID_BUTTONS[0]) == VQMBT_MAX_DEVICES, "ID_BUTTONS size != VQMBT_MAX_DEVICES");

// UI request.
typedef enum QmRequestId : unsigned int {
    REQUEST_BULK_UPDATE = 0,
    REQUEST_BUTTON_PRESSED,
    REQUEST_BLUETOOTH_ON,
    REQUEST_BLUETOOTH_OFF,
    REQUEST_DEVICE_DISCONNECTED,
    REQUEST_DEVICE_CONNECTED,
} QmRequestId;
typedef struct QmRequest {
    QmRequestId id;
    union {
        int idx;
        struct {
            unsigned int mac0, mac1;
        } mac;
        struct {
            bool bluetooth_on;
            int num_devices;
            const VqmbtDeviceInfo* devices;
        } bulk;
    };
} QmRequest;

void update_ui(const QmRequest* request);
int qm_state_start(void);
int qm_state_stop(void);

#endif  // QM_STATE_H
