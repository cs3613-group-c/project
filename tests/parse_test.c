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



int main() {
    
    
    parse_t ret = {{0}, 0, {0}, 0, 0};
    intersection_t sctns[MAX_INTERSECTIONS] = {0};
    train_t trains[MAX_TRAINS] = {0};
    int num_sctns = 0;
    int num_trains = 0;
    
    if(parse_file("input/intersections.txt", "input/trains.txt", sctns, trains, &num_sctns, &num_trains) > 0){
        printf("parse completed with errors\n\n");
    }
    else
        printf("parse completed without errors\n\n");

    //print intersection data
    for (int i = 0; i < num_sctns; i++) {
         intersection_print_status(&sctns[i]);
        
    }
    
    //print train data
    for (int i = 0; i < num_trains; i++) {
       train_print_status(&trains[i]);
    }
        printf("\n\n");

    printf("route count: %d\nintersection count: %d\n\n", num_trains,
           num_sctns);

    return 0;
    
} // end main
