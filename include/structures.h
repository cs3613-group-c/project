/*
 * Group C
 *
 * Authors: Cade Blakeman, Gabe Cornelius, Drake Geeteh, Em Jordan
 * Date:
 * Description: Struct declarations for the project. Add as needed.
 *
 */
#ifndef STRUCTURES_H
#define STRUCTURES_H

#include "constants.h"
#include "message.h"
#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>

typedef enum {
    LOCK_MUTEX,
    LOCK_SEMAPHORE,
} intersection_lock_type_t;

typedef union {
    pthread_mutex_t *mutex;
    sem_t *semaphore;
} intersection_lock_t;

typedef struct {
    // Modified for resource allocation table implementation - G.C.
    char name[MAX_NAME_LENGTH];
    int capacity;
    intersection_lock_type_t lock_type;
    intersection_lock_t *lock_data;
    char holding_trains[MAX_TRAINS][MAX_NAME_LENGTH];
    bool table_holding_trains[MAX_TRAINS]; // Boolean initialized to zero, to
                                           // index trains and indicate if they
                                           // are held
    int num_holding_trains;
} intersection_t;

typedef struct {

    char name[MAX_NAME_LENGTH];
    char route[MAX_ROUTE_LENGTH][MAX_NAME_LENGTH];
    int route_len;
    int current_position;

} train_t;

typedef struct { //FIXME: make parse_file into a void method and delete this struct
    int routes[MAX_TRAINS][MAX_ROUTE_LENGTH], route_count,
        sctn[MAX_ROUTE_LENGTH], sctn_count, error;
} parse_t;

typedef struct {
    intersection_t intersections[MAX_INTERSECTIONS];
    int num_intersections;
    train_t trains[MAX_TRAINS];
    int num_trains;
    pthread_mutex_t time_mutex;
    int sim_time;
    parse_t input; // FIXME: temporary data structure until we can properly load
                   // from parser into other structs
    message_queue_t request_queue;
    message_queue_t response_queue;
} shared_memory_t;

#endif
