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
} Intersection;

typedef struct {
    char name[MAX_NAME_LENGTH];
    char route[MAX_ROUTE_LENGTH][MAX_NAME_LENGTH];
    int route_length;
    int current_position;
} Train;

typedef struct {
    long msg_type;
    char train_name[MAX_NAME_LENGTH];
    char intersection_name[MAX_NAME_LENGTH];
    int request_type;
} Message;

typedef struct {
    Intersection intersections[MAX_INTERSECTIONS];
    int num_intersections;
    Train trains[MAX_TRAINS];
    int num_trains;
    pthread_mutex_t time_mutex;
    int sim_time;
} SharedMemory;

#endif
