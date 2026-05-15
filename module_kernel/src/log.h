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
 * @brief Macros for logging. TODO
 ******************************************************************************/

#ifndef LOG_H_MODULE
#define LOG_H_MODULE

#include <psp2kern/kernel/debug.h>
#include <psp2kern/kernel/rtc.h>
#include <psp2kern/kernel/threadmgr.h>

#define LOG_MESSAGE_(delay, category, fmtMsg, ...)                                                                     \
    do {                                                                                                               \
        SceDateTime _time;                                                                                             \
        ksceRtcGetCurrentClockLocalTime(&_time);                                                                       \
        ksceKernelPrintf("[%02d:%02d:%02d.%03d] [" category "] [" MODULE_NAME "] [%s:%d:%s] " fmtMsg "\n", _time.hour, \
                         _time.minute, _time.second, _time.microsecond / 1000, __FILE__, __LINE__, __func__,           \
                         ##__VA_ARGS__);                                                                               \
        if (!__builtin_constant_p(delay) || (delay) > 0) ksceKernelDelayThread((delay));                               \
    } while (0)

#define LOG_MESSAGE_NOOP_(delay, fmtMsg, ...)           \
    do {                                                \
        (void)(delay);                                  \
        if (0) ksceKernelPrintf(fmtMsg, ##__VA_ARGS__); \
    } while (0)

#include <log.h>  // TODO explain

#endif  // LOG_H_MODULE
