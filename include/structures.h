/*
* Group C
*
* Set of structs in the system. Add as needed.
*
*/
#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <pthread.h>
#include <semaphore.h>
#include "constants.h"

typedef enum {
    LOCK_MUTEX,
    LOCK_SEMAPHORE,
} intersection_lock_type_t;

typedef union {
    pthread_mutex_t *mutex;
    sem_t *semaphore;
} intersection_lock_t;

typedef struct {
    char name[MAX_NAME_LENGTH];
    int capacity;
    intersection_lock_type_t lock_type;
    intersection_lock_t *lock_data;
    char holding_trains[MAX_TRAINS][MAX_NAME_LENGTH];
    int num_holding_trains;
} intersection_t;

typedef struct {
    char name[MAX_NAME_LENGTH];
    char route[MAX_ROUTE_LENGTH][MAX_NAME_LENGTH];
    int route_len;
    int current_position;
} train_t;

typedef struct{
	int route[9][26], route_count, sctn[26], sctn_count, error; 
} Parse;

typedef struct {
    intersection_t intersections[MAX_INTERSECTIONS];
    int num_intersections;
    train_t trains[MAX_TRAINS];
    int num_trains;
    pthread_mutex_t time_mutex;
    int sim_time;
    Parse input; //FIXME: temporary data structure until we can properly load from parser into other structs
} shared_memory_t;

typedef struct{
    int route[MAX_TRAINS][MAX_ROUTE_LENGTH], route_count, sctn[MAX_ROUTE_LENGTH], sctn_count, error; 
} Parse;

#endif
