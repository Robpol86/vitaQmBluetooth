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

#include <psp2kern/io/stat.h>
#include <psp2kern/kernel/debug.h>
#include <stdarg.h>
#include <stdbool.h>

#include "log.h"

#define SCE_ERROR_ERRNO_EEXIST 0x80010011

#define LOG_DIR_PARENT_ "ux0:" PROJECT_NAME
#define LOG_DIR_ LOG_DIR_PARENT_ "/logs/"

static bool is_initialized = false;

/**
 * TODO
 */
void logfile_init(void) {
    int ret;

    // Create log directories.
    ret = ksceIoMkdir(LOG_DIR_PARENT_, 0777);
    if (ret < 0 && ret != SCE_ERROR_ERRNO_EEXIST) {
        LOG_ERROR("ksceIoMkdir(log_dir=\"%s\") returned error: 0x%08X", LOG_DIR_PARENT_, ret);
        return;
    }
    ret = ksceIoMkdir(LOG_DIR_, 0777);
    if (ret < 0 && ret != SCE_ERROR_ERRNO_EEXIST) {
        LOG_ERROR("ksceIoMkdir(log_dir=\"%s\") returned error: 0x%08X", LOG_DIR_, ret);
        return;
    }

    is_initialized = true;
}

/**
 * TODO
 */
void logfile_write_line(const char* line, ...) {
    if (!is_initialized) return;
    va_list ap;
    va_start(ap, line);
    ksceKernelVprintf(line, ap);
    va_end(ap);
}
