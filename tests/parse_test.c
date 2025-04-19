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

    //print intersection data
    for (int i = 0; i < num_sctns;
         i++) { // debug print for intersection assignment
        if (sctn[i].capacity == 0)
            continue;
        printf("sctn[%d]:\nname: %s\ncapacity: %d\n",i, sctn[i].name, sctn[i].capacity);
        if(sctn[i].lock_type == LOCK_MUTEX)
            printf("lock_type: LOCK_MUTEX\n");
        else if (sctn[i].lock_type == LOCK_SEMAPHORE)
            printf("lock_type: LOCK_SEMAPHORE\n");
        else
            printf("lock_type: ERROR\n");
        printf("num_holding_trains: %d\n", sctn[i].num_holding_trains);
        printf("holding_trains: ");
        for(int j = 0; j < MAX_TRAINS; j++){ //expecting null
            printf("%s", sctn[i].holding_trains[j]);
            
        }
        printf("\n");
        
        printf("table_holding_trains:\n");
        for(int j = 0; j < MAX_TRAINS; j++){ //expecting null
            printf("     Train %d, %s\n", j+1, sctn[i].table_holding_trains[j]== true ? "TRUE" : "FALSE");
            
        }
        printf("\n\n");
    }
    
    //print train data
    for (int i = 0; i < num_trains; i++) {
        printf("\ntrain[%d]:\nname: %s\nroute_len: %d\ncurrent_position: %d\nroute:\n", i, train[i].name, train[i].route_len, train[i].current_position);
        // printf("j: %d", j);

        for (int j = 0; j < 26; j++) {
            // printf("i: %d, j: %d\n", i, j);
            if (*train[i].route[j] != 0)
                printf("     %s\n", train[i].route[j]);
        }
        printf("\n\n");
    }

    printf("route count: %d\nsctn count: %d\n\n", num_trains,
           num_sctns);

    return 0;
    
} // end main
