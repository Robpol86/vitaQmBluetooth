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
 * @brief Macros for logging.
 ******************************************************************************/

#ifndef LOG_H
#define LOG_H

#ifndef NDEBUG
#include <psp2kern/kernel/debug.h>
#include <psp2kern/kernel/rtc.h>
#endif  // NDEBUG

#ifndef NDEBUG
/**
 * Macro that logs a debug message.
 *
 * @param fmtMsg The log message including any format specifiers.
 * @param ... Arguments for the format specifiers.
 */
#define LOG_DEBUG(fmtMsg, ...)                                                                                  \
    do {                                                                                                        \
        SceDateTime _time;                                                                                      \
        ksceRtcGetCurrentClockLocalTime(&_time);                                                                \
        ksceKernelPrintf("[%02d:%02d:%02d.%03d] [DEBUG] [" MODULE_NAME "] [%s:%d:%s] " fmtMsg "\n", _time.hour, \
                         _time.minute, _time.second, _time.microsecond / 1000, __FILE__, __LINE__, __func__,    \
                         ##__VA_ARGS__);                                                                        \
    } while (0)
#else
/**
 * No-op in release builds.
 */
#define LOG_DEBUG(fmtMsg, ...) \
    do {                       \
    } while (0)
#endif  // NDEBUG

#endif  // LOG_H
