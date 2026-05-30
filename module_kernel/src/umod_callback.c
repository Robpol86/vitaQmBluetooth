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
 * - Increase consumer delay and confirm their notifyCount increases from 1 to 2 or whatever.
 * - Confirm ring buffer overrun scenario by lowering RING_BUFFER_SIZE
 * - Confirm uint overflow scenario by setting intial value to MAX_INT - 5
 * - understand atomic functions
 * - On boot before user module is started kernel module does work. Make sure it's not sending to user before it's ready.
 * - consistent LOG_DEBUG/ERROR(... "error 0x" or ... "returned %d"), no colon.
 */

#include "umod_callback.h"

#include <psp2kern/kernel/sysmem/data_transfers.h>
#include <psp2kern/kernel/threadmgr.h>
#include <stdatomic.h>

#include "log.h"
#include "syscall.h"
#include "vqmbt.h"

#define RING_BUFFER_SIZE 16

static VqmbtEvent ring_buffer[RING_BUFFER_SIZE];
static _Atomic unsigned int ring_buffer_write_idx = 0;
static _Atomic unsigned int ring_buffer_read_idx = 0;
static _Atomic SceUID registered_cb_uid = -1;

/**
 * TODO
 *
 * @param event TODO
 * @return TODO
 */
int umod_cb_emit_event(const VqmbtEvent* event) {
    if (event == NULL || event->id == VQMBT_EVENT_NONE) {
        return VQMBT_ERROR_INVALID_ARGUMENT;
    }

    // Write event into ring buffer atomically.
    unsigned int write_idx = atomic_load_explicit(&ring_buffer_write_idx, memory_order_relaxed);
    int slot_no = write_idx % RING_BUFFER_SIZE;
    ring_buffer[slot_no] = *event;
    atomic_store_explicit(&ring_buffer_write_idx, write_idx + 1, memory_order_release);
    LOG_DEBUG(0, "Wrote event to slot_no=%d", slot_no);

    // Tell the consumer an event is ready to be read.
    SceUID cb_uid = atomic_load_explicit(&registered_cb_uid, memory_order_relaxed);
    if (cb_uid >= 0) {
        int ret = ksceKernelNotifyCallback(cb_uid, 0);  // TODO explore arg2
        if (ret < 0) {
            LOG_ERROR("ksceKernelNotifyCallback(cb_uid=0x%08X) returned error 0x%08X", cb_uid, ret);
        } else {
            LOG_DEBUG(0, "ksceKernelNotifyCallback(cb_uid=0x%08X) returned %d", cb_uid, ret);
        }
    }

    return 0;
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

    if (event == NULL) {
        LOG_ERROR("Invalid event pointer: NULL");
        return VQMBT_ERROR_INVALID_ARGUMENT;
    }

    // Atomically read indices.
    unsigned int read_idx = atomic_load_explicit(&ring_buffer_read_idx, memory_order_relaxed);
    unsigned int write_idx = atomic_load_explicit(&ring_buffer_write_idx, memory_order_acquire);
    if (read_idx == write_idx) {
        // No new data.
        return 0;
    }
    if (write_idx - read_idx > RING_BUFFER_SIZE) {
        // Producer wrote more since the last read than the size of the buffer, fault.
        LOG_WARN("Ring buffer overrun: write_idx=%u read_idx=%u (delta=%u)", write_idx, read_idx, write_idx - read_idx);
        atomic_store_explicit(&ring_buffer_read_idx, write_idx, memory_order_relaxed);
        return VQMBT_ERROR_CB_OVERFLOW;
    }

    // Read event from buffer, then check again if buffer was overrun during the read.
    VqmbtEvent event_from_buffer = ring_buffer[read_idx % RING_BUFFER_SIZE];
    write_idx = atomic_load_explicit(&ring_buffer_write_idx, memory_order_acquire);
    if (write_idx - read_idx > RING_BUFFER_SIZE) {
        LOG_WARN("Ring buffer overrun mid-read: write_idx=%u read_idx=%u (delta=%u)", write_idx, read_idx,
                 write_idx - read_idx);
        atomic_store_explicit(&ring_buffer_read_idx, write_idx, memory_order_relaxed);
        return VQMBT_ERROR_CB_OVERFLOW;
    }
    atomic_store_explicit(&ring_buffer_read_idx, read_idx + 1, memory_order_relaxed);  // Commit.

    // Export to user space.
    int ret = ksceKernelCopyToUser(event, &event_from_buffer, sizeof(event_from_buffer));
    if (ret < 0) {
        LOG_ERROR("ksceKernelCopyToUser returned error: 0x%08X", ret);
        return VQMBT_ERROR_KERNEL_SIDE;
    }

    return 1;
}

/**
 * TODO
 *
 * @param cb_uid TODO
 * @return TODO
 */
int kvqmbt_register_callback(SceUID cb_uid) {
    uint32_t syscall_state_ SYSCALL_STATE = 0;
    ENTER_SYSCALL(syscall_state_);

    if (cb_uid < 0) {
        LOG_ERROR("Invalid callback UID: 0x%08X", cb_uid);
        return VQMBT_ERROR_INVALID_ARGUMENT;
    }

    atomic_store_explicit(&registered_cb_uid, cb_uid, memory_order_relaxed);
    LOG_DEBUG(0, "Registered callback UID 0x%08X", cb_uid);

    return 0;
}

/**
 * TODO
 *
 * @return TODO
 */
void kvqmbt_unregister_callback(void) {
    uint32_t syscall_state_ SYSCALL_STATE = 0;
    ENTER_SYSCALL(syscall_state_);

    SceUID cb_uid_old = atomic_exchange_explicit(&registered_cb_uid, -1, memory_order_relaxed);
    LOG_DEBUG(0, "Unregistered callback UID 0x%08X", cb_uid_old);
}

/**
 * TODO.
 */
int umod_cb_start(void) {
    // TODO

    return 0;
}

/**
 * TODO.
 */
int umod_cb_stop(void) {
    // TODO

    return 0;
}
