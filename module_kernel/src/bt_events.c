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
 * @brief Interface with the bluetooth subsystem. TODO.
 ******************************************************************************/

/***************************************************************************************************************************
# Logs

## Disable bluetooth subsystem from Settings

SceBtEvent: id=0x15 mac0=0x00000000 mac1=0x00000000 unk1=0x00 unk2=0x0000 unk3=0x00000020
SceBtEvent: id=0x06 mac0=0xF26B3406 mac1=0x0000708C unk1=0x16 unk2=0x0000 unk3=0x00000000
SceBtEvent: id=0x15 mac0=0x00000000 mac1=0x00000000 unk1=0x00 unk2=0x0000 unk3=0x00000000

### Enable

SceBtEvent: id=0x15 mac0=0x00000000 mac1=0x00000000 unk1=0x00 unk2=0x0000 unk3=0x00000019
SceBtEvent: id=0x15 mac0=0x00000000 mac1=0x00000000 unk1=0x00 unk2=0x0000 unk3=0x00000009

## Disable bluetooth subsystem from Quick Menu

SceBtEvent: id=0x15 mac0=0x00000000 mac1=0x00000000 unk1=0x00 unk2=0x0000 unk3=0x00000020
SceBtEvent: id=0x15 mac0=0x00000000 mac1=0x00000000 unk1=0x00 unk2=0x0000 unk3=0x00000000

### Enable

SceBtEvent: id=0x15 mac0=0x00000000 mac1=0x00000000 unk1=0x00 unk2=0x0000 unk3=0x00000019
SceBtEvent: id=0x15 mac0=0x00000000 mac1=0x00000000 unk1=0x00 unk2=0x0000 unk3=0x00000009

## Disable bluetooth subsystem:
 *      Called: notifyId=-1 notifyCount=1 notifyArg=0 userData=0x00000000
 *              SceBtEvent: id=0x15 mac0=0x00000000 mac1=0x00000000 unk1=0x00 unk2=0x0000 unk3=0x00000020
 *                          Name: ""
 *      Called: notifyId=-1 notifyCount=1 notifyArg=0 userData=0x00000000
 *              SceBtEvent: id=0x15 mac0=0x00000000 mac1=0x00000000 unk1=0x00 unk2=0x0000 unk3=0x00000000
 *                          Name: ""

 * Enable bluetooth subsystem:
 *      Called: notifyId=-1 notifyCount=1 notifyArg=0 userData=0x00000000
 *              SceBtEvent: id=0x15 mac0=0x00000000 mac1=0x00000000 unk1=0x00 unk2=0x0000 unk3=0x00000019
 *                          Name: ""
 *      Called: notifyId=-1 notifyCount=1 notifyArg=0 userData=0x00000000
 *              SceBtEvent: id=0x15 mac0=0x00000000 mac1=0x00000000 unk1=0x00 unk2=0x0000 unk3=0x00000009
 *                          Name: ""
 * Device add event:
 *      Called: notifyId=-1 notifyCount=2 notifyArg=0 userData=0x00000000
 *              SceBtEvent: id=0x07 mac0=0x00000000 mac1=0x00000000 unk1=0x00 unk2=0x0000 unk3=0x00000000
 *                          Name: ""
 *              SceBtEvent: id=0x02 mac0=0x00000000 mac1=0x00000000 unk1=0x00 unk2=0x0000 unk3=0x00000000
 *                          Name: ""
 *      Called: notifyId=-1 notifyCount=1 notifyArg=0 userData=0x00000000
 *              SceBtEvent: id=0x07 mac0=0x00000000 mac1=0x00000000 unk1=0x00 unk2=0x0000 unk3=0x00000000
 *                          Name: ""
 *      Called: notifyId=-1 notifyCount=1 notifyArg=0 userData=0x00000000
 *              SceBtEvent: id=0x04 mac0=0xF26B3406 mac1=0x0000708C unk1=0x03 unk2=0x0000 unk3=0x000A0D75
 *                          Name: "AirPods Pro"
 *      Called: notifyId=-1 notifyCount=1 notifyArg=0 userData=0x00000000
 *              SceBtEvent: id=0x07 mac0=0x00000000 mac1=0x00000000 unk1=0x00 unk2=0x0000 unk3=0x00000000
 *                          Name: ""
 *      Called: notifyId=-1 notifyCount=1 notifyArg=0 userData=0x00000000
 *              SceBtEvent: id=0x07 mac0=0x00000000 mac1=0x00000000 unk1=0x00 unk2=0x0000 unk3=0x00000000
 *                          Name: ""
 *              SceBtEvent: id=0x05 mac0=0xF26B3406 mac1=0x0000708C unk1=0x00 unk2=0x0000 unk3=0x00000039
 *                          Name: "AirPods Pro"
 *      Called: notifyId=-1 notifyCount=1 notifyArg=0 userData=0x00000000
 *              SceBtEvent: id=0x0E mac0=0xF26B3406 mac1=0x0000708C unk1=0x00 unk2=0x0000 unk3=0x00000066
 *                          Name: "AirPods Pro"
 *      Called: notifyId=-1 notifyCount=1 notifyArg=0 userData=0x00000000
 *              SceBtEvent: id=0x10 mac0=0xF26B3406 mac1=0x0000708C unk1=0x00 unk2=0x0000 unk3=0x00000008
 *                          Name: "AirPods Pro"
 * Device delete event:
 *      SceBtEvent: id=0x07 mac0=0x00000000 mac1=0x00000000 unk1=0x00 unk2=0x0000 unk3=0x00000000
 * Device connect successful events:
 *      NOTE: 0x07 sometimes
 *      Called: notifyId=-1 notifyCount=1 notifyArg=0 userData=0x00000000
 *              SceBtEvent: id=0x02 mac0=0x00000000 mac1=0x00000000 unk1=0x00 unk2=0x0000 unk3=0x00000000
 *                          Name: ""
 *                          Inquiry stop event
 *      Called: notifyId=-1 notifyCount=1 notifyArg=0 userData=0x00000000
 *              SceBtEvent: id=0x05 mac0=0xF26B3406 mac1=0x0000708C unk1=0x00 unk2=0x0000 unk3=0x00000039
 *                          Name: "AirPods Pro"
 *                          connect accepted vid_pid=0000:0000
 *      Called: notifyId=-1 notifyCount=1 notifyArg=0 userData=0x00000000
 *              SceBtEvent: id=0x0E mac0=0xF26B3406 mac1=0x0000708C unk1=0x00 unk2=0x0000 unk3=0x00000066
 *                          Name: "AirPods Pro"
 *                          Unknown event id: 0x0E
 *      Called: notifyId=-1 notifyCount=1 notifyArg=0 userData=0x00000000
 *              SceBtEvent: id=0x10 mac0=0xF26B3406 mac1=0x0000708C unk1=0x00 unk2=0x0000 unk3=0x00000008
 *                          Name: "AirPods Pro"
 *                          Unknown event id: 0x10
 * Device connect timeout events:
 *      Called: notifyId=-1 notifyCount=1 notifyArg=0 userData=0x00000000
 *              SceBtEvent: id=0x02 mac0=0x00000000 mac1=0x00000000 unk1=0x00 unk2=0x0000 unk3=0x00000000
 *                          Name: ""
 *      Called: notifyId=-1 notifyCount=1 notifyArg=0 userData=0x00000000
 *              SceBtEvent: id=0x05 mac0=0xF26B3406 mac1=0x0000708C unk1=0x04 unk2=0x0000 unk3=0x00000000
 *                          Name: "AirPods Pro"
 * Device local disconnect event:
 *      Called: notifyId=-1 notifyCount=1 notifyArg=0 userData=0x00000000
 *              SceBtEvent: id=0x06 mac0=0xF26B3406 mac1=0x0000708C unk1=0x16 unk2=0x0000 unk3=0x00000000
 *                          Name: "AirPods Pro"
 * Device remote disconnect event:
 *      Called: notifyId=-1 notifyCount=1 notifyArg=0 userData=0x00000000
 *              SceBtEvent: id=0x06 mac0=0xF26B3406 mac1=0x0000708C unk1=0x13 unk2=0x0000 unk3=0x00000000
 *                          Name: "AirPods Pro"
****************************************************************************************************************************/

#include <psp2kern/bt.h>
#include <psp2kern/kernel/cpu.h>
#include <psp2kern/kernel/modulemgr.h>
#include <psp2kern/kernel/sysclib.h>
#include <psp2kern/kernel/sysmem.h>
#include <psp2kern/kernel/threadmgr.h>
#include <stdbool.h>

#include "log.h"
#include "vqmbt.h"

static SceUID uid_callback = -1;
static SceUID uid_thread = -1;
static bool run_thread = false;

/**
 * TODO
 *
 * TODO:
 * - Review enum
 * - Significant fields: id, unk1 (event status code?), unk3 (event payload?)
 * - Log all events again with latest logs. Log multiple devices looking for differences in event fields.
 */
static void kvqmbtHandleEvent(const SceBtEvent* event) {
    LOG_DEBUG(0, "SceBtEvent: id=0x%02hhX mac0=0x%08X mac1=0x%08X unk1=0x%02hhX unk2=0x%04hX unk3=0x%08X", event->id,
              event->mac0, event->mac1, event->unk1, event->unk2, event->unk3);

#ifndef NDEBUG
    if (event->mac0 > 0 && event->mac1 > 0) {
        char name[128];
        int ret = ksceBtGetDeviceName(event->mac0, event->mac1, name);
        if (ret == 0) {
            LOG_DEBUG(0, "            Name: \"%s\"", name);
        } else {
            LOG_ERROR("ksceBtGetDeviceName(mac0=0x%08X, mac1=0x%08X) returned error: 0x%08X", event->mac0, event->mac1,
                      ret);
        }
    }
#endif  // NDEBUG

    switch (event->id) {
        case VQMBT_BT_EVENT_ADD_REMOVE_DEVICE:
            LOG_DEBUG(0, "            Device added/removed event");
            break;

        case VQMBT_BT_EVENT_DISCONNECT:
            switch (event->unk1) {
                case 0x13:
                    LOG_DEBUG(0, "            Device disconnected remotely event");
                    break;
                default:
                    LOG_DEBUG(0, "            Device disconnected event");
                    break;
            }
            break;

        default:
            LOG_DEBUG(0, "            Unhandled event id: 0x%02X", event->id);
            break;
    }
}

/**
 * TODO.
 *
 * TODO:
 * - rerun events to collect logging for different times and devices
 * - unsatisfactory timeout events
 */
static int kvqmbtEventCallback(int notifyId, int notifyCount, int notifyArg, void* userData) {
    (void)notifyId;
    (void)notifyCount;
    (void)notifyArg;
    (void)userData;

    while (true) {
        SceBtEvent event = {0};
        int ret;

        // Fetch event data.
        do {
            ret = ksceBtReadEvent(&event, 1);
        } while (ret == SCE_BT_ERROR_CB_OVERFLOW);

        // Handle errors.
        if (ret < 0) {
            LOG_ERROR("ksceBtReadEvent returned error: 0x%08X", ret);
            break;
        }

        // Handle no more events to read.
        if (ret == 0) {
            break;
        }

        // TODO remove v (ginza hotel noise)
        if (event.mac0 == 0x64D34C28 && event.mac1 == 0x0000ACD5) continue;
        if (event.mac0 == 0x6462C838 && event.mac1 == 0x0000ACD5) continue;
        if (event.mac0 == 0x4321847E && event.mac1 == 0x00004023) continue;
        if (event.mac0 == 0x432185BA && event.mac1 == 0x00004023) continue;
        // TODO remove ^

        // Continue in handler.
        kvqmbtHandleEvent(&event);
    }

    return 0;
}

/**
 * TODO
 *
 * TODO:
 * - ksceBtRegisterCallback flags, see if they filter events or macs. Ignore inquiry events.
 */
static int kvqmbtEventThread(SceSize args, void* argp) {
    (void)args;
    (void)argp;

    uid_callback = ksceKernelCreateCallback("kvqmbtEventCallback", 0, kvqmbtEventCallback, NULL);
    LOG_DEBUG(0, "ksceKernelCreateCallback returned 0x%08X", uid_callback);
    // TODO flags1 is probably a mask for SceBtEvent id's. Mask out unused IDs in release builds, keep all in debug.
    // TODO set flags2 to 0?
    int ret = ksceBtRegisterCallback(uid_callback, 0, 0xFFFFFFFF, 0xFFFFFFFF);
    LOG_DEBUG(0, "ksceBtRegisterCallback returned 0x%08X", ret);

    // Sleep until thread is stopped.
    while (run_thread) {
        // TODO switch to event?
        ksceKernelDelayThreadCB(200 * 1000);
    }

    // Thread is stopping, clean up.
    ret = ksceBtUnregisterCallback(uid_callback);
    LOG_DEBUG(0, "ksceBtUnregisterCallback returned 0x%08X", ret);
    ret = ksceKernelDeleteCallback(uid_callback);
    LOG_DEBUG(0, "ksceKernelDeleteCallback returned 0x%08X", ret);
    uid_callback = -1;

    return 0;
}

/**
 * TODO
 *
 * TODO:
 * - priority too low? (lower value == higher priority)
 * - Handle ksceKernelStartThread error.
 * - Return errors so caller can return non-success.
 */
void kvqmbtEventStart(void) {
    if (uid_thread >= 0) {
        return;
    }

    run_thread = true;

    // Create the thread.
    uid_thread = ksceKernelCreateThread("kvqmbtEventThread", kvqmbtEventThread, 0x96, 0x1000, 0,
                                        SCE_KERNEL_THREAD_CPU_AFFINITY_MASK_DEFAULT, NULL);
    if (uid_thread < 0) {
        LOG_ERROR("ksceKernelCreateThread returned error: 0x%08X", uid_thread);
        return;
    }
    LOG_DEBUG(0, "ksceKernelCreateThread returned 0x%08X", uid_thread);

    // Start the thread.
    int ret = ksceKernelStartThread(uid_thread, 0, NULL);
    LOG_DEBUG(0, "ksceKernelStartThread returned 0x%08X", ret);
}

/**
 * TODO
 *
 * TODO:
 * - Return errors so caller can return non-success.
 */
void kvqmbtEventStop(void) {
    if (uid_thread < 0) {
        return;
    }

    // Tell the thread to stop.
    run_thread = false;

    // Wait for thread to stop.
    int ret = ksceKernelWaitThreadEnd(uid_thread, NULL, NULL);
    LOG_DEBUG(0, "ksceKernelWaitThreadEnd returned 0x%08X", ret);

    // Delete the thread.
    ret = ksceKernelDeleteThread(uid_thread);
    LOG_DEBUG(0, "ksceKernelDeleteThread returned 0x%08X", ret);

    uid_thread = -1;
}
