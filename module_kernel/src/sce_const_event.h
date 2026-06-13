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
 * @brief TODO.
 ******************************************************************************/

#ifndef SCE_CONST_EVENT_H
#define SCE_CONST_EVENT_H

typedef enum VqmbtInferredBtEventId {
    VQMBT_BT_EVENT_INQUIRY_RESULT = 0x01,
    VQMBT_BT_EVENT_INQUIRY_STOP = 0x02,
    VQMBT_BT_EVENT_PAIRING_REQUEST = 0x04,
    VQMBT_BT_EVENT_CONNECT_RESULT = 0x05,
    VQMBT_BT_EVENT_DISCONNECT = 0x06,
    VQMBT_BT_EVENT_ADD_REMOVE_CONNECTING_DEVICE = 0x07,
    VQMBT_BT_EVENT_CONNECT_REQUESTED = 0x08,
    VQMBT_BT_EVENT_CONNECT_UNPAIRED = 0x09,
    VQMBT_BT_EVENT_BUTTON_PRESSED = 0x0D,
    VQMBT_BT_EVENT_UNKNOWN1 = 0x0E,
    VQMBT_BT_EVENT_UNKNOWN2 = 0x10,
    VQMBT_BT_EVENT_UNKNOWN3 = 0x11,
    VQMBT_BT_EVENT_TOGGLE_BLUETOOTH = 0x15,
    VQMBT_BT_EVENT_UNKNOWN4 = 0x1C,
} VqmbtInferredBtEventId;

#endif  // SCE_CONST_EVENT_H
