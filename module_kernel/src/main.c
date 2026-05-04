/*
CatLog
Copyright (C) 2024 Cat
Copyright (C) 2020 Princess of Sleeping
Copyright (C) 2020 Asakura Reiko

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, version 3 of the License.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "log.h"

#include <psp2/kernel/error.h>
#include <psp2kern/ctrl.h>
#include <psp2kern/io/fcntl.h>
#include <psp2kern/io/stat.h>
#include <psp2kern/kernel/cpu.h>
#include <psp2kern/kernel/debug.h>
#include <psp2kern/kernel/modulemgr.h>
#include <psp2kern/kernel/sysmem.h>
#include <psp2kern/kernel/threadmgr.h>
#include <psp2kern/kernel/utils.h>
#include <psp2kern/netps.h>
#include <stdarg.h>
#include <taihen.h>

void _start() __attribute__((weak, alias("module_start"))); // TODO remove?

int module_start(SceSize argc, const void *args)
{
  (void)argc;
  (void)args;

  LOG_DEBUG("TODO RP");

  return SCE_KERNEL_START_SUCCESS;
}
