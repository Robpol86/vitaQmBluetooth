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

/**
 * Main entrypoint of the kernel plugin.
 */

#include <psp2kern/kernel/debug.h>
#include <psp2kern/kernel/modulemgr.h>

#include "log.h"

int module_start(SceSize argc, const void* args) {
    (void)argc;
    (void)args;

    // TODO not working, maybe recv message from QuickMenu then emit, then respond, then QM emits ack.
    LOG_DEBUG("TODO RP");

    return SCE_KERNEL_START_SUCCESS;
}
