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

#include <psp2/kernel/clib.h>
#include <psp2/kernel/processmgr.h>

#include "config.h"

// TODO improve timestamp
// todo noop log debug and helpers on release
#define LOG_DEBUG(fmtMsg, ...)                                                                                         \
    sceClibPrintf("[%llu] [" PROJECT_NAME "] [DEBUG] " fmtMsg "\n", (unsigned long long)sceKernelGetProcessTimeWide(), \
                  ##__VA_ARGS__)

#endif  // LOG_H
