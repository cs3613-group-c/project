/*
 * Group C
 *
 * Name: Em Jordan
 * Email: em.jordan@okstate.edu
 * Date: 4/17/2025
 * Description: A small header used to run a suite of tests on
 */
#ifndef TEST_RUNNER_H

#include <stdbool.h>

// Macros to ease the pain of logging
// TODO: File logging? Colors?
#define TEST_LOG_INFO(fmt, ...) printf("[*] " fmt "\n", __VA_ARGS__);
#define TEST_LOG_ERR(fmt, ...) printf("[!] " fmt "\n", __VA_ARGS__);

// A function pointer that defines what a test looks like
typedef bool (*test_func_t)();

// A struct that defines the data for our tests
typedef struct {
    const char *name;
    test_func_t func;
} test_data_t;

// Runs the given tests, prints the results to the output, and returns true if
// succeeded
bool test_all(const test_data_t tests[], int tests_len);

#endif
