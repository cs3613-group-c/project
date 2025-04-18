/*
 * Group C
 *
 * Author: Drake Geeteh
 * Email: drake.geeteh@okstate.edu
 * Date: 4/4/2025
 * Description: Includes the following synchronization mechanisms which will be
 * integrated in the system:
 *
 * init_intersection_sync: Initialize sync primitives for an intersection
 * cleanup_intersection_sync: Clean up sync primitives for an intersection
 * try_acquire_intersection: Try to acquire an intersection
 * release_intersection: Release an intersection
 * is_train_holding_intersection: Check if a train is holding an intersection
 * get_intersection_state: Get the current state of an intersection
 *
 */
#ifndef SYNC_H
#define SYNC_H

#include "structures.h"

// Initialize synchronization primitives for an intersection
void init_intersection_sync(intersection_t *intersection);

// Clean up synchronization primitives for an intersection
void cleanup_intersection_sync(intersection_t *intersection);

// Try to acquire an intersection
int try_acquire_intersection(intersection_t *intersection,
                             const char *train_name);

// Release an intersection
void release_intersection(intersection_t *intersection, const char *train_name);

// Check if a train is holding an intersection
int is_train_holding_intersection(intersection_t *intersection,
                                  const char *train_name);

// Get the current state of an intersection
void get_intersection_state(intersection_t *intersection, char *state_str);

#endif // SYNC_H
