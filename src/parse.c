/*
 * Group C
 *
 * Author: Gabe Cornelius
 * Email: gabriel.j.cornelius@okstate.edu
 * Date: 4/6/2025
 * Description: File parsing functions, extracts data from intersections.txt
 * and trains.txt and stores it in arrays to be passed as a "Parse" struct to
 * the calling function.
 * Additionally, passes an error code indicating which errors have occured
 */
#include "constants.h"
#include "structures.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <semaphore.h>
#include <pthread.h>
#include <stdlib.h>

// The name of the file to read intersections from
#define INTERSECTION_FILE_NAME "Intersections.txt"
// The max length of our strings to read from
#define MAX_STR_LEN 256

void init_intersection_sync(intersection_t *intersection); //Redeclaring this here from sync.h because of random linker error I couldn't fix :(

void init_intersection_sync(intersection_t *intersection) {
    if (intersection->lock_type == LOCK_MUTEX) {
        intersection->lock_data =
            (intersection_lock_t *)malloc(sizeof(intersection_lock_t));
        if (!intersection->lock_data) {
            perror("Failed to allocate mutex");
            exit(EXIT_FAILURE);
        }
        intersection->lock_data->mutex =
            (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
        pthread_mutex_init(intersection->lock_data->mutex, NULL);
    } else {
        intersection->lock_data =
            (intersection_lock_t *)malloc(sizeof(intersection_lock_t));
        if (!intersection->lock_data) {
            perror("Failed to allocate semaphore");
            exit(EXIT_FAILURE);
        }
        intersection->lock_data->semaphore = (sem_t *)malloc(sizeof(sem_t));
        sem_init(intersection->lock_data->semaphore, 1, intersection->capacity);
    }
}


int parse_file(const char *sctns_filename, const char *trains_filename, intersection_t *sctns, train_t *trains, int *num_sctns, int *num_trains) {
    int error = 0;
    // trains[0].route_len = 8;//DEBUG
    
    // Initialize each field in the struct
    // route[10][26], route_count, sctn[26], sctn_count, error
    parse_t ret = {{0}, 0, {0}, 0, 0};

    // File Format{
    //				IntersectionName:Capacity
    //  			 }

    // File Format: IntersectionName:Capacity
    char intName[MAX_STR_LEN] = INTERSECTION_FILE_NAME;
    FILE *sctns_file = fopen(sctns_filename, "r");
    if (sctns_file == NULL) {
        perror(strcat(intName, " not found"));
        error ^= 1;
        return error;
    }

    char line[MAX_STR_LEN];
    int l = 0; // line number for indexing into our sctn array

    while (fgets(line, sizeof(line), sctns_file)) {
        char match0[] = "Intersection";
        char name = ' ';
        int size = 0;

        for (int i = 0; i < sizeof(line); i++) {
            if (line[i] == match0[i] && i < sizeof(match0) - 1) { // Currently Matching Intersection
                continue;
            }

            if ((line[i] != match0[i]) && i < sizeof(match0) - 1) { // fails to match 'Intersection'
                // printf("failed to match Intersection");
                error ^= 2;
                return error;
            }

            if ((line[i] != match0[i]) &&
                i == sizeof(match0) - 1) { // Finished matching 'Intersection',
                                           // starting assignments
                int j = line[i] - 'A';
                if (!(0 <= j && j < 26)) { // check if the index is valid
                    error ^= 2;
                    return error;
                }
                if (line[i + 1] !=
                    ':') { // Fails due to not found ':' at exptected location
                           // (two char Int name or missing ':')
                    error ^= 2;
                    return error;
                }
                if (0 <= j && j < 26) { // Successful assignment
                    ret.sctn[j] = line[i + 2] - '0';

                    ret.sctn_count++;
                    break;
                }
            }
        }
    } // end while

    // for(int i = 0; i < 26; i++){ //debug print for intersection assignment
    // 	if(ret.sctn[i] == 0) continue;
    // 	printf("Intersection = %c, size = %d\n", i + 'A',ret.sctn[i]);
    // }

    // Purpose: Define train names and their routes (ordered list of
    // intersections). Format: TrainName:Intersection1,Intersection2,...
    FILE *trains_file = fopen(trains_filename, "r");
    if (trains_file == NULL) {
        perror("Error");
        error ^= 4;
        return error;
    }
    char *tok;

    char match1[] = "Train";
    char match2[] = ":Intersection";
    char match3[] = ",Intersection";
    // printf("sizeof(match[2]): %d\n", sizeof(match2));
    while (fgets(line, sizeof(line), trains_file)) {
        // printf(line);
        bool m1 = false;
        bool m2 = false;
        int train = -1;
        int offset = 0;
        int stop = 0;
        for (int i = 0, j = 0; i < sizeof(line); i++) {
            // printf("%c",line[i]);
            if (line[i] == '\n' || line[i] == 0) { // break once you reach linebreak or end of file
                break;

            } else if (!m1) { // state 0

                if (match1[i] != line[i] &&
                    i < sizeof(match1) - 1) { // check if fails to match all of match1
                    // printf("break @ i %d\nline[i]: %c\n", i , line[i]);
                    error ^= 8; // set error bit 3 for formatting issue with
                                    // trains.txt
                    break;
                }
                // if(match1[i] == line[i]) continue;

                if (match1[i] != line[i] && !m1) {
                    m1 = true;
                    j = 0;
                    if (('1' <= line[i]) && (line[i]<= '9')) {
                        
                        train = line[i] - '0';
                        ret.route_count++;
                        // printf("i %d\nline[i]: %c\n", i , line[i]);
                    } else // failed to provide valid index for trains
                        error ^= 8; // set error bit 3 for formatting issue
                                        // with trains.txt
                }
            } // end state 0

            else if (!m2) { // state 1

                if (match2[j] != line[i] && j < sizeof(match2) - 1) {
                    printf("break @ i %d\nline[i]: %c\n", i, line[i]);
                    error ^= 8; // set error bit 3 for formatting issue with
                                    // trains.txt
                    break;
                }

                // if(match2[j] == line[i]) continue;

                if (match2[j] != line[i]) {
                    m2 = true;
                    j = 0;
                    // printf("\nTrain: %d\nFirst junct: %d\n",train, line[i] -
                    // 'A');
                    ret.routes[train - 1][stop] = line[i];
                    stop++;
                    continue; // skip iterating j
                }
                j++; // iterate j while still scanning match

            } // end state 1

            else { // state 2, looping reading ",Intersection" / match3
                // i = 20 on first occurence

                // printf("I: %d\nJ: %d\n", i, j);
                // printf("Line[I] : %c\n", line[i]);
                // printf("Match[J] : %c\n", match3[j]);
                if (match3[j] != line[i] && j < sizeof(match2) - 1) {
                    // printf("break @@ i %d\nline[i]: %c\n", i , line[i]);
                    error ^= 8; // set error bit 3 for formatting issue with
                                    // trains.txt
                    break;
                }

                if (match3[j] != line[i]) {
                    // printf("\nTrain: %djunct: %d\n",train, line[i] - 'A');
                    ret.routes[train - 1][stop] = line[i];
                    stop++;
                    j = 0;
                    continue;
                }

                j++;
            }

        } // end for each character in line

    } // end line reader while

    for (int i = 0, count = 0; i < 9; i++) { // Check for consecutive in route

        if (ret.routes[i][0] == 0 && count == ret.route_count) {
            break;
        }
        if (ret.routes[i][0] == 0 && count != ret.route_count) { // sets an error if you see a zero
                                                                 // before you expect
            printf("Error: nonconsecutive train numbers\n");
            error ^= 16;
            break;
        }

        count++;
    }

    for (int i = 0, count = 0; i < 26; i++) {

        if (ret.sctn[i] == 0 && count == ret.sctn_count) {
            break;
        }
        if (ret.sctn[i] == 0 && count != ret.sctn_count) {
            printf("Error: nonconsecutive intersection letters\n");
            error ^= 32;
            break;
        }
        count++;
    }
    
        
    //BEGIN intersection assignments
    *num_sctns = ret.sctn_count;
    // printf("\n\nDebug print num_sctns: %d\n\n", *num_sctns); //FIXME: DEBUG
    for(int i = 0; i < MAX_INTERSECTIONS; i++){ //initialize
        // *sctn
        sctns[i].capacity = ret.sctn[i];
        if(sctns[i].capacity != 0){ //check for which intersections are valid and assign them initial values
            
            //assign intersection names
            sprintf(sctns[i].name, "Intersection %c", i+ 'A');
            // printf("%s\n", sctns[i].name); //DEBUG
            
            //assign intersection lock type
            if(sctns[i].capacity == 1){
                sctns[i].lock_type = LOCK_MUTEX;
            }
            else
                sctns[i].lock_type = LOCK_SEMAPHORE;
            
            init_intersection_sync(&sctns[i]);
                                    
        }
        
        //Initialize holding_trains data members: holding_trains, table_holding_trains, num_holding_trains
        
        sctns[i].num_holding_trains = 0;
        for(int j = 0; j < MAX_TRAINS; j++){
            sctns[i].table_holding_trains[j] = false;
            sprintf(sctns[i].holding_trains[j], "%d", 0);
        }        
    }
    //END intersection assignments
    
    //BEGIN train assignments
    *num_trains = ret.route_count;
    for(int i = 0, len = 0; i < MAX_TRAINS; i++, len = 0){
        if(ret.routes[i][0] != 0){ //check for valid intersections only
            
            //Assign train name
            sprintf(trains[i].name, "Train %d", i+1);
            // printf("%s |", trains[i].name); //FIXME: DEBUG PRINT
            
            //Count and assign route_len and route
            for(int j = 0; j < MAX_INTERSECTIONS; j++){
                
                if(ret.routes[i][j] != 0){
                       
                    sprintf(trains[i].route[j], "Intersection %c", ret.routes[i][j]);
                    len++;
                
                }
                
                else
                    break;
                
            }
            trains[i].current_position = 0; //FIXME: consider initializing this to -1
            trains[i].route_len = len;
            // printf("%d\n", trains[i].route_len);
            
            
        }
    }
    
    return error;
}



/* FIXME: check that these are up to date
Error Codes:
        bit 0: (error & 1) {complete}
                Catastrophic, failed to open intersections
        bit 1: (error & 2) {complete}
                Catastrophic, formatting issue with intersections
        bit 2: (error & 4) {complete}
                Catastrophic, failed to open trains.txt
        bit 3: (error & 8) {complete}
                Catastrophic, formatting issue with trains.txt
        bit 4: (error & 16) {complete}
                Non-consecutive train numbers
        bit 5:  (error & 32) {complete}
                Non-consecutive intersection letters
        bit 6:
                Warning only, double assignment with trains
        bit ?:
                check if intersections have non-zero sizes, recoverable
dependent on if trains.txt references a stop at a zero intersection bit ?: check
for duplicate route stops
 */
