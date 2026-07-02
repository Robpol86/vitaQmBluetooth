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
 * @brief DELETE ME.
 ******************************************************************************/

#include <psp2kern/bt.h>

#include "log.h"
#include "syscall.h"
#include "vqmbt.h"

int example_syscall(void) {
    uint32_t syscall_state_ SYSCALL_STATE = 0;
    ENTER_SYSCALL(syscall_state_);

    VqmbtDeviceInfo dev = {0};
    return sizeof(dev);
}

VqmbtInferredDevState example_kvqmbt_device_state(unsigned int mac0, unsigned int mac1) {
    uint32_t syscall_state_ SYSCALL_STATE = 0;
    ENTER_SYSCALL(syscall_state_);

    VqmbtInferredDevState state = ksceBtGetConnectingInfo(mac0, mac1);
    LOG_DEBUG(0, "ksceBtGetConnectingInfo(mac0=%08X, mac1=%08X) returned state=%d", mac0, mac1, state);

    return state;
}
