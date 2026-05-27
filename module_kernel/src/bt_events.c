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

## Enable/Disable/Add(Pair)/Remove #######################################################

### Disable bluetooth subsystem from Settings

SceBtEvent: id=0x15 unk1=0x00 unk3=0x00000020 mac0=0x00000000 mac1=0x00000000 unk2=0x0000
SceBtEvent: id=0x15 unk1=0x00 unk3=0x00000000 mac0=0x00000000 mac1=0x00000000 unk2=0x0000

#### Enable

SceBtEvent: id=0x15 unk1=0x00 unk3=0x00000019 mac0=0x00000000 mac1=0x00000000 unk2=0x0000
SceBtEvent: id=0x15 unk1=0x00 unk3=0x00000009 mac0=0x00000000 mac1=0x00000000 unk2=0x0000

### Disable bluetooth subsystem from Quick Menu

SceBtEvent: id=0x15 unk1=0x00 unk3=0x00000020 mac0=0x00000000 mac1=0x00000000 unk2=0x0000
SceBtEvent: id=0x06 unk1=0x16 unk3=0x00000000 mac0=0xF26B3406 mac1=0x0000708C unk2=0x0000
            Name: "AirPods Pro"
SceBtEvent: id=0x15 unk1=0x00 unk3=0x00000000 mac0=0x00000000 mac1=0x00000000 unk2=0x0000

#### Enable

SceBtEvent: id=0x15 unk1=0x00 unk3=0x00000019 mac0=0x00000000 mac1=0x00000000 unk2=0x0000
SceBtEvent: id=0x15 unk1=0x00 unk3=0x00000009 mac0=0x00000000 mac1=0x00000000 unk2=0x0000

### Delete APP2

SceBtEvent: id=0x07 unk1=0x00 unk3=0x00000000 mac0=0x00000000 mac1=0x00000000 unk2=0x0000

### Add APP2 cancelled

SceBtEvent: id=0x02 unk1=0x00 unk3=0x00000000 mac0=0x00000000 mac1=0x00000000 unk2=0x0000
SceBtEvent: id=0x07 unk1=0x00 unk3=0x00000000 mac0=0x00000000 mac1=0x00000000 unk2=0x0000
SceBtEvent: id=0x04 unk1=0x03 unk3=0x000D55A6 mac0=0xF26B3406 mac1=0x0000708C unk2=0x0000
            Name: "AirPods Pro"
*TAPPED CANCEL*
SceBtEvent: id=0x05 unk1=0x05 unk3=0x00000000 mac0=0xF26B3406 mac1=0x0000708C unk2=0x0000
            Name: "AirPods Pro"
SceBtEvent: id=0x06 unk1=0x16 unk3=0x00000000 mac0=0xF26B3406 mac1=0x0000708C unk2=0x0000
            Name: "AirPods Pro"

### Add APP2 success

SceBtEvent: id=0x02 unk1=0x00 unk3=0x00000000 mac0=0x00000000 mac1=0x00000000 unk2=0x0000
SceBtEvent: id=0x04 unk1=0x03 unk3=0x0007AC81 mac0=0xF26B3406 mac1=0x0000708C unk2=0x0000
            Name: "AirPods Pro"
*Confirm with user*
SceBtEvent: id=0x07 unk1=0x00 unk3=0x00000000 mac0=0x00000000 mac1=0x00000000 unk2=0x0000
SceBtEvent: id=0x07 unk1=0x00 unk3=0x00000000 mac0=0x00000000 mac1=0x00000000 unk2=0x0000
SceBtEvent: id=0x05 unk1=0x00 unk3=0x00000039 mac0=0xF26B3406 mac1=0x0000708C unk2=0x0000
            Name: "AirPods Pro"
SceBtEvent: id=0x0E unk1=0x00 unk3=0x00000066 mac0=0xF26B3406 mac1=0x0000708C unk2=0x0000
            Name: "AirPods Pro"
SceBtEvent: id=0x10 unk1=0x00 unk3=0x00000008 mac0=0xF26B3406 mac1=0x0000708C unk2=0x0000
            Name: "AirPods Pro"

### Add APP2 failed

SceBtEvent: id=0x02 unk1=0x00 unk3=0x00000000 mac0=0x00000000 mac1=0x00000000 unk2=0x0000
*Failed immediately with "An error has occurred"*
SceBtEvent: id=0x06 unk1=0x14 unk3=0x00000000 mac0=0xF26B3406 mac1=0x0000708C unk2=0x0000
            Name: "AirPods Pro"

### Add APP2 timeout

SceBtEvent: id=0x02 unk1=0x00 unk3=0x00000000 mac0=0x00000000 mac1=0x00000000 unk2=0x0000
*WAITING... TIMED OUT*
SceBtEvent: id=0x05 unk1=0x04 unk3=0x00000000 mac0=0xF26B3406 mac1=0x0000708C unk2=0x0000
            Name: "AirPods Pro"

## Connect/Disconnect ####################################################################

### Connect Airpods Pro 2 successfully

SceBtEvent: id=0x07 unk1=0x00 unk3=0x00000000 mac0=0x00000000 mac1=0x00000000 unk2=0x0000  (sometimes)
SceBtEvent: id=0x02 unk1=0x00 unk3=0x00000000 mac0=0x00000000 mac1=0x00000000 unk2=0x0000
SceBtEvent: id=0x05 unk1=0x00 unk3=0x00000039 mac0=0xF26B3406 mac1=0x0000708C unk2=0x0000
            Name: "AirPods Pro"
SceBtEvent: id=0x0E unk1=0x00 unk3=0x00000066 mac0=0xF26B3406 mac1=0x0000708C unk2=0x0000
            Name: "AirPods Pro"
SceBtEvent: id=0x10 unk1=0x00 unk3=0x00000008 mac0=0xF26B3406 mac1=0x0000708C unk2=0x0000
            Name: "AirPods Pro"

#### Disconnect by user

SceBtEvent: id=0x06 unk1=0x16 unk3=0x00000000 mac0=0xF26B3406 mac1=0x0000708C unk2=0x0000
            Name: "AirPods Pro"

#### Disconnect by closing case

SceBtEvent: id=0x06 unk1=0x13 unk3=0x00000000 mac0=0xF26B3406 mac1=0x0000708C unk2=0x0000
            Name: "AirPods Pro"

### Connect APP2 timeout

SceBtEvent: id=0x07 unk1=0x00 unk3=0x00000000 mac0=0x00000000 mac1=0x00000000 unk2=0x0000
SceBtEvent: id=0x02 unk1=0x00 unk3=0x00000000 mac0=0x00000000 mac1=0x00000000 unk2=0x0000
SceBtEvent: id=0x06 unk1=0x94 unk3=0x00000000 mac0=0xF26B3406 mac1=0x0000708C unk2=0x0000
            Name: "AirPods Pro"

#### Another sample

SceBtEvent: id=0x07 unk1=0x00 unk3=0x00000000 mac0=0x00000000 mac1=0x00000000 unk2=0x0000
            Device added/removed/connecting
*WAITING... TIMED OUT*
SceBtEvent: id=0x05 unk1=0x04 unk3=0x00000000 mac0=0xF26B3406 mac1=0x0000708C unk2=0x0000
            Name: "AirPods Pro"
            Unhandled event id=0x05

### Connect bad state hangs

TODO

****************************************************************************************************************************/

#include <psp2kern/bt.h>
#include <psp2kern/kernel/cpu.h>
#include <psp2kern/kernel/modulemgr.h>
#include <psp2kern/kernel/sysclib.h>
#include <psp2kern/kernel/sysmem.h>
#include <psp2kern/kernel/threadmgr.h>
#include <stdbool.h>

#include "log.h"

#define PREFIX "SceBtEvent: "
#define INDENT "            "
_Static_assert(sizeof(PREFIX) == sizeof(INDENT), "INDENT width must match PREFIX");

static SceUID uid_callback = -1;
static SceUID uid_thread = -1;
static bool run_thread = false;

typedef enum VqmbtBtEventId {
    VQMBT_BT_EVENT_INQUIRY_RESULT = 0x01,
    VQMBT_BT_EVENT_INQUIRY_STOP = 0x02,
    VQMBT_BT_EVENT_PAIRING_REQUEST = 0x04,
    VQMBT_BT_EVENT_CONNECT_RESULT = 0x05,
    VQMBT_BT_EVENT_DISCONNECT = 0x06,
    VQMBT_BT_EVENT_ADD_REMOVE_CONNECTING_DEVICE = 0x07,
    VQMBT_BT_EVENT_TOGGLE_BLUETOOTH = 0x15,
    // AI
    VQMBT_BT_EVENT_CONNECT_REQUESTED = 0x08,
    VQMBT_BT_EVENT_CONNECT_UNPAIRED = 0x09,
    VQMBT_BT_EVENT_HID_REPLY_TYPE0 = 0x0A,
    VQMBT_BT_EVENT_HID_REPLY_TYPE1 = 0x0B,
} VqmbtBtEventId;

/**
 * TODO
 *
 * TODO:
 * - Review enum
 * - Significant fields: id, unk1 (event status code?), unk3 (event payload?)
 * - Log all events again with latest logs. Log multiple devices looking for differences in event fields.
 * - Test with ovaltine
 * - Test with iphone
 * - Test with ds3
 */
static void kvqmbtHandleEvent(const SceBtEvent* event) {
    LOG_DEBUG(0, PREFIX "id=0x%02X unk1=0x%02X unk3=0x%08X mac0=0x%08X mac1=0x%08X unk2=0x%04X", event->id, event->unk1,
              event->unk3, event->mac0, event->mac1, event->unk2);

    // Log device name in debug builds.
#ifndef NDEBUG
    if (event->mac0 > 0 && event->mac1 > 0) {
        char name[128];
        int ret = ksceBtGetDeviceName(event->mac0, event->mac1, name);
        if (ret == 0) {
            LOG_DEBUG(0, INDENT "Name: \"%s\"", name);
        } else {
            LOG_ERROR("ksceBtGetDeviceName(mac0=0x%08X, mac1=0x%08X) returned error: 0x%08X", event->mac0, event->mac1,
                      ret);
        }
    }
#endif  // NDEBUG

    // Handle events.
    switch (event->id) {
        case VQMBT_BT_EVENT_CONNECT_RESULT:
            switch (event->unk1) {
                case 0x00:
                    LOG_DEBUG(0, INDENT "Device connected");
                    break;
                case 0x04:
                    LOG_DEBUG(0, INDENT "Device connect failed");
                    break;
                case 0x05:
                    LOG_DEBUG(0, INDENT "Device connect cancelled by host");
                    break;
                default:
                    LOG_DEBUG(0, INDENT "Unhandled connect result event unk1=0x%02X", event->unk1);
                    break;
            }
            break;

        case VQMBT_BT_EVENT_DISCONNECT:
            switch (event->unk1) {
                case 0x13:
                    LOG_DEBUG(0, INDENT "Device disconnected remotely");
                    break;
                case 0x16:
                    LOG_DEBUG(0, INDENT "Device disconnected by host");
                    break;
                default:
                    LOG_DEBUG(0, INDENT "Unhandled disconnect event unk1=0x%02X", event->unk1);
                    break;
            }
            break;

        case VQMBT_BT_EVENT_ADD_REMOVE_CONNECTING_DEVICE:
            LOG_DEBUG(0, INDENT "Device added/removed/connecting");
            break;

        case VQMBT_BT_EVENT_TOGGLE_BLUETOOTH:
            switch (event->unk3) {
                case 0x00:
                    // Ignore
                    break;
                case 0x09:
                    LOG_DEBUG(0, INDENT "Bluetooth turned on");
                    break;
                case 0x19:
                    // Ignore
                    break;
                case 0x20:
                    LOG_DEBUG(0, INDENT "Bluetooth turned off");
                    break;
                default:
                    LOG_DEBUG(0, INDENT "Unhandled toggle bluetooth event unk3=0x%08X", event->unk3);
                    break;
            }
            break;

        default:
            LOG_DEBUG(0, INDENT "Unhandled event id=0x%02X", event->id);
            break;
    }
}

/**
 * TODO.
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

        // Continue in handler.
        kvqmbtHandleEvent(&event);
    }

    return 0;
}

/**
 * TODO
 *
 * Note: set id_mask to 0xFFFFFFFF to receive and log all events.
 *
 * TODO:
 * - ksceBtRegisterCallback flags, see if they filter events or macs. Ignore inquiry events.
 */
static int kvqmbtEventThread(SceSize args, void* argp) {
    (void)args;
    (void)argp;

    // Create callback.
    uid_callback = ksceKernelCreateCallback("kvqmbtEventCallback", 0, kvqmbtEventCallback, NULL);
    LOG_DEBUG(0, "ksceKernelCreateCallback returned 0x%08X", uid_callback);

    // Register callback.
    const unsigned int id_mask =
        0xFFFFFFFFU & ~((1U << VQMBT_BT_EVENT_INQUIRY_RESULT) | (1U << VQMBT_BT_EVENT_INQUIRY_STOP) |
                        (1U << VQMBT_BT_EVENT_PAIRING_REQUEST));             // TODO make inclusive instead
    int ret = ksceBtRegisterCallback(uid_callback, 0, id_mask, 0xFFFFFFFF);  // TODO test flags2 0x0
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
