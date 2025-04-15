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

#include "../include/sync.h"
#include "../include/utils.h"
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void init_intersection_sync(intersection_t *intersection) {
    if (intersection->lock_type == LOCK_MUTEX) {
        intersection->lock_data =
            (intersection_lock_t *)malloc(sizeof(intersection_lock_t));
        if (!intersection->lock_data) {
            perror("Failed to allocate mutex");
            exit(EXIT_FAILURE);
        }
        intersection->lock_data->mutex =
            (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
        pthread_mutex_init(intersection->lock_data->mutex, NULL);
    } else {
        intersection->lock_data =
            (intersection_lock_t *)malloc(sizeof(intersection_lock_t));
        if (!intersection->lock_data) {
            perror("Failed to allocate semaphore");
            exit(EXIT_FAILURE);
        }
        intersection->lock_data->semaphore = (sem_t *)malloc(sizeof(sem_t));
        sem_init(intersection->lock_data->semaphore, 1, intersection->capacity);
    }
}

void cleanup_intersection_sync(intersection_t *intersection) {
    if (intersection->lock_type == LOCK_MUTEX) {
        pthread_mutex_destroy(intersection->lock_data->mutex);
        free(intersection->lock_data->mutex);
        free(intersection->lock_data);
        intersection->lock_data = NULL;
    } else {
        sem_destroy(intersection->lock_data->semaphore);
        free(intersection->lock_data->semaphore);
        free(intersection->lock_data);
        intersection->lock_data = NULL;
    }
}

int try_acquire_intersection(intersection_t *intersection,
                             const char *train_name) {
    if (intersection->lock_type == LOCK_MUTEX) {
        if (pthread_mutex_trylock(intersection->lock_data->mutex) == 0) {
            strcpy(intersection->holding_trains[0], train_name);
            intersection->num_holding_trains = 1;
            return 1; // Successfully acquired
        }
        return 0; // Failed to acquire
    } else {
        int value;
        sem_getvalue(intersection->lock_data->semaphore, &value);
        if (value > 0) {
            sem_wait(intersection->lock_data->semaphore);
            strcpy(
                intersection->holding_trains[intersection->num_holding_trains],
                train_name);
            intersection->num_holding_trains++;
            return 1; // Successfully acquired
        }
        return 0; // Failed to acquire
    }
}

void release_intersection(intersection_t *intersection,
                          const char *train_name) {
    // Remove train from holding list
    for (int i = 0; i < intersection->num_holding_trains; i++) {
        if (strcmp(intersection->holding_trains[i], train_name) == 0) {
            // Shift remaining trains down
            for (int j = i; j < intersection->num_holding_trains - 1; j++) {
                strcpy(intersection->holding_trains[j],
                       intersection->holding_trains[j + 1]);
            }
            intersection->num_holding_trains--;
            break;
        }
    }

    // Release the resource
    if (intersection->lock_type == LOCK_MUTEX) {
        pthread_mutex_unlock(intersection->lock_data->mutex);
    } else {
        sem_post(intersection->lock_data->semaphore);
    }
}

int is_train_holding_intersection(intersection_t *intersection,
                                  const char *train_name) {
    for (int i = 0; i < intersection->num_holding_trains; i++) {
        if (strcmp(intersection->holding_trains[i], train_name) == 0) {
            return 1;
        }
    }
    return 0;
}

void get_intersection_state(intersection_t *intersection, char *state_str) {
    if (intersection->lock_type == LOCK_MUTEX) {
        sprintf(state_str, "Mutex intersection %s: %s", intersection->name,
                intersection->num_holding_trains > 0 ? "Locked" : "Unlocked");
    } else {
        int value;
        sem_getvalue(intersection->lock_data->semaphore, &value);
        sprintf(state_str,
                "Semaphore intersection %s: Count=%d, Holding=%d trains",
                intersection->name, value, intersection->num_holding_trains);
    }
}
