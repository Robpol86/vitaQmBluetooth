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
 */

#include <cmocka.h>

#include "qm_state.c"

/**
 * Test bulk update from no devices to no devices.
 */
static void test_bulk_from_clean_no_devices(void** state) {
    (void)state;

    // Setup.
    qm_state = (QmState){0};

    // Run.
    const VqmbtDeviceInfo devices[VQMBT_MAX_DEVICES] = {0};
    const bool changed = qm_state_update_ui(&(QmsRequest){
        .id = QMS_REQUEST_BULK_UPDATE,
        .bulk.bluetooth_on = true,
        .bulk.num_devices = 0,
        .bulk.devices = devices,
    });

    // Verify.
    assert_false(changed);
    for (int i = 0; i < VQMBT_MAX_DEVICES; i++) {
        assert_string_equal(qm_state.buttons[i].device.name, "");
        assert_int_equal(qm_state.buttons[i].device.mac0, 0);
        assert_int_equal(qm_state.buttons[i].device.mac1, 0);
        assert_int_equal(qm_state.buttons[i].device.state, VQMBT_BT_STATE_UNKNOWN0);
        assert_int_equal(qm_state.buttons[i].btn_state, BTNSTATE_SLOT_EMPTY_DISABLED);
    }
}

static void test_bulk_from_clean_one_device(void** state) {
    (void)state;

    skip();  // static_assert(1 == 1, "TODO");  // TODO
}

static void test_bulk_from_clean_max_devices(void** state) {
    (void)state;

    skip();  // static_assert(1 == 1, "TODO");  // TODO
}

static void test_bulk_from_clean_one_device_bt_off(void** state) {
    (void)state;

    skip();  // static_assert(1 == 1, "TODO");  // TODO
}

static void test_bulk_from_clean_one_device_already_connected(void** state) {
    (void)state;

    skip();  // static_assert(1 == 1, "TODO");  // TODO
}

static void test_bulk_add_remove_only_device(void** state) {
    (void)state;

    skip();  // static_assert(1 == 1, "TODO");  // TODO
}

static void test_bulk_add_remove_second_device(void** state) {
    (void)state;

    skip();  // static_assert(1 == 1, "TODO");  // TODO
}

static void test_bt_on_off_on_off(void** state) {
    (void)state;

    // TODO 0, 1, 2, max devices.
    skip();  // static_assert(1 == 1, "TODO");  // TODO
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_bulk_from_clean_no_devices),
        cmocka_unit_test(test_bulk_from_clean_one_device),
        cmocka_unit_test(test_bulk_from_clean_max_devices),
        cmocka_unit_test(test_bulk_from_clean_one_device_bt_off),
        cmocka_unit_test(test_bulk_from_clean_one_device_already_connected),
        cmocka_unit_test(test_bulk_add_remove_only_device),
        cmocka_unit_test(test_bulk_add_remove_second_device),
        cmocka_unit_test(test_bt_on_off_on_off),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
