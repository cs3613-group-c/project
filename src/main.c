/*
 * Group C
 *
 * Author: Drake Geeteh
 * Email: drake.geeteh@okstate.edu
 * Date: 4/4/2025
 * Description: Main program. Initializes shared resources, parses input files,
 * forks train processes, parses intersections.txt & trains.txt, cleans up.
 */
#include "parse.h"
#include "structures.h"
#include "message.h"
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


/* Assumptions:
/* 1) write_log() exists
/* 2) 2 instances of message_queue_t: request_queue and response_queue
*/
void train_process(train_t *train) {
    int current_position = 0;
    char log_message[MAX_LOG_SIZE];
    
    while (current_position < train->route_len) {
        // Current intersection to acquire
        char* intersection_name = train->route[current_position];
        
        // Send acquire request
        message_t request_msg = {
            .type = REQUEST_ACQUIRE,
            .src = train->name,
            .dst = "SERVER",
            .data = {
                .acquire = intersection_name
            }
        };
        
        sprintf(log_message, "%s: Sent ACQUIRE request for %s.", train->name, intersection_name);
        write_log(log_message);
        
        // Send message
        send_request(request_queue, request_msg);
        
        // Wait for response
        message_t response = wait_for_response(response_queue, train->name);
        
        // Check response type
        if (response.type == RESPONSE_GRANT) {
            sprintf(log_message, "%s: Acquired %s. Proceeding...", train->name, intersection_name);
            write_log(log_message);
            

            // Sim travel time
            sleep(2);
            
            // Move to next position
            current_position++;
            
            // If not at the end, release the intersection
            if (current_position < train->route_length) {
                // Send release request
                message_t release_msg = {
                    .type = REQUEST_RELEASE,
                    .src = train->name,
                    .dst = "SERVER",
                    .data = {
                        .release = intersection_name
                    }
                };
                
                sprintf(log_message, "%s: Released %s.", train->name, intersection_name);
                write_log(log_message);
                
                // Send message
                send_request(request_queue, release_msg);
            }
        } else if (response.type == RESPONSE_WAIT) {
            sprintf(log_message, "%s: Waiting for %s to become available.", train->name, intersection_name);
            write_log(log_message);
            
            // Wait a bit and retry
            sleep(1);
        } else if (response.type == RESPONSE_DENY) {
            sprintf(log_message, "%s: Denied access to %s due to deadlock resolution and will yield", 
                    train->name, intersection_name);
            write_log(log_message);
            
            // In case of deadlock resolution, we sleep longer
            sleep(2);
                        
            // When a train is preempted:
            sprintf(log_message, "%s: Backing off and will retry for %s.", train->name, intersection_name);
            write_log(log_message);
        }
    }
    
    // Train has completed its route
    sprintf(log_message, "%s: Completed route.", train->name);
    write_log(log_message);
    
    exit(EXIT_SUCCESS);
}

//
void server_process() {}

int main() {
    pid_t pid;
    key_t key = ftok(".", 'R');              // IPC Key
    msgq_id = msgget(key, IPC_CREAT | 0666); // Message queue ID
    shm_id = shmget(key, sizeof(shared_memory_t),
                    IPC_CREAT | 0666); // Shared memory ID
    shared_memory = (shared_memory_t *)shmat(shm_id, NULL, 0); // Allocate shm
    log_file = fopen("simulation.log", "w"); // Open simulation.log file

    parse_t input = parse_file(
        "input/intersections.txt",
        "input/trains.txt",
        shared_memory); // FIXME: Check that these directories work
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
