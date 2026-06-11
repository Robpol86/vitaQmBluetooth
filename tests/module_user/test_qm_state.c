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

// todo https://www.samlewis.me/2016/09/embedded-unit-testing-with-cmocka/

#include <cmocka.h>

#include "qm_state.h"
#include "vqmbt.h"

/**
 * TODO remove this test.
 */
static void test_mac_to_idx(void** state) {
    (void)state;

    assert_int_equal(mac_to_idx(1, 1), VQMBT_ERROR_INVALID_ARGUMENT);
}

/**
 * Test bulk update from no devices to no devices.
 */
static void test_bulk_from_clean_no_devices(void** state) {
    (void)state;

    // Setup.
    // TODO zero out state
    // TODO mock refresh_ui() and assert it does not run.

    // Run.
    const VqmbtDeviceInfo devices[VQMBT_MAX_DEVICES] = {0};
    qm_state_update_ui(&(QmsRequest){
        .id = QMS_REQUEST_BULK_UPDATE,
        .bulk.bluetooth_on = true,
        .bulk.num_devices = 0,
        .bulk.devices = devices,
    });

    // Verify.
    // TODO
}

// static void test_bulk_from_clean_one_device(void) {
//     static_assert(1 == 1, "TODO");  // TODO
// }

// static void test_bulk_from_clean_max_devices(void) {
//     static_assert(1 == 1, "TODO");  // TODO
// }

// static void test_bulk_from_clean_one_device_bt_off(void) {
//     static_assert(1 == 1, "TODO");  // TODO
// }

// static void test_bulk_from_clean_one_device_already_connected(void) {
//     static_assert(1 == 1, "TODO");  // TODO
// }

// static void test_bulk_add_remove_only_device(void) {
//     static_assert(1 == 1, "TODO");  // TODO
// }

// static void test_bulk_add_remove_second_device(void) {
//     static_assert(1 == 1, "TODO");  // TODO
// }

// static void test_bt_on_off_on_off(void) {
//     // TODO 0, 1, 2, max devices.
//     static_assert(1 == 1, "TODO");  // TODO
// }

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_mac_to_idx),
        cmocka_unit_test(test_bulk_from_clean_no_devices),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
