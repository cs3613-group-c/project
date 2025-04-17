/*
 * Group C
 *
 * Author:
 * Email:
 * Date:
 * Description:
 *
 */

#include "structures.h"
#include <sys/msg.h>
#include <unistd.h>

extern int msgq_id;
extern shared_memory_t *shared_memory;

void train_process(train_t *train) {
    /* TODO: Implement the train process that navigates through intersections
     * 1. Process each intersection in the train's route sequentially
     * 2. For each intersection:
     *    a) Send ACQUIRE_REQUEST message to the server
     *    b) Wait for GRANT_RESPONSE from the server
     *    c) Once granted, simulate travel time through intersection
     *    d) Send RELEASE_REQUEST when done with the intersection
     *    e) Move to next intersection in route
     *
     * - Use Message struct for communication with server
     * - Handle message sending/receiving errors
     * - Keep track of current position in route
     * - Log all actions with write_log()
     * - Simulate travel time with sleep(1);
     * - Exit process when route is complete
     *
     * Message format:
     * - For ACQUIRE_REQUEST:
     *   msg.msg_type = ACQUIRE_REQUEST
     *   msg.train_name = train->name
     *   msg.intersection_name = current intersection name
     *
     * - For RELEASE_REQUEST:
     *   msg.msg_type = RELEASE_REQUEST
     *   msg.train_name = train->name
     *   msg.intersection_name = current intersection name
     */
}
