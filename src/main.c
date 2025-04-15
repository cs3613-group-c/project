/*
 * Group C
 *
 * Author: Drake Geeteh
 * Email: drake.geeteh@okstate.edu
 * Date: 4/4/2025
 * Description: Main program. Initializes shared resources, parses input files,
 * forks train processes, parses intersections.txt & trains.txt, cleans up.
 *
 */

#include "../include/ipc.h"
#include "../include/parse.h"
#include "../include/structures.h"
#include "../include/table.h"
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

int main() {
    pid_t pid;
    key_t key = ftok(".", 'R');              // IPC Key
    msgq_id = msgget(key, IPC_CREAT | 0666); // Message queue ID
    shm_id = shmget(key, sizeof(shared_memory_t),
                    IPC_CREAT | 0666); // Shared memory ID
    shared_memory = (shared_memory_t *)shmat(shm_id, NULL, 0); // Allocate shm
    log_file = fopen("simulation.log", "w"); // Open simulation.log file

    Parse input = fileparse(
        "input/intersections.txt",
        "input/trains.txt"); // FIXME: Check that these directories work
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
