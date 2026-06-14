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
 * @brief Trial-and-error guessed bluetooth device system states.
 ******************************************************************************/

#ifndef SCE_CONST_STATE_H
#define SCE_CONST_STATE_H

#define VQMBT_DEVICE_NAME_MAX 0x79  // From ksceBtGetDeviceName function signature in bt.h.

typedef enum VqmbtInferredDevState : int {
    VQMBT_BT_STATE_UNKNOWN0 = 0,
    VQMBT_BT_STATE_DISCONNECTED = 1,
    VQMBT_BT_STATE_CONNECTING = 2,
    VQMBT_BT_STATE_UNKNOWN3 = 3,
    VQMBT_BT_STATE_DISCONNECTING = 4,
    VQMBT_BT_STATE_CONNECTED = 5,
    VQMBT_BT_STATE_REGISTERING = 6,
} VqmbtInferredDevState;

#endif  // SCE_CONST_STATE_H
