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
#include "structures.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <unistd.h>

shared_memory_t *shared_memory;
FILE *log_file;
int msgq_id;
int shm_id;

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
            sleep(2);

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
        if (request_queue->length > 0) {
            // Dequeue the message
            message_t req_msg = queue_dequeue(request_queue);
            
            // Check request type
            if (req_msg.type == REQUEST_ACQUIRE) {
                const char* train_name = req_msg.src;
                const char* intersection_name = req_msg.data.acquire;
                int intersection_idx = find_intersection(intersection_name);
                intersection_t* intersection = &shared_memory->intersections[intersection_idx];
                
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
                    
                    char state_str[MAX_LOG_SIZE/2]; // Use half the log message size for state
                    get_intersection_state(intersection, state_str);
                    snprintf(log_message, MAX_LOG_SIZE, "SERVER: GRANTED %s to %s. %s", 
                            intersection_name, train_name, state_str);
                    log_event(log_message);
                    
                    // Send grant response
                    message_t resp_msg = {
                        .type = RESPONSE_GRANT,
                        .src = "SERVER",
                        .dst = train_name,
                        .data = {
                            .grant = intersection_name
                        }
                    };
                    send_response(response_queue, resp_msg);
                } else {
                    // Could not acquire the intersection
                    char state_str[MAX_LOG_SIZE/2]; // Use half the log message size for state
                    get_intersection_state(intersection, state_str);
                    snprintf(log_message, MAX_LOG_SIZE, "SERVER: %s is busy. %s added to wait queue. %s", 
                            intersection_name, train_name, state_str);
                    log_event(log_message);
                    
                    // Send wait response
                    message_t resp_msg = {
                        .type = RESPONSE_WAIT,
                        .src = "SERVER",
                        .dst = train_name,
                        .data = {
                            .wait = NULL
                        }
                    };
                    send_response(response_queue, resp_msg);
                    
                    // TODO: Check for deadlocks
                    if (detect_deadlock()) {
                        resolve_deadlock();
                    }
                    ////////////////////
                }
            } else if (req_msg.type == REQUEST_RELEASE) {
                const char* train_name = req_msg.src;
                const char* intersection_name = req_msg.data.release;
                int intersection_idx = find_intersection(intersection_name);
                intersection_t* intersection = &shared_memory->intersections[intersection_idx];
                
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
                char state_str[MAX_LOG_SIZE/2]; // Use half the log message size for state
                get_intersection_state(intersection, state_str);
                snprintf(log_message, MAX_LOG_SIZE, "SERVER: %s released %s. %s", 
                        train_name, intersection_name, state_str);
                log_event(log_message);
                
                // Send confirmation
                message_t resp_msg = {
                    .type = RESPONSE_GRANT,
                    .src = "SERVER",
                    .dst = train_name,
                    .data = {
                        .grant = intersection_name
                    }
                };
                send_response(response_queue, resp_msg);
                
                // Check if train has completed its route
                int completed = 1;
                for (int i = 0; i < shared_memory->num_trains; i++) {
                    if (strcmp(shared_memory->trains[i].name, train_name) == 0) {
                        if (shared_memory->trains[i].current_position >= shared_memory->trains[i].route_length) {
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
    log_file = fopen("simulation.log", "w");                   // Open simulation.log file

    // FIXME: Check that these directories work
    parse_t input = parse_file("input/intersections.txt", "input/trains.txt", shared_memory);

    if (input.error > 0) {
        printf("Issue with config file\n");
        return 1;
    }

    // Fork one process per train
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
    fclose(log_file);
    shmdt(shared_memory);
    msgctl(msgq_id, IPC_RMID, NULL);
    shmctl(shm_id, IPC_RMID, NULL);
    return 0;
}
