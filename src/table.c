/*
 * Group C
 *
 * Author: Gabe Cornelius
 * Email: gabriel.j.cornelius@okstate.edu
 * Date: 4/6/2025
 * Description: Resource allocation table methods to appropriately add and remove items from the structs
 */
#include "table.h"
#include "structures.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
// shared_memory_t *mem;

// int table_init(intersection_t *sctns, train_t *train) {

//     mem = m;
//     /* (*mem).num_intersections = parse.sctn_count;
//     (*mem).num_trains = parse.route_count;
//     this is handled in parse now FIXME: delete
//  */
//     for (int i = 0; i < MAX_TRAINS; i++) {

//         sctns[i].capacity = parse.sctn[i];
//         sctns[i].num_holding_trains = 0;

//         for (int j = 0; j < MAX_TRAINS; j++) { // Train holding initializer

//             sctns[i].table_holding_trains[j] = false;
//         }
//     }

//     return 0;
// } //end table_init

// bool table_is_sctn_locked(int i) { // return 1 if locked and 0 otherwise
//     return (sctns[i].num_holding_trains > sctns[i].capacity);
// }

void table_add_train(intersection_t *sctn, train_t *train) {
    
    //increment intersection's train count
    if(sctn->num_holding_trains < sctn->capacity && !sctn->table_holding_trains[train->index]){ //check that the train is not yet in the intersection
            
        (*sctn).num_holding_trains++;
        (*sctn).table_holding_trains[train->index] = true;
        
        //Add train and intersection to each other's tracking array
        strcpy(train->holding_intersections[sctn->index], sctn->name);
        strcpy(sctn->holding_trains[train->index], train->name);
        
        if(sctn->num_holding_trains >= sctn->capacity)
            sctn->is_locked = true;
        
        //remove train and intersection from each others' waiting arrays in case of entries
        strcpy(sctn->waiting_trains[train->index], "0");
        strcpy(train->waiting_intersections[sctn->index], "0");
        
    }
    
}

void table_rem_train(intersection_t *sctn, train_t *train) {
    if(sctn->table_holding_trains[train->index]){ //check that the train is in the intersection
        
    //decrement intersection's train count
    (*sctn).num_holding_trains--;
    (*sctn).table_holding_trains[train->index] = false;
    
    //Remove the train and intersection from their tracking array
    strcpy(train->holding_intersections[sctn->index], "0");
    strcpy(sctn->holding_trains[train->index], "0");
    
    //Set the lock status according to the new count
    if(sctn->num_holding_trains < sctn->capacity)
        sctn->is_locked = false;
    }
    
   
}

void table_print(intersection_t *sctns, train_t *trains, int num_sctns) {


    printf("Intersection: ID | Capacity | Lock State | Held Trains | Holding Trains\n");

    for (int i = 0, j = 0; i < num_sctns; i++, j = 0) {
        printf(
            "Intersection: %c  |    %d     | %s |      %d      |  ",
            (i + 'A'),
            sctns[i].capacity,
            sctns[i].is_locked ? "  Locked  " : " Unlocked ",
            sctns[i].num_holding_trains);
        printf("[");
        if (sctns[i].table_holding_trains[j])
            printf(" Train %d", j + 1);
        for (int j = 1; j < MAX_TRAINS; j++) {
            // printf("%d", sctns[i].table_holding_trains[j]);
            if (sctns[i].table_holding_trains[j])
                printf(", Train %d", j + 1);
        }
        printf(" ]\n");
    }
    
}
