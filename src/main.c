/*
 * Group C
 *
 * Author: Drake Geeteh
 * Email: drake.geeteh@okstate.edu
 * Date: 4/4/2025
 * Description: Main program. Initializes shared resources, parses input files,
 * forks train processes, parses intersections.txt & trains.txt, cleans up.
 */
#include "logger.h"
#include "message.h"
#include "parse.h"
#include "rag.h"
#include "structures.h"
#include "sync.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <unistd.h>

shared_memory_t *shared_memory;
FILE *log_file;
int msgq_id;
int shm_id;

int find_intersection(const char *inter_name) {
    for (int i = 0; i < shared_memory->num_intersections; i++) {
        if (strcmp(inter_name, shared_memory->intersections[i].name))
            return i;
    }
}

void train_process(train_t *train) {
    int current_position = 0;
    char log_message[MAX_LOG_SIZE];

    while (current_position < train->route_len) {
        // Current intersection to acquire
        char *intersection_name = train->route[current_position];

        // Send acquire request
        message_t request_msg = {
            .type = REQUEST_ACQUIRE,
            .src = train->name,
            .dst = "SERVER",
            .data = {.acquire = intersection_name}};

        sprintf(log_message, "%s: Sent ACQUIRE request for %s.", train->name, intersection_name);
        log_event(log_message);

        // Send message
        send_request(&shared_memory->request_queue, request_msg);

        // Wait for response
        message_t response = wait_for_response(&shared_memory->response_queue, train->name);

        // Check response type
        if (response.type == RESPONSE_GRANT) {
            sprintf(log_message, "%s: Acquired %s. Proceeding...", train->name, intersection_name);
            log_event(log_message);

            // Sim travel time
            // sleep(2);

            // I'm using this instead of sleep to show the delay in shared time
            increment_sim_time(2);

            // Move to next position
            current_position++;

            // If not at the end, release the intersection
            if (current_position < train->route_len) {
                // Send release request
                message_t release_msg = {
                    .type = REQUEST_RELEASE,
                    .src = train->name,
                    .dst = "SERVER",
                    .data = {.release = intersection_name}};

                sprintf(log_message, "%s: Released %s.", train->name, intersection_name);
                log_event(log_message);

                // Send message
                send_request(&shared_memory->request_queue, release_msg);
            }
        } else if (response.type == RESPONSE_WAIT) {
            sprintf(
                log_message,
                "%s: Waiting for %s to become available.",
                train->name,
                intersection_name);
            log_event(log_message);

            // Wait a bit and retry
            sleep(1);
        } else if (response.type == RESPONSE_DENY) {
            sprintf(
                log_message,
                "%s: Denied access to %s due to deadlock resolution and "
                "will yield",
                train->name,
                intersection_name);
            log_event(log_message);

            // In case of deadlock resolution, we sleep longer
            sleep(2);

            // When a train is preempted:
            sprintf(
                log_message,
                "%s: Backing off and will retry for %s.",
                train->name,
                intersection_name);
            log_event(log_message);
        }
    }

    // Train has completed its route
    sprintf(log_message, "%s: Completed route.", train->name);
    log_event(log_message);

    exit(EXIT_SUCCESS);
}

// Current assumptions:
// 1) find_intersection() exists
// 2) if detect_deadlock() --> resolve_deadlock()

void server_process() {
    // Initialize the Resource Allocation Graph
    resource_alloc_graph_t rag;
    graph_init(&rag);
    for (int i = 0; i < shared_memory->num_trains; i++) {
        graph_add_process(&rag, i);
    }
    for (int i = 0; i < shared_memory->num_intersections; i++) {
        graph_add_resource(&rag, i, shared_memory->intersections[i].capacity);
    }

    char log_message[MAX_LOG_SIZE];
    int trains_completed = 0;

    while (trains_completed < shared_memory->num_trains) {
        // Process request if any
        if (shared_memory->request_queue.length > 0) {
            // Dequeue the message
            message_t req_msg = queue_dequeue(&shared_memory->request_queue);

            // Check request type
            if (req_msg.type == REQUEST_ACQUIRE) {
                const char *train_name = req_msg.src;
                const char *intersection_name = req_msg.data.acquire;
                int intersection_idx = find_intersection(intersection_name);
                intersection_t *intersection = &shared_memory->intersections[intersection_idx];

                // Find train index
                int train_idx = -1;
                for (int i = 0; i < shared_memory->num_trains; i++) {
                    if (strcmp(shared_memory->trains[i].name, train_name) == 0) {
                        train_idx = i;
                        break;
                    }
                }

                /// Add request edge to RAG
                graph_request_edge(&rag, train_idx, intersection_idx, 1);

                // Try to acquire the intersection using our sync library
                if (try_acquire_intersection(intersection, train_name)) {
                    // Successfully acquired the intersection
                    // Update the RAG - remove request edge and add assignment edge
                    graph_remove_request(&rag, train_idx, intersection_idx, 1);
                    graph_assign_edge(&rag, train_idx, intersection_idx, 1);

                    char state_str[MAX_LOG_SIZE / 2]; // Use half the log message size for state
                    get_intersection_state(intersection, state_str);
                    snprintf(
                        log_message,
                        MAX_LOG_SIZE,
                        "SERVER: GRANTED %s to %s. %s",
                        intersection_name,
                        train_name,
                        state_str);
                    log_event(log_message);

                    // Send grant response
                    message_t resp_msg = {
                        .type = RESPONSE_GRANT,
                        .src = "SERVER",
                        .dst = train_name,
                        .data = {.grant = intersection_name}};
                    send_response(&shared_memory->response_queue, resp_msg);
                } else {
                    // Could not acquire the intersection
                    char state_str[MAX_LOG_SIZE / 2]; // Use half the log message size for state
                    get_intersection_state(intersection, state_str);
                    snprintf(
                        log_message,
                        MAX_LOG_SIZE,
                        "SERVER: %s is busy. %s added to wait queue. %s",
                        intersection_name,
                        train_name,
                        state_str);
                    log_event(log_message);

                    // Send wait response
                    message_t resp_msg = {
                        .type = RESPONSE_WAIT,
                        .src = "SERVER",
                        .dst = train_name,
                        .data = {.wait = NULL}};
                    send_response(&shared_memory->response_queue, resp_msg);

                    // Detect deadlocks - will return [train, intersection] if a deadlock was found
                    int output_array[2];
                    memcpy(
                        output_array,
                        graph_check_deadlock(&rag, output_array),
                        sizeof(output_array));

                    if (output_array[0] != -1) // checking if there is a deadlock
                    {
                        char message[1024];
                        train_t problem_train = shared_memory->trains[output_array[0]];
                        intersection_t *problem_intersection =
                            &shared_memory->intersections[output_array[1]];
                        const char *train_name = problem_train.name;
                        sprintf(
                            message,
                            "Preempting %s from %s",
                            problem_intersection->name,
                            train_name);
                        log_event(message);

                        // attempt to release intersection from train
                        release_intersection(problem_intersection, problem_train.name);
                        graph_remove_assignment(&rag, output_array[0], output_array[1], 1);
                        sprintf(
                            message,
                            "%s released %s forcibly.",
                            problem_train.name,
                            problem_intersection->name);
                        log_event(message);
                    }
                    ////////////////////
                }
            } else if (req_msg.type == REQUEST_RELEASE) {
                const char *train_name = req_msg.src;
                const char *intersection_name = req_msg.data.release;
                int intersection_idx = find_intersection(intersection_name);
                intersection_t *intersection = &shared_memory->intersections[intersection_idx];

                // Find train index
                int train_idx = -1;
                for (int i = 0; i < shared_memory->num_trains; i++) {
                    if (strcmp(shared_memory->trains[i].name, train_name) == 0) {
                        train_idx = i;
                        break;
                    }
                }

                // Release the intersection
                release_intersection(intersection, train_name);

                // Update the RAG - remove assignment edge
                graph_remove_assignment(&rag, train_idx, intersection_idx, 1);

                // Get the current state for logging
                char state_str[MAX_LOG_SIZE / 2]; // Use half the log message size for state
                get_intersection_state(intersection, state_str);
                snprintf(
                    log_message,
                    MAX_LOG_SIZE,
                    "SERVER: %s released %s. %s",
                    train_name,
                    intersection_name,
                    state_str);
                log_event(log_message);

                // Send confirmation
                message_t resp_msg = {
                    .type = RESPONSE_GRANT,
                    .src = "SERVER",
                    .dst = train_name,
                    .data = {.grant = intersection_name}};
                send_response(&shared_memory->response_queue, resp_msg);

                // Check if train has completed its route
                int completed = 1;
                for (int i = 0; i < shared_memory->num_trains; i++) {
                    if (strcmp(shared_memory->trains[i].name, train_name) == 0) {
                        if (shared_memory->trains[i].current_position >=
                            shared_memory->trains[i].route_len) {
                            trains_completed++;
                        } else {
                            completed = 0;
                        }
                        break;
                    }
                }

                if (completed) {
                    sprintf(log_message, "%s: Completed route.", train_name);
                    log_event(log_message);
                }
            }
        }
    }

    // All trains have completed
    sprintf(log_message, "SIMULATION COMPLETE. All trains reached destinations.");
    log_event(log_message);
}
//

int main() {
    pid_t pid;
    key_t key = ftok(".", 'R');              // IPC Key
    msgq_id = msgget(key, IPC_CREAT | 0666); // Message queue ID
    shm_id = shmget(key, sizeof(shared_memory_t),
                    IPC_CREAT | 0666);                         // Shared memory ID
    shared_memory = (shared_memory_t *)shmat(shm_id, NULL, 0); // Allocate shm
    // log_file = fopen("simulation.log", "w");                   // Open simulation.log file

    pthread_mutexattr_t attrb;
    pthread_mutexattr_init(&attrb);
    pthread_mutexattr_setpshared(&attrb, PTHREAD_PROCESS_SHARED);
    pthread_mutex_init(&shared_memory->time_mutex, &attrb);
    shared_memory->sim_time = 0;

    set_shared_time(shared_memory);
    init_logger("simulation.log");

    // FIXME: Check that these directories work
    // Parse input file data into shared memory: intersections, trains, num_intersections,
    // num_trains
    if (parse_file(
            "input/intersections.txt",
            "input/trains.txt",
            shared_memory->intersections,
            shared_memory->trains,
            &shared_memory->num_intersections,
            &shared_memory->num_trains) > 0) {
        printf("parse completed with errors\n\n");
    } else
        printf("parse completed without errors\n\n");

    // Fork one process per train
    // Author: Erin Dunlap
    for (int i = 0; i < shared_memory->num_trains; i++) {
        pid = fork();
        if (pid == 0) { // Child process (trains)
            train_process(&shared_memory->trains[i]);
            exit(EXIT_SUCCESS);
        }
    }

    // Parent process (server)
    server_process();

    // Wait for all train processes to finish
    for (int i = 0; i < shared_memory->num_trains; i++) {
        wait(NULL);
    }

    // Cleanup
    // fclose(log_file);
    close_logger();
    shmdt(shared_memory);
    msgctl(msgq_id, IPC_RMID, NULL);
    shmctl(shm_id, IPC_RMID, NULL);
    return 0;
}
