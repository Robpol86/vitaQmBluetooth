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

#include <assert.h>  // TODO remove
#include <cmocka.h>

// #include "delme.c"

static void test_mac_to_idx(void) {
    static_assert(1 == 1, "TODO");  // TODO
}

static void test_bulk_from_clean_no_devices(void) {
    static_assert(1 == 1, "TODO");  // TODO
}

static void test_bulk_from_clean_one_device(void) {
    static_assert(1 == 1, "TODO");  // TODO
}

static void test_bulk_from_clean_max_devices(void) {
    static_assert(1 == 1, "TODO");  // TODO
}

static void test_bulk_from_clean_one_device_bt_off(void) {
    static_assert(1 == 1, "TODO");  // TODO
}

static void test_bulk_from_clean_one_device_already_connected(void) {
    static_assert(1 == 1, "TODO");  // TODO
}

static void test_bulk_add_remove_only_device(void) {
    static_assert(1 == 1, "TODO");  // TODO
}

static void test_bulk_add_remove_second_device(void) {
    static_assert(1 == 1, "TODO");  // TODO
}

static void test_bt_on_off_on_off(void) {
    // TODO 0, 1, 2, max devices.
    static_assert(1 == 1, "TODO");  // TODO
}

int main(void) {
    // TODO eliminate main() in tests and generate it in CMakeLists.txt?

    test_mac_to_idx();

    test_bulk_from_clean_no_devices();
    test_bulk_from_clean_one_device();
    test_bulk_from_clean_max_devices();

    test_bulk_from_clean_one_device_bt_off();
    test_bulk_from_clean_one_device_already_connected();

    test_bulk_add_remove_only_device();
    test_bulk_add_remove_second_device();

    test_bt_on_off_on_off();

    return 0;
}
