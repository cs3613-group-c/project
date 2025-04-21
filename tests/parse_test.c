#include "constants.h"
#include "parse.h"
#include "structures.h"
#include "test_runner.h"
#include <stdbool.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/mman.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
//  Group C
//  Author: Gabe Cornelius
//  Email: gabriel.j.cornelius@okstate.edu
//  Date: 4/20/2025
//  Description: Test for parse function, ensuring correct operation
//
bool test_should_parse_successfully() {
    parse_t ret = {{0}, 0, {0}, 0, 0};
    intersection_t sctns[MAX_INTERSECTIONS] = {0};
    train_t trains[MAX_TRAINS] = {0};
    int num_sctns = 0;
    int num_trains = 0;

    int errors =
        parse_file(
            "input/intersections.txt", "input/trains.txt", sctns, trains, &num_sctns, &num_trains) >
        0;

    // print intersection data
    for (int i = 0; i < num_sctns; i++) {
        intersection_print_status(&sctns[i]);
    }

    // print train data
    for (int i = 0; i < num_trains; i++) {
        train_print_status(&trains[i]);
    }
    printf("\n\n");

    printf("route count: %d\nintersection count: %d\n\n", num_trains, num_sctns);

    return errors == 0;
}

int main() {
    test_data_t tests[] = {
        (test_data_t){.name = "Files parse successfully", .func = test_should_parse_successfully}};
    test_all(tests, sizeof(tests) / sizeof(test_data_t));
    return 0;
}
