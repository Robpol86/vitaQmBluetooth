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

// TODO #include "bluetooth.c"
#include "fff.h"

DEFINE_FFF_GLOBALS;

static void test_kvqmbt_get_paired_devices(void** state) {
    (void)state;

    assert_int_equal(1, 1);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_kvqmbt_get_paired_devices),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
