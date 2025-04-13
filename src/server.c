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
#include <errno.h>
#include <sys/msg.h>
#include <semaphore.h>
#include <pthread.h>
#include "../include/ipc.h"
#include "../include/structures.h"
#include "../include/utils.h"

extern int msgq_id;
extern shared_memory_t *shared_memory;

void server_process() {
    /* TODO: Implement the main server process that handles train requests
     * 1. Keep track of completed trains
     * 2. Process messages from trains in a loop until all trains complete
     * 3. Handle two types of messages:
     *    a) ACQUIRE_REQUEST: When a train wants to enter an intersection
     *    b) RELEASE_REQUEST: When a train wants to leave an intersection
     * 
     * For ACQUIRE_REQUEST:
     * - Check if the intersection is available (mutex or semaphore)
     * - If available, grant access and send GRANT_RESPONSE
     * - If not available, add to wait queue and check for deadlocks
     * 
     * For RELEASE_REQUEST:
     * - Remove train from the intersection's holding list
     * - Release the resource (mutex or semaphore)
     * - Check if the train has completed its route
     * 
     * Use the Message struct for communication:
     * - msg_type: ACQUIRE_REQUEST or RELEASE_REQUEST
     * - train_name: Name of the requesting train
     * - intersection_name: Name of the intersection
     */
}


int detect_deadlock() {
    /* TODO: Implement deadlock detection
     * 1. Build a resource allocation graph
     * 2. Check for cycles in the graph (indicating deadlock)
     * 3. Return 1 if deadlock is detected, 0 otherwise
     *
     * - Use shared_memory->trains to get train information
     * - Use shared_memory->intersections to get intersection states
     */
    return 0;
}

// Maybe ignore deadlocks
void resolve_deadlock() {
    /* TODO: Implement deadlock resolution
     * 1. Choose a train to "back out" of its request
     * 2. Release any resources held by the chosen train
     * 3. Update the system state accordingly
     */
}
