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
 * - Tapping on the notification says "Cannot Find Application"
 * - Move SceNotificationUtil workaround into its own function.
 * - Decompile release binary and confirm no notify instructions compared to main branch.
 */

#include "notify.h"

#include <psp2/sysmodule.h>
#include <psp2/types.h>
#include <stdint.h>
#include <taihen.h>

#include "log.h"

// Library not found: [SceNotificationUtil, ver=1]

#define VQMBT_NOTIFICATIONUTIL_LIB_NID 0xE19097C3
#define VQMBT_NOTIFICATIONUTIL_SEND_NID 0xDE6F33F4
#define NOTIFY_TEXT_BUFFER_BYTES 0x410

typedef SceInt32 (*VqmbtSendNotificationFn)(const SceWChar16* text);

/**
 * TODO
 */
static void utf8_to_utf16(const uint8_t* src, uint16_t* dst) {
    int i;
    for (i = 0; src[i];) {
        if ((src[i] & 0xE0) == 0xE0) {
            *(dst++) = ((src[i] & 0x0F) << 12) | ((src[i + 1] & 0x3F) << 6) | (src[i + 2] & 0x3F);
            i += 3;
        } else if ((src[i] & 0xC0) == 0xC0) {
            *(dst++) = ((src[i] & 0x1F) << 6) | (src[i + 1] & 0x3F);
            i += 2;
        } else {
            *(dst++) = src[i];
            i += 1;
        }
    }

    *dst = '\0';
}

/**
 * TODO
 */
void notify_if_debug(void) {
#ifdef NDEBUG
    return;
#endif
    int ret = sceSysmoduleLoadModule(SCE_SYSMODULE_NOTIFICATION_UTIL);
    if (ret < 0) {
        LOG_ERROR("sceSysmoduleLoadModule returned error 0x%08X", ret);
        return;
    }
    LOG_DEBUG(0, "sceSysmoduleLoadModule returned 0x%08X", ret);

    uintptr_t send_addr = 0;
    ret = taiGetModuleExportFunc("SceNotificationUtil", VQMBT_NOTIFICATIONUTIL_LIB_NID, VQMBT_NOTIFICATIONUTIL_SEND_NID,
                                 &send_addr);
    if (ret < 0) {
        LOG_ERROR("taiGetModuleExportFunc returned error 0x%08X", ret);
        return;
    }
    LOG_DEBUG(0, "taiGetModuleExportFunc returned 0x%08X", ret);

    static SceWChar16 text[NOTIFY_TEXT_BUFFER_BYTES / sizeof(SceWChar16)];
    sceClibMemset(text, 0, sizeof(text));
    utf8_to_utf16((const uint8_t*)"Hello World", text);

    ret = ((VqmbtSendNotificationFn)send_addr)(text);
    LOG_DEBUG(0, "VqmbtSendNotificationFn returned 0x%08X", ret);
}
