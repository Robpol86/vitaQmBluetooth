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

/**
 * TODO:
 * - On full buffer drop old events, but notify consumer (umod) of this happening so it can call
 *   kvqmbt_get_paired_devices() to "reset".
 * - Increase consumer delay and confirm their notifyCount increases from 1 to 2 or whatever.
 */
#include "umod_callback.h"

#include <psp2kern/kernel/threadmgr.h>
#include <psp2kern/kernel/threadmgr/mutex.h>
#include <stdbool.h>

#include "log.h"
#include "syscall.h"
#include "vqmbt.h"

// TODO
// #define RINGBUF_CAPACITY 16
// static SceUID user_cb_uid = -1;
// static VqmbtEvent ring_buffer[RINGBUF_CAPACITY];
static SceUID mutex_id = -1;

/**
 * TODO
 *
 * @param event TODO
 * @return TODO
 */
int umod_cb_emit_event(const VqmbtEvent* event) {
    LOG_DEBUG(0, "TODO %p", event);

    // Obtain lock.
    int ret = ksceKernelLockMutex(mutex_id, 1, NULL);
    if (ret < 0) {
        LOG_ERROR("ksceKernelLockMutex returned error: 0x%08X", ret);
        return ret;
    }

    // TODO

    // Release lock.
    ret = ksceKernelUnlockMutex(mutex_id, 1);
    if (ret < 0) {
        LOG_ERROR("ksceKernelUnlockMutex returned error: 0x%08X", ret);
        return ret;
    }

    // Notify consumer the event is ready.
    // ksceKernelNotifyCallback(); // TODO explore arg2.

    return 0;  // TODO? VQMBT_ERROR_CB_OVERFLOW? Return number of events read now (1 or 0 or <0 on error).
}

/**
 * TODO
 *
 * @param event TODO
 * @return TODO
 */
int kvqmbt_read_event(VqmbtEvent* event) {
    uint32_t syscall_state_ SYSCALL_STATE = 0;
    ENTER_SYSCALL(syscall_state_);

    LOG_DEBUG(0, "TODO %p", event);

    // TODO

    return 0;  // TODO? VQMBT_ERROR_CB_OVERFLOW? Return number of events read now (1 or 0 or <0 on error).
}

/**
 * TODO
 *
 * @param cb TODO
 * @return TODO
 */
int kvqmbt_register_callback(SceUID cb) {
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
int kvqmbt_unregister_callback(SceUID cb) {
    uint32_t syscall_state_ SYSCALL_STATE = 0;
    ENTER_SYSCALL(syscall_state_);

    LOG_DEBUG(0, "Unregistering callback UID 0x%08X", cb);

    // TODO

    return 0;  // TODO?
}

/**
 * TODO.
 */
int umod_cb_start(void) {
    // Create a mutex.
    mutex_id = ksceKernelCreateMutex("kvqmbt-umod_callback-mutex", 0, 0, NULL);
    if (mutex_id < 0) {
        LOG_ERROR("ksceKernelCreateMutex returned error 0x%08X", mutex_id);
        return (int)mutex_id;
    }
    LOG_DEBUG(0, "ksceKernelCreateMutex returned mutex_id=0x%08X", mutex_id);

    return 0;
}

/**
 * TODO.
 */
int umod_cb_stop(void) {
    // Delete the mutex.
    int ret = ksceKernelDeleteMutex(mutex_id);
    if (ret < 0) {
        LOG_ERROR("ksceKernelDeleteMutex returned error 0x%08X", ret);
        return ret;
    }
    LOG_DEBUG(0, "ksceKernelDeleteMutex returned 0x%08X", ret);

    return 0;
}
