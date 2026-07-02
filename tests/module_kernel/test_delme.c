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

#include "delme.c"

// Setup mocks.
DEFINE_FFF_GLOBALS;

/**
 * Setup test fixture. Called before each test.
 */
static int setup(void** state) {
    (void)state;

    // Reset fff.
    FFF_RESET_HISTORY();  // TODO remove setup function?

    return 0;
}

static void test_delet_me(void** state) {
    (void)state;

    int ret = delete_me();
    assert_int_equal(ret, 123);
}

/**
 * Entrypoint for all tests in this file.
 */
int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test_setup(test_delet_me, setup),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
