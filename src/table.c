/*
 * Group C
 *
 * Author: Gabe Cornelius
 * Email: gabriel.j.cornelius@okstate.edu
 * Date: 4/6/2025
 * Description:
 */
#include "table.h"
#include "structures.h"
#include <stdbool.h>
#include <stdio.h>

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

void table_sctn_add_train(intersection_t *sctn, train_t *train) {

    (*sctn).num_holding_trains++;
}

// void table_sctn_rem_train(int sctns, int train) {

//     if ((sctns[sctn]
//             .table_holding_trains[train]) { // FIXME: Fairly sure this makes a race condition and
//                                             // this check may be done somewhere else

//         sctns[sctn].num_holding_trains--;
//         sctns[sctn].table_holding_trains[train] = false;
//     }
// }

// void table_print() {

//     printf("table print\n");

//     char lock[] = "locked";
//     printf("Intersection: ID | Capacity | Lock State | Holding Trains\n");

//     for (int i = 0, j = 0; i < (*mem).num_intersections; i++, j = 0) {

//         printf(
//             "Intersection: %c  |    %d     | %s |  ",
//             (i + 'A'),
//             sctns[i].capacity,
//             table_is_sctn_locked(i) ? "  Locked  " : " Unlocked ");
//         printf("[");
//         if (sctns[i].table_holding_trains[j])
//             printf(" Train %d", j + 1);
//         for (int j = 1; j < MAX_TRAINS; j++) {
//             // printf("%d", sctns[i].table_holding_trains[j]);
//             if (sctns[i].table_holding_trains[j])
//                 printf(", Train %d", j + 1);
//         }
//         printf(" ]\n");
//     }
// }
