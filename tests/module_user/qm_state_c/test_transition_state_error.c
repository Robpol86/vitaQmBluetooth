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

// Include source code to test.
#include "qm_state.c"

// Setup mocks.
DEFINE_FFF_GLOBALS;
FAKE_VALUE_FUNC(int, QuickMenuRebornSetWidgetColor, const char*, float, float, float, float);
FAKE_VALUE_FUNC(int, QuickMenuRebornSetWidgetLabel, const char*, const char*);
FAKE_VALUE_FUNC(int, kvqmbt_connect_device, unsigned int, unsigned int);
FAKE_VALUE_FUNC(int, kvqmbt_disconnect_device, unsigned int, unsigned int);
FAKE_VALUE_FUNC(int, sceKernelCreateLwMutex, SceKernelLwMutexWork*, const char*, unsigned int, int,
                const SceKernelLwMutexOptParam*);
FAKE_VALUE_FUNC(int, sceKernelDeleteLwMutex, SceKernelLwMutexWork*);
FAKE_VALUE_FUNC(int, sceKernelGetThreadId);
FAKE_VALUE_FUNC(int, sceKernelLockLwMutex, SceKernelLwMutexWork*, int, unsigned int*);
FAKE_VALUE_FUNC(int, sceKernelUnlockLwMutex, SceKernelLwMutexWork*, int);
FAKE_VALUE_FUNC(int, sceRtcGetCurrentClock, SceDateTime*, int);
FAKE_VALUE_FUNC_VARARG(int, sceClibPrintf, const char*, ...);
FAKE_VALUE_FUNC_VARARG(int, sceClibSnprintf, char*, SceSize, const char*, ...);
FAKE_VOID_FUNC_VARARG(logfile_write_line, int, int, int, const char*, ...);
void* sceClibMemcpy(void* d, const void* s, SceSize n) { return memcpy(d, s, n); }
void* sceClibMemset(void* d, int c, SceSize n) { return memset(d, c, n); }

/**
 * Setup test fixture. Called before each test.
 */
static int setup(void** state) {
    (void)state;

    // Reset state to empty.
    qm_state = (QmState){0};

    // Reset fff.
    FFF_RESET_HISTORY();

    return 0;
}

/**
 * Test.
 */
static void test_not_connectable(void** state) {
    (void)state;

    skip();  // TODO
}

/**
 * Test.
 */
static void test_kernel_side_busy(void** state) {
    (void)state;

    skip();  // TODO
}

/**
 * Test.
 */
static void test_surface_sce_error(void** state) {
    (void)state;

    skip();  // TODO
}

/**
 * Entrypoint for all tests in this file.
 */
int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test_setup(test_not_connectable, setup),
        cmocka_unit_test_setup(test_kernel_side_busy, setup),
        cmocka_unit_test_setup(test_surface_sce_error, setup),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
