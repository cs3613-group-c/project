/*
 * Group C
 *
 * Author: Erin Dunlap
 * Email: erin.dunlap10@oksate.edu
 * Date: 3/28/2025
 * Description: header file for forking implementation. the fork_trains function makes sure that a separate process gets made for each train
 */
#ifndef FORKING_H
#define FORKING_H

#include "structures.h"

// Called from main to fork and simulate trains
void fork_trains(train_t *train_list, int train_count);

#endif
