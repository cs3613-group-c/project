/*
 * Group C
 *
 * Author: Gabe Cornelius
 * Email: gabriel.j.cornelius@okstate.edu
 * Date: 4/6/2025
 * Description:
 */
#ifndef TABLE_H
#define TABLE_H

#include "structures.h"


int table_init(intersection_t *sctn, train_t *train);
void table_add_train(intersection_t *sctn, train_t *train);
void table_wait_train(intersection_t *sctn, train_t *train);
void table_rem_train(intersection_t *sctn, train_t *train);
void table_print(intersection_t *sctn, train_t *train, int num_sctns);

#endif
