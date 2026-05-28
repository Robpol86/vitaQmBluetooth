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

#include <psp2kern/kernel/threadmgr.h>
#include <stdbool.h>

#include "log.h"
#include "syscall.h"
#include "vqmbt.h"

/**
 * TODO
 *
 * @param cb TODO
 * @return TODO
 */
int kvqmbtRegisterCallback(SceUID cb) {
    uint32_t syscall_state_ SYSCALL_STATE = 0;
    ENTER_SYSCALL(syscall_state_);

    LOG_DEBUG(0, "Registering callback UID 0x%08X", cb);

    // TODO

    return 0;  // TODO?
}

/**
 * TODO
 *
 * @param cb TODO
 * @return TODO
 */
int kvqmbtUnregisterCallback(SceUID cb) {
    uint32_t syscall_state_ SYSCALL_STATE = 0;
    ENTER_SYSCALL(syscall_state_);

    LOG_DEBUG(0, "Unregistering callback UID 0x%08X", cb);

    // TODO

    return 0;  // TODO?
}

/**
 * TODO
 *
 * @param event TODO
 * @return TODO
 */
int kvqmbtReadEvent(VqmbtEvent* event) {
    uint32_t syscall_state_ SYSCALL_STATE = 0;
    ENTER_SYSCALL(syscall_state_);

    LOG_DEBUG(0, "TODO %p", event);

    // TODO

    return 0;  // TODO? VQMBT_ERROR_CB_OVERFLOW? Return number of events read now (1 or 0 or <0 on error).
}

/**
 * TODO
 *
 * @param event TODO
 * @return TODO
 */
int kvqmbtEmitEvent(VqmbtEvent* event) {
    uint32_t syscall_state_ SYSCALL_STATE = 0;
    ENTER_SYSCALL(syscall_state_);

    LOG_DEBUG(0, "TODO %p", event);

    // TODO

    return 0;  // TODO? VQMBT_ERROR_CB_OVERFLOW? Return number of events read now (1 or 0 or <0 on error).
}
