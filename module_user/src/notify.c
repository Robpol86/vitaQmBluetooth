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

#include "notify.h"

#include <psp2/notificationutil.h>

#include "log.h"

/**
 * TODO remove.
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
    char text_buf[SCE_NOTIFICATIONUTIL_TEXT_MAX * 2];
    sceClibSnprintf(text_buf, sizeof(text_buf), "Hello World");

    SceNotificationUtilProgressInitParam param;
    sceClibMemset(&param, 0, sizeof(SceNotificationUtilProgressInitParam));
    utf8_to_utf16((uint8_t*)text_buf, param.notificationText);

    SceInt32 ret = sceNotificationUtilSendNotification(param.notificationText);
    LOG_DEBUG(0, "sceNotificationUtilSendNotification returned 0x%08X", ret);
}
