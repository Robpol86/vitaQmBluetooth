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
 * - In module_load mkdir ux0:/data/vitaQmBluetooth/logs
 * - delete logs older than 7 days in module_load; NO, delete old logs if count > 7 including today.
 * - Handle error: dir is file
 * - https://github.com/Ibrahim778/QuickMenuReborn/blob/vdsuite/src/user/main.cpp
 * - Log file name format: vitaQmBluetooth-YYYYMMDD.log
 * - Support timezone change (filename change).
 * - Support uma0.
 */

#include <psp2kern/kernel/debug.h>
#include <stdarg.h>

/**
 * TODO
 */
void logfile_write_line(const char* line, ...) {
    va_list ap;
    va_start(ap, line);
    ksceKernelVprintf(line, ap);
    va_end(ap);
}
