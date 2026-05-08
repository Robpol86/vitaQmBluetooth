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

#include <psp2kern/kernel/modulemgr.h>

#include "log.h"

/**
 * Main entrypoint. Called when the module is started.
 *
 * @param args The size of the arguments passed to the module.
 * @param argp A pointer to the arguments passed to the module.
 * @return SCE_KERNEL_START_SUCCESS on success, or an error code on failure.
 */
int module_start(SceSize args, const void* argp) {
    (void)args;
    (void)argp;

    LOG_DEBUG("Initialized");

    return SCE_KERNEL_START_SUCCESS;
}

/**
 * Unloading entrypoint. Called when the module is stopped.
 *
 * @param args The size of the arguments passed to the module.
 * @param argp A pointer to the arguments passed to the module.
 * @return SCE_KERNEL_STOP_SUCCESS on success, or an error code on failure.
 */
int module_stop(SceSize args, const void* argp) {
    (void)args;
    (void)argp;

    // TODO unload components.

    LOG_DEBUG("Deinitialized");

    return SCE_KERNEL_STOP_SUCCESS;
}
