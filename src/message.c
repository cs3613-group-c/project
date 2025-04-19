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

message_queue_t *queue_alloc() {
    // TODO: allocate a queue
    message_queue_t *queue = NULL;
    // Initialize our queue before returning it
    queue_init(queue);
    return queue;
}

void queue_init(message_queue_t *queue) {
    queue->head = 0;
    queue->tail = 0;
    queue->length = 0;
}

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
    // TODO:  We should add error types to properly handle any issues that occur
    // In this case, we should wait for an open position before adding the
    // message
    if (queue->length >= queue->capacity) {
        return -1;
    }

    // Push our message into the queue
    size_t next_index = (queue->tail + 1) % queue->capacity;
    queue->tail = next_index;
    queue->items[next_index] = msg;
    queue->length++;
    return 0;
}

message_t queue_dequeue(message_queue_t *queue) {
    message_t msg = queue->items[queue->head];
    // Designate our slot as open by setting its data to the default open slot
    // struct
    queue->items[queue->head] = MSG_OPEN_SLOT;

    size_t next_index = (queue->head + 1) % queue->capacity;
    queue->head = next_index;
    queue->length--;
    return msg;
}

message_t queue_peek(message_queue_t *queue) { return queue->items[queue->head]; }

void send_request(message_queue_t *queue, message_t message) {
    if (queue_enqueue(queue, message) == -1) {
        // TODO: error
        return;
    }
}

void handle_request(message_queue_t *queue, message_t message) {
    // if we don't have a message to process, ignore it
    if (queue->length <= 0)
        return;

    message_t msg = queue_dequeue(queue);

    switch (msg.type) {
    case REQUEST_ACQUIRE:
        // TODO: Check for actual access via mutexes/semaphores
        send_response(
            queue,
            (message_t){
                .type = RESPONSE_GRANT,
                .src = "SERVER",
                .dst = message.src,
                .data =
                    {
                        .grant = message.data.acquire,
                    },
            });
    case REQUEST_RELEASE:
        // TODO: Do we need to send a response for this? We'll set it as
        // granting anyway but
        send_response(
            queue,
            (message_t){
                .type = RESPONSE_GRANT,
                .src = "SERVER",
                .dst = message.src,
                .data =
                    {
                        .grant = message.data.acquire,
                    },
            });

        break;
    default:
        // TODO: Should we return an error for unhandled messages?
        // ("[WARNING] RECEIVED UNHANDLED MESSAGE: %s",
        // message_type_name(msg.type));
        break;
    }
}

void send_response(message_queue_t *queue, message_t message) {
    if (queue_enqueue(queue, message) != 0) {
        // TODO: handle errors
    }
}

void handle_response(message_queue_t *queue, message_t message) {
    // if we don't have a message to process, ignore it
    if (queue->length <= 0)
        return;

    message_t msg = queue_dequeue(queue);

    // TODO: properly link this with our intersections
    switch (msg.type) {
    case RESPONSE_GRANT:
        printf(
            "[CLIENT %s: INFO] Server has granted client access to "
            "intersection",
            message.dst);
    case RESPONSE_WAIT:
        printf("[CLIENT %s: INFO] Server told client to wait for intersection", message.dst);
        break;
    case RESPONSE_DENY:
        printf("[CLIENT %s: WARN] Server was denied access to intersection", message.dst);
        break;
    default:
        printf(
            "[CLIENT %s: WARN] RECEIVED UNHANDLED MESSAGE: %s",
            message.dst,
            message_type_name(msg.type));
        break;
    }
}

message_t wait_for_response(message_queue_t *queue, const char *dst) {
    // waits for the queue to process its message before returning the response
    while (queue->length <= 0 || strcmp(queue_peek(queue).dst, dst) != 0) {
    }
    return queue_dequeue(queue);
}
