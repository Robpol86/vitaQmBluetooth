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

#include <psp2kern/bt.h>
#include <psp2kern/kernel/cpu.h>
#include <psp2kern/kernel/modulemgr.h>
#include <psp2kern/kernel/sysclib.h>
#include <psp2kern/kernel/sysmem.h>
#include <psp2kern/kernel/threadmgr.h>
#include <stdbool.h>

#include "log.h"

static SceUID uid_callback = -1;
static SceUID uid_thread = -1;

/**
 * TODO.
 */
static int kvqmbtEventCallback(int notifyId, int notifyCount, int notifyArg, void* userData) {
    (void)notifyId;
    (void)notifyCount;
    (void)notifyArg;
    (void)userData;

    LOG_DEBUG(0, "Called with notifyId=%d notifyCount=%d notifyArg=%d userData=%p", notifyId, notifyCount, notifyArg,
              userData);

    // static SceBtHidRequest hid_request;
    static unsigned char recv_buff[0x100];

    while (1) {
        int ret;
        SceBtEvent hid_event;

        memset(&hid_event, 0, sizeof(hid_event));

        do {
            ret = ksceBtReadEvent(&hid_event, 1);
        } while (ret == SCE_BT_ERROR_CB_OVERFLOW);

        if (ret <= 0) {
            break;
        }

        LOG_DEBUG(0, "->Event:");
        for (int i = 0; i < 0x10; i++) LOG_DEBUG(0, " %02X", hid_event.data[i]);

        LOG_DEBUG(0, "hid_event.mac0=%08X hid_event.mac1=%08X", hid_event.mac0, hid_event.mac1);

        switch (hid_event.id) {
            case 0x01: { /* Inquiry result event */
                unsigned short vid_pid[2];
                ksceBtGetVidPid(hid_event.mac0, hid_event.mac1, vid_pid);
                LOG_DEBUG(0, "inquiry vid_pid=%04X:%04X", vid_pid[0], vid_pid[1]);
                break;
            }

            case 0x02: /* Inquiry stop event */
                LOG_DEBUG(0, "Inquiry stop event");
                break;

            case 0x04: /* Link key request? event */
                LOG_DEBUG(0, "link key request event");
                ksceBtReplyUserConfirmation(hid_event.mac0, hid_event.mac1, 1);
                break;

            case 0x05: { /* Connection accepted event */
                unsigned short vid_pid[2];
                ksceBtGetVidPid(hid_event.mac0, hid_event.mac1, vid_pid);
                LOG_DEBUG(0, "connect accepted vid_pid=%04X:%04X", vid_pid[0], vid_pid[1]);
                break;
            }

            case 0x06: /* Device disconnect event*/
                LOG_DEBUG(0, "device disconnect event");
                break;

            case 0x08: /* Connection requested event */
                /*
                 * Do nothing since we will get a 0x05 event afterwards.
                 */
                LOG_DEBUG(0, "connection requested event");
                break;

            case 0x09: /* Connection request without being paired? event */
                /*
                 * The Vita needs to have a pairing with the DS4,
                 * otherwise it won't connect.
                 */
                LOG_DEBUG(0, "connection request without being paired event");
                break;

            case 0x0A: /* HID reply to 0-type request */

                LOG_DEBUG(0, "DS4 0x0A event: 0x%02X", recv_buff[0]);

                switch (recv_buff[0]) {
                    case 0x11:
                        LOG_DEBUG(0, "DS4 0x11 event: battery level %d%%", recv_buff[1]);
                        break;

                    default:
                        LOG_DEBUG(0, "Unknown DS4 event: 0x%02X", recv_buff[0]);
                        break;
                }

                break;

            case 0x0B: /* HID reply to 1-type request */
                LOG_DEBUG(0, "DS4 0x0B event: 0x%02X", recv_buff[0]);
                break;

            default:
                LOG_DEBUG(0, "Unknown event id: 0x%02X", hid_event.id);
                break;
        }
    }

    return 0;
}

/**
 * TODO
 */
static int kvqmbtEventThread(SceSize args, void* argp) {
    uid_callback = ksceKernelCreateCallback("kvqmbtEventCallback", 0, kvqmbtEventCallback, NULL);
    LOG_DEBUG(0, "ksceKernelCreateCallback() returned 0x%08X", uid_callback);
    int ret = ksceBtRegisterCallback(uid_callback, 0, 0xFFFFFFFF, 0xFFFFFFFF);
    LOG_DEBUG(0, "ksceBtRegisterCallback() returned 0x%08X", ret);

    // Sleep indefinitely.
    while (true) {                            // TODO check variable instead
        ksceKernelDelayThreadCB(200 * 1000);  // TODO switch to event?
    }

    return 0;
}

/**
 * TODO
 */
void kvqmbtEventStart(void) {
    // TODO priority too low (inverted)?
    uid_thread = ksceKernelCreateThread("kvqmbtEventThread", kvqmbtEventThread, 0x96, 0x1000, 0,
                                        SCE_KERNEL_THREAD_CPU_AFFINITY_MASK_DEFAULT, NULL);
    LOG_DEBUG(0, "ksceKernelCreateThread() returned 0x%08X", uid_thread);
    int ret = ksceKernelStartThread(uid_thread, 0, NULL);
    LOG_DEBUG(0, "ksceKernelStartThread() returned 0x%08X", ret);
}

/**
 * TODO
 */
void kvqmbtEventStop(void) {
    // TODO
    /*
    ksceBtUnregisterCallback(uid_callback);
    ksceKernelDeleteCallback(uid_callback);
    */
}
