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
 * @brief Module-specific macros for logging.
 ******************************************************************************/

#ifndef LOG_H_MODULE
#define LOG_H_MODULE

#include <psp2/kernel/clib.h>
#include <psp2/kernel/threadmgr.h>
#include <psp2/rtc.h>

#define LOG_MESSAGE_(delay, category, fmtMsg, ...)                                                                  \
    do {                                                                                                            \
        SceDateTime _time;                                                                                          \
        sceRtcGetCurrentClockLocalTime(&_time);                                                                     \
        sceClibPrintf("[%02d:%02d:%02d.%03d] [" category "] [" MODULE_NAME "] [%s:%d:%s] " fmtMsg "\n", _time.hour, \
                      _time.minute, _time.second, _time.microsecond / 1000, __FILE__, __LINE__, __func__,           \
                      ##__VA_ARGS__);                                                                               \
        if (!__builtin_constant_p(delay) || (delay) > 0) sceKernelDelayThread((delay));                             \
    } while (0)

#define LOG_MESSAGE_NOOP_(delay, fmtMsg, ...)        \
    do {                                             \
        (void)(delay);                               \
        if (0) sceClibPrintf(fmtMsg, ##__VA_ARGS__); \
    } while (0)

#include <log.h>  // Bring in LOG_DEBUG, LOG_ERROR, and other common macros.

#endif  // LOG_H_MODULE
