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
 * @brief Interface with the bluetooth subsystem. TODO.
 ******************************************************************************/

#include <psp2kern/bt.h>
#include <psp2kern/kernel/cpu.h>
#include <psp2kern/kernel/modulemgr.h>
#include <psp2kern/kernel/sysclib.h>
#include <psp2kern/kernel/sysmem.h>
#include <psp2kern/kernel/threadmgr.h>
#include <stdbool.h>

#include "log.h"

/**
 * TODO.
 *
 * TODO:
 * - make static.
 */
int kvqmbtEventCallback(int notifyId, int notifyCount, int notifyArg, void* userData) {
    (void)notifyId;
    (void)notifyCount;
    (void)notifyArg;
    (void)userData;

    LOG_DEBUG(0, "Called with notifyId=%d notifyCount=%d notifyArg=%d userData=%p", notifyId, notifyCount, notifyArg,
              userData);

    return 0;
}
