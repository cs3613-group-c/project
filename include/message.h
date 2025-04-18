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
#ifndef MESSAGE_H
#define MESSAGE_H

#include <stddef.h>

/* The different types that our messages could be */
typedef enum {
    // NOTE: This is *not* a message. It is a reserved member that signifies an
    // open slot in our message queue
    MESSAGE_OPEN_SLOT = 0,
    // Train -> Server Messages
    // A request from the train to acquire a specific intersection
    REQUEST_ACQUIRE,
    // A request from the train to release a specific intersection
    REQUEST_RELEASE,
    // Server -> Train Messages
    // A response from the server that grants the train their requested
    // intersection
    RESPONSE_GRANT,
    // A response from the server that indicates a train should wait for their
    // request intersection
    RESPONSE_WAIT,
    // A response from the server that denies that request for an intersection
    RESPONSE_DENY,
} message_type_t;

/* Returns the name of the given message type */
const char *message_type_name(message_type_t type);
/*
 * The payload types that will dictate our requests/responses
 * In this case, a void* indicates that NULL should be used in place of actual
 * data
 * */
typedef void *message_empty_data_t;
typedef const char *message_intersection_name_t;

/* The associated message payloads */
typedef union {
    void *open_slot;
    // The name of the intersection that the train is wishing to acquire
    message_intersection_name_t acquire;
    message_intersection_name_t release;
    // TODO: Do we need to the name of the train back for any of the responses?
    message_intersection_name_t grant;
    message_empty_data_t wait;
    message_empty_data_t deny;
} message_data_t;

/*
 * The actual tagged union that will store our message type, the source of the
 * message, the destination for the message, and the associated data Both
 * requests & responses are held as mesasges. It is up to the respective queues
 * to process the requests they want
 * */
typedef struct {
    message_type_t type;
    const char *src;
    const char *dst;
    message_data_t data;
} message_t;

/* A message that will indicate an open space in our message queue */
#define MSG_OPEN_SLOT                                                          \
    (message_t) {                                                              \
        MESSAGE_OPEN_SLOT, "", "", { .open_slot = NULL }                       \
    }

/*
 * Our message queue type. In this case, we're using a dynamic array w/ a
 * pointer to our shared memory space. From there, we have a head index & tail
 * index with a length to track how the queue looks in memory.
 *
 * We *could* utilize a linked list type here but I think an array would suit us
 * just as well and would save us time if we need to search for a node
 *
 * Ideally, we'll have functions like `smem_alloc`, `smem_resize`,
 * `smem_realloc`, and `smem_destroy` to properly manage our shared memory space
 * with respect to our message queue type.
 *
 * TODO: Is this something that Cade could expose for us?
 */
typedef struct {
    message_t *items;
    size_t length;
    size_t capacity;

    size_t head;
    size_t tail;
} message_queue_t;

/* ALlocates a queue into our shared memory space */
message_queue_t *queue_alloc();

/* Initializes a given queue */
void queue_init(message_queue_t *queue);

/* Returns the first open index from the head or a negative value if it can't
 * find one */
int queue_find_slot(message_queue_t *queue);

/* Enqueues the given message onto the queue. A negative value is returned if an
 * error occurs */
int queue_enqueue(message_queue_t *queue, message_t msg);

/* Dequeues a message from the given queue */
message_t queue_dequeue(message_queue_t *queue);

/* Peeks at the front-most item in the queue */
message_t queue_peek(message_queue_t *queue);

/* Puts a message into the request queue to be processed */
void send_request(message_queue_t *queue, message_t message);
/* Handles a given message from the request queue */
void handle_request(message_queue_t *queue, message_t message);

/* The response queue's logic for sending messages from the server to the trains
 */
void send_response(message_queue_t *queue, message_t message);
/* The response queue's logic for processing messages */
void handle_response(message_queue_t *queue, message_t message);

/* Waits for a response to the given destination before returning */
message_t wait_for_response(message_queue_t *queue, const char *dst);

#endif
