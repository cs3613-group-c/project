// Author: Em Jordan
// TODO: Figure out the best way to structure this so child processes are restricted to their operations
// I think the parent process is going to manage the shared memory space(?)
//
// TODO: I'll need to read the project document more just to make sure I have the specifications right!
typedef struct {

} message_queue_t;

void message_acquire(message_queue_t *queue) {

}


void message_release(message_queue_t *queue) {

}

void message_grant(message_queue_t *queue) {}

void message_wait(message_queue_t *queue) {}

void message_deny(message_queue_t * queue) {}
