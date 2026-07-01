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

/**
 * TODO:
 * - https://www.samlewis.me/2016/09/embedded-unit-testing-with-cmocka/
 * - Revisit mocks, prune, re-implement, understand.
 */

#include <cmocka.h>
#include <fff.h>
#include <stdio.h>
#include <string.h>

#include "qm_state.c"
#include "vqmbt.h"

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
FAKE_VALUE_FUNC_VARARG(int, sceClibPrintf, const char*, ...);
FAKE_VALUE_FUNC_VARARG(int, sceClibSnprintf, char*, SceSize, const char*, ...);
FAKE_VOID_FUNC_VARARG(logfile_write_line, int, int, int, const char*, ...);

// === Hand-written bridges — need real behavior ===
void* sceClibMemcpy(void* d, const void* s, SceSize n) { return memcpy(d, s, n); }
void* sceClibMemset(void* d, int c, SceSize n) { return memset(d, c, n); }

// === Hand-written no-ops — tests don't care about these side effects ===
int sceRtcGetCurrentClock(SceDateTime* t, int tz) {
    if (t) memset(t, 0, sizeof(*t));
    (void)tz;
    return 0;
}

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
 * Test bulk update from clean state to single-device state.
 */
static void test_bulk_from_clean_one_device(void** state) {
    (void)state;

    // Run.
    const VqmbtDeviceInfo devices[VQMBT_MAX_DEVICES] = {
        [0] = {.name = "Test Device", .mac0 = 0x12345678, .mac1 = 0x9ABCDEF0, .state = VQMBT_SCE_BT_STATE_DISCONNECTED},
    };
    const bool changed = qm_state_update_ui(&(QmsRequest){
        .id = QMS_REQUEST_BULK_UPDATE,
        .bulk.bluetooth_on = true,
        .bulk.num_devices = 1,
        .bulk.devices = devices,
    });

    // Verify.
    assert_true(changed);
    assert_string_equal(qm_state.buttons[0].device.name, "Test Device");
    assert_int_equal(qm_state.buttons[0].device.mac0, 0x12345678);
    assert_int_equal(qm_state.buttons[0].device.mac1, 0x9ABCDEF0);
    assert_int_equal(qm_state.buttons[0].device.state, VQMBT_SCE_BT_STATE_DISCONNECTED);
    assert_int_equal(qm_state.buttons[0].btn_state, BTNSTATE_DISCONNECTED);
    for (int i = 1; i < VQMBT_MAX_DEVICES; i++) {
        assert_string_equal(qm_state.buttons[i].device.name, "");
        assert_int_equal(qm_state.buttons[i].device.mac0, 0);
        assert_int_equal(qm_state.buttons[i].device.mac1, 0);
        assert_int_equal(qm_state.buttons[i].device.state, VQMBT_SCE_BT_STATE_UNKNOWN0);
        assert_int_equal(qm_state.buttons[i].btn_state, BTNSTATE_SLOT_EMPTY_DISABLED);
    }
}

/**
 * Entrypoint for all tests in this file.
 */
int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test_setup(test_bulk_from_clean_one_device, setup),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
