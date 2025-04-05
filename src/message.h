// Author: Em Jordan
// TODO: Figure out the best way to structure this so child processes are restricted to their operations
// I think the parent process is going to manage the shared memory space(?)
//
// TODO: I'll need to read the project document more just to make sure I have the specifications right!
#ifndef MESSAGE_H
#define MESSAGE_H 

/* The different types that our messages could be */
typedef enum {
    MESSAGE_ACQUIRE,
    MESSAGE_RELEASE,
    MESSAGE_GRANT,
    MESSAGE_WAIT,
    MESSAGE_DENY,
} message_type_t;

typedef union {
    struct {} acquire;
    struct {} release;
    struct {} grant;
    struct {} wait; 
    struct {} deny;
} message_data_t;


typedef struct {
    message_type_t type;
    message_data_t data;
} message_t;

/* 
 * Our message queue type. In this case, we're using a dynamic array w/ a pointer to our shared memory space.
 *
 * Ideally, we'll have functions like `smem_alloc`, `smem_resize`, `smem_realloc`, and `smem_destroy` to properly manage our shared memory space
 * with respect to our message queue type.
 *
 * TODO: Is this something that Cade could expose for us?
 */
typedef struct {
    message_t* items;
    size_t count;
    size_t capacity;
} message_queue_t;

/* Puts a message into the queue to be processed */
void send_message(const char* src, message_t message);

/* Handles a given message from the message_queue */
void handle_message(message_t message); 

#endif
