#include "constants.h"
#include "parse.h"
#include "structures.h"
#include <stdbool.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/mman.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

shared_memory_t *m;
int main() {
    
    parse_t ret = {{0}, 0, {0}, 0, 0};
    intersection_t sctn[MAX_INTERSECTIONS] = {0};
    train_t train[MAX_TRAINS] = {0};
    int num_sctns = 0;
    int num_trains = 0;
    
    if(parse_file("input/intersections.txt", "input/trains.txt", sctn, train, &num_sctns, &num_trains) > 0){
        printf("parse completed with errors\n\n");
    }
    else
        printf("parse completed without errors\n\n");

    for (int i = 0; i < num_sctns;
         i++) { // debug print for intersection assignment
        if (sctn[i].capacity == 0)
            continue;
        printf("Intersection = %c, size = %d\n", i + 'A', sctn[i].capacity);
    }

    for (int i = 0; i < num_trains; i++) {
        printf("\nTrain %d:", i + 1);
        // printf("j: %d", j);

        for (int j = 0; j < 26; j++) {
            // printf("i: %d, j: %d\n", i, j);
            if (*train[i].route[j] != 0)
                printf(" %s, ", train[i].route[j]);
        }
        printf("\n");
    }

    printf("route count: %d\nsctn count: %d\n\n", num_trains,
           num_sctns);

    return 0;
    
} // end main
