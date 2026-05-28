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
 * @brief Miscellaneous macros and functions.
 ******************************************************************************/

#ifndef COMMON_H
#define COMMON_H

#include <psp2kern/kernel/cpu.h>
#include <stdint.h>

/**
 * Call EXIT_SYSCALL on scope exit.
 */
static inline void _syscall_state_cleanup(uint32_t* _state) { EXIT_SYSCALL(*_state); }
#define SYSCALL_STATE __attribute__((cleanup(_syscall_state_cleanup)))

#endif  // COMMON_H
