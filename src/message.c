// Author: Em Jordan
// TODO: Figure out the best way to structure this so child processes are restricted to their operations
// I think the parent process is going to manage the shared memory space(?)
//
// TODO: I'll need to read the project document more just to make sure I have the specifications right!
#include <stddef.h>
#include "../include/message.h"

const char* message_type_name(message_type_t type) {
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
    // In this case, we should wait for an open position before adding the message
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
    // Designate our slot as open by setting its data to the default open slot struct
    queue->items[queue->head] = MSG_OPEN_SLOT;
    queue->head++;
    queue->length--;
    return msg;
}

void send_request(message_queue_t *queue, const char *src, message_t message) {
    
}

void handle_request(message_queue_t *queue, const char *dst, message_t message) {

}

void send_response(message_queue_t *queue, const char *dst, message_t message) {

}

void handle_response(message_queue_t *queue, const char *src, message_t message) {

}
