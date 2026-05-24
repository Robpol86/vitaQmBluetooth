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

/**
 * TODO:
 * - In module_load mkdir ux0:/data/vitaQmBluetooth/logs
 * - If low disk space disable file logging and show error in notifications or dialog (custom error code?).
 * - Performant to touch file on boot intead of on log message? What about date change in the middle of logging?
 * - Log file name format: vitaQmBluetooth-YYYYMMDD.log
 * - logfile.h
 * - no colors in logfile
 * - make fetched-logs tail-last
 * - make fetched-screenshots
 * - delete logs older than 7 days in module_load
 * - https://github.com/Ibrahim778/QuickMenuReborn/blob/vdsuite/src/user/main.cpp
 */

#ifndef LOGFILE_H
#define LOGFILE_H

#define VQMBT_LOG_ROOT1 "ux0:"
#define VQMBT_LOG_ROOT2 "uma0:"
#define VQMBT_LOG_DIR1 VQMBT_LOG_ROOT1 PROJECT_NAME "/logs/"
#define VQMBT_LOG_DIR2 VQMBT_LOG_ROOT2 PROJECT_NAME "/logs/"

#endif  // LOGFILE_H
