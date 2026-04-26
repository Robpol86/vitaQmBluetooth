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

#include <psp2/kernel/modulemgr.h>

#include "quickmenu.h"

int module_start(SceSize args, const void* argp) {
    (void)args;
    (void)argp;

    quickmenu_start();

    return SCE_KERNEL_START_SUCCESS;
}

int module_stop(SceSize args, const void* argp) {
    (void)args;
    (void)argp;

    quickmenu_stop();

    return SCE_KERNEL_STOP_SUCCESS;
}
