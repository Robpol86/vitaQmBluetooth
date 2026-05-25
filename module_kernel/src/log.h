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

#include <psp2kern/kernel/debug.h>
#include <psp2kern/kernel/rtc.h>
#include <psp2kern/kernel/threadmgr.h>

#include "logfile.h"  // IWYU pragma: keep

/**
 * Macro to write log message to log file only in debug builds.
 */
#ifndef NDEBUG
#define LOG_MESSAGE_TO_FILE_(fmt, ...) logfile_write_line(fmt, ##__VA_ARGS__)
#else
#define LOG_MESSAGE_TO_FILE_(fmt, ...) ((void)0)
#endif  // NDEBUG

/**
 * Main macro that handles getting the current time, logging to stdout, logging
 * to log file via LOG_MESSAGE_TO_FILE_ macro, then optionally sleeping the thread.
 */
#define LOG_MESSAGE_(delay, fmt, msg, ...)                                               \
    do {                                                                                 \
        SceDateTime dt_;                                                                 \
        ksceRtcGetCurrentClockLocalTime(&dt_);                                           \
        ksceKernelPrintf(fmt msg "\n", LOG_FORMAT_VALUES_(dt_), ##__VA_ARGS__);          \
        LOG_MESSAGE_TO_FILE_(fmt msg "\n", LOG_FORMAT_VALUES_(dt_), ##__VA_ARGS__);      \
        if (!__builtin_constant_p(delay) || (delay) > 0) ksceKernelDelayThread((delay)); \
    } while (0)

/**
 * Macro that does nothing, used by include/log.h when logging should be disabled.
 */
#define LOG_MESSAGE_NOOP_(delay, fmt, msg, ...)          \
    do {                                                 \
        (void)(delay);                                   \
        if (0) ksceKernelPrintf(fmt msg, ##__VA_ARGS__); \
    } while (0)

#include <log.h>  // Bring in LOG_DEBUG, LOG_ERROR, and other common macros.

#endif  // LOG_H_MODULE
