#ifndef SYNC_H
#define SYNC_H

#include "structures.h"

// Initialize synchronization primitives for an intersection
void init_intersection_sync(intersection_t* intersection);

// Clean up synchronization primitives for an intersection
void cleanup_intersection_sync(intersection_t* intersection);

// Try to acquire an intersection
int try_acquire_intersection(intersection_t* intersection, const char* train_name);

// Release an intersection
void release_intersection(intersection_t* intersection, const char* train_name);

// Check if a train is holding an intersection
int is_train_holding_intersection(intersection_t* intersection, const char* train_name);

// Get the current state of an intersection
void get_intersection_state(intersection_t* intersection, char* state_str);

#endif // SYNC_H 