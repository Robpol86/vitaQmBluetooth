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

/**
 * Macro that logs an error message.
 *
 * @param fmtMsg The log message including any format specifiers.
 * @param ... Arguments for the format specifiers.
 */
#define LOG_ERROR(fmtMsg, ...) LOG_MESSAGE_(0, "ERROR", fmtMsg, ##__VA_ARGS__)

#ifndef NDEBUG
/**
 * Macro that logs a debug message.
 *
 * If log messages overwrite each other try setting the delay to 50000.
 *
 * @param delay Delay the thread for these many microseconds if >0 (mitigates clobbering).
 * @param fmtMsg The log message including any format specifiers.
 * @param ... Arguments for the format specifiers.
 */
#define LOG_DEBUG(delay, fmtMsg, ...) LOG_MESSAGE_(delay, "DEBUG", fmtMsg, ##__VA_ARGS__)
#else
#define LOG_DEBUG(delay, fmtMsg, ...) LOG_MESSAGE_NOOP_(delay, fmtMsg, ##__VA_ARGS__)
#endif  // NDEBUG

#endif  // LOG_H
