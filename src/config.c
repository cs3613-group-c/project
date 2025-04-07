/*
* Group C
*
* Author:
* Email:
* Date:
* Description:
*
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include "../include/structures.h"

extern SharedMemory* shared_memory;
extern FILE* log_file;

static void initialize_time_mutex() {
    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED);
    pthread_mutex_init(&shared_memory->time_mutex, &attr);
}

static void setup_intersection(char* name, int capacity) {
    Intersection* intersection = &shared_memory->intersections[shared_memory->num_intersections];
    strcpy(intersection->name, name);
    intersection->capacity = capacity;
    intersection->num_holding_trains = 0;

    if (capacity == 1) {
        intersection->type = MUTEX_TYPE;
        intersection->mutex = malloc(sizeof(pthread_mutex_t));
        pthread_mutexattr_t attr;
        pthread_mutexattr_init(&attr);
        pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED);
        pthread_mutex_init(intersection->mutex, &attr);
        intersection->semaphore = NULL;
    } else {
        intersection->type = SEMAPHORE_TYPE;
        intersection->semaphore = malloc(sizeof(sem_t));
        sem_init(intersection->semaphore, 1, capacity);
        intersection->mutex = NULL;
    }

    shared_memory->num_intersections++;
}

static void parse_intersections(const char* intersections_file) {
    /* TODO: Parse the intersections configuration file
     * 1. Open and read the intersections file
     * 2. For each line:
     *    - Parse intersection name and capacity (format: "name:capacity")
     *    - Call setup_intersection() with parsed values
     */
}

static void parse_trains(const char* trains_file) {
    /* TODO: Parse the trains configuration file
     * 1. Open and read the trains file
     * 2. For each line:
     *    - Parse train name and route (format: "name:intersection1,intersection2,...")
     *    - Initialize train structure in shared memory:
     *      * Set name
     *      * Set route_length to 0
     *      * Parse and store route intersections
     *      * Increment route_length for each intersection
     * 3. Increment number of trains for each train processed
     */
}

void parse_config_files(const char* intersections_file, const char* trains_file) {
    /* TODO: Initialize and parse all configuration
     * 1. Initialize shared memory counters and time
     * 2. Call initialize_time_mutex()
     * 3. Parse intersection configurations
     * 4. Parse train configurations
     * 5. Log the initialization results
     *
     * - Set num_intersections = 0
     * - Set num_trains = 0
     * - Set sim_time = 0
     * - Log each intersection's details after setup
     */
}
