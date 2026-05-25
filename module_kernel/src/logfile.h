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
 * @brief Module-agnostic macros for logging.
 ******************************************************************************/

/**
 * TODO:
 * - If low disk space disable file logging and show error in notifications or dialog (custom error code?).
 * - static int increment every write. Every n write check free space and disable. Check again after n and re-enable if
 *   enough space.
 * - Performant to touch file on boot intead of on log message? What about date change in the middle of logging?
 * - no colors in logfile
 * - make fetched-logs tail-last
 * - make fetched-screenshots
 * - Only write to log file (and mkdir) in debug builds. Release only printf to stdout.
 * - spin off log.h and logfile.h into library to reuse in my projects. FetchContent. cache in ci for ratelimit?
 */

#ifndef LOGFILE_H_MODULE
#define LOGFILE_H_MODULE

void logfile_write_line(const char* line, ...);

#endif  // LOGFILE_H_MODULE
