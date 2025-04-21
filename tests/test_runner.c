/*
 * Group C
 *
 * Name: Em Jordan
 * Email: em.jordan@okstate.edu
 * Date: 4/17/2025
 * Description: A small header used to run a suite of tests on our codebase
 */
#include "test_runner.h"
#include <stdbool.h>
#include <stdio.h>

// Tests all the functions passed to it, reports the results, and returns true
// if succeeded
bool test_all(const test_data_t tests[], int tests_len) {
    int passed = 0;
    printf("-------------------------------------\n");
    for (int i = 0; i < tests_len; i++) {
        // grab our test and try it
        test_data_t test = tests[i];

        TEST_LOG_INFO("Running '%s'...", test.name);
        bool result = (test.func)();
        if (result) {
            TEST_LOG_INFO("Test '%s' succeeded", test.name);
        } else {
            TEST_LOG_ERR("Test '%s' failed", test.name);
        }
        passed += result ? 1 : 0;
    }

    bool succeeded = passed == tests_len;
    printf("-------------------------------------\n");
    if (succeeded) {
        TEST_LOG_INFO("RESULTS: PASSED (%d / %d)", passed, tests_len);
    } else {
        TEST_LOG_ERR("RESULTS: FAILED (%d / %d)", passed, tests_len);
    }
    return succeeded;
}
