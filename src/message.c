/*
 * Group C
 *
 * Author: Em Jordan
 * Email: em.jordan@okstate.edu
 * Date: 4/6/2025
 * Description:
 *
 * TODO: Figure out the best way to structure this so child processes are
 * restricted to their operations I think the parent process is going to manage
 * the shared memory space(?)
 *
 * TODO: I'll need to read the project document more just to make sure I have
 * the specifications right!
 */
#include "message.h"
#include <bits/pthreadtypes.h>
#include <pthread.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

const char *message_type_name(message_type_t type) {
    switch (type) {
    case MESSAGE_OPEN_SLOT:
        return "[OPEN]";
    case REQUEST_ACQUIRE:
        return "REQUEST_ACQUIRE";
    case REQUEST_RELEASE:
        return "REQUEST_RELEASE";
    case RESPONSE_GRANT:
        return "RESPONSE_GRANT";
    case RESPONSE_WAIT:
        return "RESPONSE_WAIT";
    case RESPONSE_DENY:
        return "RESPONSE_DENY";
    }
}

void queue_init(message_queue_t *queue) {
    queue->head = 0;
    queue->tail = 0;
    queue->length = 0;

    // setup attributes to make sure we can use the mutex across processes
    pthread_mutexattr_t attrs;
    pthread_mutexattr_init(&attrs);
    pthread_mutexattr_setpshared(&attrs, PTHREAD_PROCESS_SHARED);
    // initialize process
    pthread_mutex_init(&queue->lock, &attrs);
}

void queue_deinit(message_queue_t *queue) { pthread_mutex_destroy(&queue->lock); }

int queue_find_slot(message_queue_t *queue) {
    for (int i = queue->head; i < queue->length; i++) {
        // Make sure our index stays in bounds
        int index = (i % queue->capacity);
        if (queue->items[index].type == MESSAGE_OPEN_SLOT) {
            return index;
        }
    }
    return -1;
}

int queue_enqueue(message_queue_t *queue, message_t msg) {
    // Acquire a lock before we do anything
    pthread_mutex_lock(&queue->lock);
    // TODO:  We should add error types to properly handle any issues that occur
    // In this case, we should wait for an open position before adding the
    // message
    if (queue->length >= queue->capacity) {
        pthread_mutex_unlock(&queue->lock);
        return -1;
    }

    // Push our message into the queue
    queue->items[queue->tail] = msg;
    // Update our tail & length
    queue->tail = (queue->tail + 1) % queue->capacity;
    queue->length++;
    // Unlock after we've done our modifications
    pthread_mutex_unlock(&queue->lock);
    return 0;
}

message_t queue_dequeue(message_queue_t *queue) {
    pthread_mutex_lock(&queue->lock);
    message_t msg = queue->items[queue->head];
    // Designate our slot as open by setting its data to the default open slot
    // struct
    queue->items[queue->head] = MSG_OPEN_SLOT;

    size_t next_index = (queue->head + 1) % queue->capacity;
    queue->head = next_index;
    queue->length--;
    pthread_mutex_unlock(&queue->lock);
    return msg;
}

message_t queue_peek(message_queue_t *queue) { return queue->items[queue->head]; }

void send_request(message_queue_t *queue, message_t message) {
    if (queue_enqueue(queue, message) == -1) {
        // TODO: error
        return;
    }
}

void send_response(message_queue_t *queue, message_t message) {
    if (queue_enqueue(queue, message) != 0) {
        // TODO: handle errors
    }
}
message_t wait_for_response(message_queue_t *queue, const char *dst) {
    // waits for the queue to process its message before returning the response
    while (queue->length <= 0 || strcmp(queue_peek(queue).dst, dst) != 0) {
    }
    return queue_dequeue(queue);
}
