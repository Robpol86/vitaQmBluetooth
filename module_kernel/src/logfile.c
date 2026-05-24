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

#include "logfile.h"

#include <psp2kern/io/stat.h>

// static char logfile_path_[256] = {0};

/**
 * TODO.
 *
 * TODO:
 * - In module_load mkdir ux0:/data/vitaQmBluetooth/logs
 * - delete logs older than 7 days in module_load
 * - Handle error: dir is file
 * - https://github.com/Ibrahim778/QuickMenuReborn/blob/vdsuite/src/user/main.cpp
 * - Log file name format: vitaQmBluetooth-YYYYMMDD.log
 * - Support timezone change (filename change).
 * - Support uma0.
 */
void logfile_init(void) {
    // Create directories.
    ksceIoMkdir(LOGFILE_PROJECT1_, 0777);  // TODO better permissions
    ksceIoMkdir(LOGFILE_DIR1_, 0777);

    // logfile_path_ = LOGFILE_DIR1_ "/" LOGFILE_NAME_FORMAT_;

    // TODO
}
