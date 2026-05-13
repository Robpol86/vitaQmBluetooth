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
 * @brief Interface for functions exposed from the kernel module.
 ******************************************************************************/

#ifndef SYSCALLS_H
#define SYSCALLS_H

void log_paired_devices(void);

typedef struct VqmbtDeviceInfo {
    char name[128];
    unsigned int mac0;
    unsigned int mac1;
} VqmbtDeviceInfo;

int kvqmbtGetPairedDevices(VqmbtDeviceInfo* info, int info_size);

#endif  // SYSCALLS_H
