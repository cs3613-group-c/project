/*
* Group C
*
* Description: Set of utility functions in the system. Add as needed.
*
*/
#ifndef UTILS_H
#define UTILS_H

#include "structures.h"

void format_sim_time(int sim_time, char* formatted_time);
void write_log(const char* message);
int find_intersection(const char* name);

#endif
