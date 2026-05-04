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

#ifndef LOG_H
#define LOG_H

#include <psp2kern/kernel/debug.h>

#include "config.h"

// TODO noop LOG_DEBUG() and helpers for release builds (#52).
// TODO better project name part of log instead of appending "Kernel"?
// TODO restore timestamp.
#define LOG_DEBUG(fmtMsg, ...)                                                                                         \
    do {                                                                                                               \
        ksceKernelPrintf("[%02d:%02d:%02d.%03d] [" PROJECT_NAME "Kernel] [%s:%d:%s] [DEBUG] " fmtMsg "\n", 0, 0, 0, 0, \
                         __FILE__, __LINE__, __func__, ##__VA_ARGS__);                                                 \
    } while (0)

#endif  // LOG_H
