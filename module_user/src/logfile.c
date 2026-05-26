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

#include <psp2/kernel/clib.h>
#include <stdarg.h>

/**
 * TODO
 */
void logfile_init(void) {
    // TODO mkdir.
}

/**
 * TODO
 */
void logfile_write_line(const char* line, ...) {
    va_list ap;
    va_start(ap, line);
    sceClibVprintf(line, ap);
    va_end(ap);
}
