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
 * @brief Module-agnostic macros for logging.
 ******************************************************************************/

#ifndef LOG_H
#define LOG_H

#define LOG_COLOR_RESET_ "\033[0m"
#define LOG_COLOR_RED_ "\033[31m"

#define LOG_FORMAT_(category) "[%02d:%02d:%02d.%03d] [" category "] [" MODULE_NAME "] [%s:%d:%s] "
#define LOG_FORMAT_VALUES_ dt_.hour, dt_.minute, dt_.second, dt_.microsecond / 1000, __FILE__, __LINE__, __func__

/**
 * Macro that logs an error message.
 *
 * @param msg The log message (supports %s and other format specifies).
 * @param ... Arguments for the format specifiers.
 */
#define LOG_ERROR(msg, ...) LOG_MESSAGE_(0, LOG_FORMAT_(LOG_COLOR_RED_ "ERROR" LOG_COLOR_RESET_), msg, ##__VA_ARGS__)

#ifndef NDEBUG
/**
 * Macro that logs a debug message.
 *
 * If log messages overwrite each other try setting the delay to 50000.
 *
 * @param delay Delay the thread for these many microseconds if >0 (mitigates clobbering).
 * @param msg The log message (supports %s and other format specifies).
 * @param ... Arguments for the format specifiers.
 */
#define LOG_DEBUG(delay, msg, ...) LOG_MESSAGE_(delay, LOG_FORMAT_("DEBUG"), msg, ##__VA_ARGS__)
#else
#define LOG_DEBUG(delay, msg, ...) LOG_MESSAGE_NOOP_(delay, "", msg, ##__VA_ARGS__)
#endif  // NDEBUG

#endif  // LOG_H
