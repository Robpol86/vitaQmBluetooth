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

#include "kmod_event.h"

#include <psp2/kernel/threadmgr.h>
#include <stdbool.h>

#include "log.h"
#include "vqmbt.h"

#define THREAD_PRIORITY 0x96 /* Higher value = lower priority. */
#define THREAD_STACK_SIZE 0x1000

#define PREFIX "VqmbtEvent: "
#define INDENT "            "
_Static_assert(sizeof(PREFIX) == sizeof(INDENT), "INDENT width must match PREFIX");

static SceUID uid_callback = -1;
static SceUID uid_thread = -1;
static bool run_thread = false;

/**
 * TODO.
 *
 * @param event Event details.
 */
static void vqmbt_handle_event(const VqmbtEvent* event) {
    // TODO
    LOG_DEBUG(0, PREFIX "id=0x%08X", event->id);
}

/**
 * TODO.
 *
 * @return Success always.
 */
static int vqmbt_event_callback(int notifyId, int notifyCount, int notifyArg, void* userData) {
    (void)notifyId;
    (void)notifyCount;
    (void)notifyArg;
    (void)userData;

    while (true) {
        VqmbtEvent event = {0};
        int ret;

        // Fetch event data.
        do {
            ret = kvqmbt_read_event(&event);
        } while (ret == VQMBT_ERROR_CB_OVERFLOW);

        // Handle errors.
        if (ret < 0) {
            LOG_ERROR("kvqmbt_read_event returned error: 0x%08X", ret);
            break;
        }

        // Handle no more events to read.
        if (ret == 0) {
            break;
        }

        // Continue in handler.
        vqmbt_handle_event(&event);
    }

    return 0;
}

/**
 * Event thread that creates and registers a callback for bluetooth events.
 *
 * The thread waits for and runs callbacks until run_thread signals it to stop.
 *
 * @return Success always.
 */
static int vqmbt_event_thread(SceSize args, void* argp) {
    (void)args;
    (void)argp;

    // Create callback.
    uid_callback = sceKernelCreateCallback("vqmbt_event_callback", 0, vqmbt_event_callback, NULL);
    LOG_DEBUG(0, "sceKernelCreateCallback returned 0x%08X", uid_callback);

    // Register callback.
    int ret = kvqmbt_register_callback(uid_callback);
    LOG_DEBUG(0, "kvqmbt_register_callback returned 0x%08X", ret);

    // Run until thread is stopped.
    while (run_thread) {
        // TODO switch to sceKernelWaitEventFlagCB?
        sceKernelDelayThreadCB(200 * 1000);  // Callback called in here.
    }

    // Thread is stopping, clean up.
    ret = kvqmbt_unregister_callback(uid_callback);
    LOG_DEBUG(0, "kvqmbt_unregister_callback returned 0x%08X", ret);
    ret = sceKernelDeleteCallback(uid_callback);
    LOG_DEBUG(0, "sceKernelDeleteCallback returned 0x%08X", ret);
    uid_callback = -1;

    return 0;
}

/**
 * Create a thread to handle events from the kernel module and start it.
 *
 * TODO:
 * - Handle sceKernelStartThread error.
 * - Return errors so caller can return non-success.
 */
void vqmbt_event_start(void) {
    if (uid_thread >= 0) {
        return;
    }

    run_thread = true;

    // Create the thread.
    uid_thread = sceKernelCreateThread("vqmbt_event_thread", vqmbt_event_thread, THREAD_PRIORITY, THREAD_STACK_SIZE, 0,
                                       SCE_KERNEL_THREAD_CPU_AFFINITY_MASK_DEFAULT, NULL);
    if (uid_thread < 0) {
        LOG_ERROR("sceKernelCreateThread returned error: 0x%08X", uid_thread);
        return;
    }
    LOG_DEBUG(0, "sceKernelCreateThread returned 0x%08X", uid_thread);

    // Start the thread.
    int ret = sceKernelStartThread(uid_thread, 0, NULL);
    LOG_DEBUG(0, "sceKernelStartThread returned 0x%08X", ret);
}

/**
 * Shut down the running event-handling thread.
 *
 * TODO:
 * - Return errors so caller can return non-success.
 */
void vqmbt_event_stop(void) {
    if (uid_thread < 0) {
        return;
    }

    // Tell the thread to stop.
    run_thread = false;

    // Wait for thread to stop.
    int ret = sceKernelWaitThreadEnd(uid_thread, NULL, NULL);
    LOG_DEBUG(0, "sceKernelWaitThreadEnd returned 0x%08X", ret);

    // Delete the thread.
    ret = sceKernelDeleteThread(uid_thread);
    LOG_DEBUG(0, "sceKernelDeleteThread returned 0x%08X", ret);

    uid_thread = -1;
}
