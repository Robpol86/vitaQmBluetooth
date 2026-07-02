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
 * @brief TODO.
 ******************************************************************************/

#include <cmocka.h>
#include <fff.h>

// Setup pre-include mocks.
DEFINE_FFF_GLOBALS;
FAKE_VOID_FUNC(mock_enter_syscall, uint32_t*);
FAKE_VOID_FUNC(mock_exit_syscall, uint32_t);
#define ENTER_SYSCALL(state) mock_enter_syscall(&(state))
#define EXIT_SYSCALL(state) mock_exit_syscall(state)

// Include source code to test.
#define _PSP2KERN_KERNEL_CPU_H_      // Blocks vitasdk cpu.h
#define _PSP2KERN_KERNEL_SYSCLIB_H_  // Blocks vitasdk sysclib.h
#include "bluetooth.c"

// Setup post-include mocks.
FAKE_VALUE_FUNC(int, ksceBtGetConfiguration);
FAKE_VALUE_FUNC(int, ksceBtGetConnectingInfo, unsigned int, unsigned int);
FAKE_VALUE_FUNC(int, ksceBtGetRegisteredInfo, int, int, SceBtRegisteredInfo*, SceSize);
FAKE_VALUE_FUNC(int, ksceBtStartConnect, unsigned int, unsigned int);
FAKE_VALUE_FUNC(int, ksceBtStartDisconnect, unsigned int, unsigned int);
FAKE_VALUE_FUNC(int, ksceKernelCopyToUser, void*, const void*, SceSize);
FAKE_VALUE_FUNC(int, ksceKernelDelayThread, SceUInt);
FAKE_VALUE_FUNC(int, ksceKernelGetThreadId);
FAKE_VALUE_FUNC(int, ksceRtcGetCurrentClock, SceDateTime*, int);
FAKE_VALUE_FUNC_VARARG(int, ksceKernelPrintf, const char*, ...);
FAKE_VOID_FUNC_VARARG(logfile_write_line, int, int, int, const char*, ...);

/**
 * Setup test fixture. Called before each test.
 */
static int setup(void** state) {
    (void)state;

    // Reset fff.
    FFF_RESET_HISTORY();  // TODO remove setup function?

    return 0;
}

/**
 * Test.
 */
static void test_kvqmbt_bluetooth_state(void** state) {
    (void)state;

    assert_int_equal(1, 1);  // TODO
}

/**
 * Entrypoint for all tests in this file.
 */
int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test_setup(test_kvqmbt_bluetooth_state, setup),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
