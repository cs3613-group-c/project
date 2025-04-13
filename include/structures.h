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

typedef struct {
    char name[MAX_NAME_LENGTH];
    int type;
    int capacity;
    pthread_mutex_t* mutex;
    sem_t* semaphore;
    char holding_trains[MAX_TRAINS][MAX_NAME_LENGTH];
    int num_holding_trains;
} intersection_t;

typedef struct {
    char name[MAX_NAME_LENGTH];
    char route[MAX_ROUTE_LENGTH][MAX_NAME_LENGTH];
    int route_len;
    int current_position;
} train_t;

typedef struct {
    intersection_t intersections[MAX_INTERSECTIONS];
    int num_intersections;
    train_t trains[MAX_TRAINS];
    int num_trains;
    pthread_mutex_t time_mutex;
    int sim_time;
} shared_memory_t;

typedef struct{
    int route[10][26], sctn[26];
    int error;
} Parse;

#endif
