// Erin Dunlap
#include "constants.h"
#include "structures.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

// These functions should be implemented elsewhere
extern void simulate_train(const char *train_name,
                           char route[][MAX_NAME_LENGTH], int route_len);
extern void run_server(); // Parent process becomes server after forking

void fork_trains(train_t *train_list, int train_count) {
    pid_t pids[MAX_TRAINS];

    for (int i = 0; i < train_count; i++) {
        pid_t pid = fork();

        if (pid < 0) {
            perror("Fork failed");
            exit(EXIT_FAILURE);
        } else if (pid == 0) {
            // Utilizing the child processes to run train simulation
            simulate_train(train_list[i].name, train_list[i].route,
                           train_list[i].route_len);
            exit(EXIT_SUCCESS);
        } else {
            // Storing parent PID
            pids[i] = pid;
        }
    }

    // Making sure the parent process runs as a central controller
    printf("Parent process has forked all trains. Running parent process as "
           "central controller.\n");
    run_server();

    // Using this to wait for all child processes to finish
    for (int i = 0; i < train_count; i++) {
        waitpid(pids[i], NULL, 0);
    }

    printf(
        "All train processes completed and shutting down parent controller.\n");
}
