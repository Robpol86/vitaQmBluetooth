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
 * - static int increment every write. Every n write check free space and disable. Check again after n and re-enable if
 *   enough space.
 * - Performant to touch file on boot intead of on log message? What about date change in the middle of logging?
 * - Log file name format: vitaQmBluetooth-YYYYMMDD.log
 * - logfile.h
 * - no colors in logfile
 * - make fetched-logs tail-last
 * - make fetched-screenshots
 * - delete logs older than 7 days in module_load
 * - https://github.com/Ibrahim778/QuickMenuReborn/blob/vdsuite/src/user/main.cpp
 * - Support timezone change (filename change).
 * - Handle error: dir is file
 */

#ifndef LOGFILE_H
#define LOGFILE_H

#define VQMBT_LOGFILE_ROOT1 "ux0:"
#define VQMBT_LOGFILE_ROOT2 "uma0:"
#define VQMBT_LOGFILE_PARENT1 VQMBT_LOGFILE_ROOT1 PROJECT_NAME
#define VQMBT_LOGFILE_PARENT2 VQMBT_LOGFILE_ROOT2 PROJECT_NAME
#define VQMBT_LOGFILE_DIR1 VQMBT_LOGFILE_PARENT1 "/logs/"
#define VQMBT_LOGFILE_DIR2 VQMBT_LOGFILE_PARENT2 "/logs/"

#define VQMBT_LOGFILE_NAME PROJECT_NAME ".log"

#endif  // LOGFILE_H
