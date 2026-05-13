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

#ifndef KBLUETOOTH_H
#define KBLUETOOTH_H

void log_paired_devices(void);

typedef struct VqmbtRegisteredInfo {
    unsigned int mac0;
    unsigned int mac1;
    char name[128];
} VqmbtRegisteredInfo;

int kvqmbtGetPairedDevices(VqmbtRegisteredInfo* info, int info_size);

#endif  // KBLUETOOTH_H
