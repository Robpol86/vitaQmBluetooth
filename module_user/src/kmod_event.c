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
 * - Re-copy bt_event.c here for consistency check.
 * - Optimize, only run the thread when quickmenu is opened.
 */

#include "kmod_event.h"

#include <psp2/kernel/error.h>
#include <psp2/kernel/threadmgr.h>
#include <stdbool.h>

#include "log.h"
#include "vqmbt.h"

#define THREAD_PRIORITY 0x96 /* Higher value = lower priority. */
#define THREAD_STACK_SIZE 0x1000

#define PREFIX "VqmbtEvent: "
#define INDENT "            "
_Static_assert(sizeof(PREFIX) == sizeof(INDENT), "INDENT width must match PREFIX");

static SceUID uid_event_flag = -1;
static SceUID uid_thread = -1;
static bool run_thread = false;

/**
 * Handle scenario where one or more events went missing.
 */
static void handle_event_dropped(void) {
    // TODO
    LOG_DEBUG(0, INDENT "TODO re-run kvqmbt_get_paired_devices()");
}

/**
 * Handler for one event. Called once per bluetooth event.
 *
 * @param event Event details.
 */
static void handle_event(const VqmbtEvent* event) {
    LOG_DEBUG(0, PREFIX "id=0x%08X", event->id);

    // Handle events.
    switch (event->id) {
        case VQMBT_EVENT_DROPPED_EVENTS:
            LOG_DEBUG(0, INDENT "Missing bluetooth events detected");
            handle_event_dropped();
            break;

        case VQMBT_EVENT_BLUETOOTH_ENABLED:
            LOG_DEBUG(0, INDENT "Bluetooth turned on");
            // TODO
            break;

        case VQMBT_EVENT_BLUETOOTH_DISABLED:
            LOG_DEBUG(0, INDENT "Bluetooth turned off");
            // TODO
            break;

        default:
            LOG_WARN(INDENT "Unhandled event id=0x%08X", event->id);
            LOG_DEBUG(0, INDENT "Ignoring id=0x%08X", event->id);
            break;
    }
}

/**
 * TODO.
 */
static void fetch_event(void) {
    while (true) {
        VqmbtEvent event = {0};

        // Fetch event data.
        int ret = kvqmbt_read_event(&event);

        // Handle errors.
        if (ret == VQMBT_ERROR_CB_OVERFLOW) {
            LOG_WARN("kvqmbt_read_event reported dropped events");
            handle_event_dropped();
            break;
        }
        if (ret < 0) {
            LOG_ERROR("kvqmbt_read_event returned error: 0x%08X", ret);
            break;
        }

        // Handle no more events to read.
        if (ret == 0) {
            break;
        }

        // Continue in handler.
        handle_event(&event);
    }
}

/**
 * TODO
 *
 * @return Success always.
 */
static int event_thread(SceSize args, void* argp) {
    (void)args;
    (void)argp;

    // Get event flag.
    uid_event_flag = kvqmbt_get_wrapped_event_flag();
    LOG_DEBUG(0, "kvqmbt_get_event_flag returned 0x%08X", uid_event_flag);
    if (uid_event_flag < 0) {
        LOG_ERROR("BUG");  // TODO handle kernel module not loaded
        return 0;
    }

    // Run until thread is stopped.
    while (run_thread) {
        // Wait for kernel module event.
        unsigned int timeout = 200 * 1000;
        int ret = sceKernelWaitEventFlag(uid_event_flag, 1, SCE_EVENT_WAITOR | SCE_EVENT_WAITCLEAR_PAT, NULL, &timeout);
        if (ret < 0 && ret != SCE_KERNEL_ERROR_WAIT_TIMEOUT) {
            LOG_ERROR("sceKernelWaitEventFlag returned error 0x%08X", ret);
            break;
        }

        // Fetch event.
        fetch_event();
    }

    // Thread is stopping, clean up.
    kvqmbt_unwrap_event_flag();
    uid_event_flag = -1;

    return 0;
}

/**
 * Create a thread to handle events from the kernel module and start it.
 *
 * TODO:
 * - Handle sceKernelStartThread error.
 * - Return errors so caller can return non-success.
 */
void kmod_event_start(void) {
    if (uid_thread >= 0) {
        return;
    }

    run_thread = true;

    // Create the thread.
    uid_thread = sceKernelCreateThread("vqmbt-kmod_event-event_thread", event_thread, THREAD_PRIORITY, THREAD_STACK_SIZE,
                                       0, SCE_KERNEL_THREAD_CPU_AFFINITY_MASK_DEFAULT, NULL);
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
void kmod_event_stop(void) {
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
