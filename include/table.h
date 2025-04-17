#ifndef TABLE_H
#define TABLE_H
#include "structures.h"


int table_init(shared_memory_t *m, parse_t input);
bool table_is_sctn_locked(int i);
void table_sctn_add_train(int sctn, int train);
void table_sctn_rem_train(int sctn, int train);
void table_print();

#endif