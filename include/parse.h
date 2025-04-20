/*
 * Group C
 *
 * Author: Gabe Cornelius
 * Email: gabriel.j.cornelius@okstate.edu
 * Date: 4/6/2025
 * Description: File parsing functions, extracts data from intersections.txt
 * and trains.txt and stores it in arrays to be passed as a "Parse" struct to
 * the calling function.
 * Additionally, passes an error code indicating which errors have occured
 */
#ifndef PARSE_H
#define PARSE_H

#include "constants.h"
#include "structures.h"

int parse_file(
    const char *sctns_filename,
    const char *trains_filename,
    intersection_t *sctns,
    train_t *trains,
    int *num_sctns,
    int *num_trains);

void train_print_status(train_t *train);
void intersection_print_status(intersection_t *sctn);
#endif
