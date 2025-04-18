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

#include "structures.h"

parse_t parse_file(const char *intersections_file, const char *trains_file, shared_memory_t *mem);

#endif
