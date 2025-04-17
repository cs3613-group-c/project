//  Group C
//  Author: Gabe Cornelius
//  Email: gabriel.j.cornelius@okstate.edu
//  Date: 4/6/2025
//  Description: File parsing functions, extracts data from intersections.txt
//  and trains.txt and stores it in arrays to be passed as a "Parse" struct to
//  the calling function.
// 	Additionally, passes an error code indicating which errors have occured

#include "structures.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

// The name of the file to read intersections from
#define INTERSECTION_FILE_NAME "Intersections.txt"
// The max length of our strings to read from
#define MAX_STR_LEN 256

parse_t parse_file(const char *intersections_file, const char *trains_file) {
    //
    // Initialize each field in the struct
    // route[10][26], route_count, sctn[26], sctn_count, error}
    parse_t ret = {{0}, 0, {0}, 0, 0};

    // File Format{
    //				IntersectionName:Capacity
    //  			 }

    // File Format: IntersectionName:Capacity
    char intName[MAX_STR_LEN] = INTERSECTION_FILE_NAME;
    FILE *intersections = fopen(intersections_file, "r");
    if (intersections == NULL) {
        perror(strcat(intName, " not found"));
        ret.error ^= 1;
        return ret;
    }

    char line[MAX_STR_LEN];
    int l = 0; // line number for indexing into our sctn array

    while (fgets(line, sizeof(line), intersections)) {
        char match0[] = "Intersection";
        char name = ' ';
        int size = 0;

        for (int i = 0; i < sizeof(line); i++) {
            if (line[i] == match0[i] &&
                i < sizeof(match0) - 1) { // Currently Matching Intersection
                continue;
            }

            if ((line[i] != match0[i]) &&
                i < sizeof(match0) - 1) { // fails to match 'Intersection'
                // printf("failed to match Intersection");
                ret.error ^= 2;
                return ret;
            }

            if ((line[i] != match0[i]) &&
                i == sizeof(match0) - 1) { // Finished matching 'Intersection',
                                           // starting assignments
                int j = line[i] - 'A';
                if (!(0 <= j && j < 26)) { // check if the index is valid
                    ret.error ^= 2;
                    return ret;
                }
                if (line[i + 1] !=
                    ':') { // Fails due to not found ':' at exptected location
                           // (two char Int name or missing ':')
                    ret.error ^= 2;
                    return ret;
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
    FILE *trains = fopen(trains_file, "r");
    if (trains == NULL) {
        perror("Error");
        ret.error ^= 4;
        return ret;
    }
    char *tok;

    char match1[] = "Train";
    char match2[] = ":Intersection";
    char match3[] = ",Intersection";
    // printf("sizeof(match[2]): %d\n", sizeof(match2));
    while (fgets(line, sizeof(line), trains)) {
        // printf(line);
        bool m1 = false;
        bool m2 = false;
        int train = -1;
        int offset = 0;
        int stop = 0;
        for (int i = 0, j = 0; i < sizeof(line); i++) {
            // printf("%c",line[i]);
            if (line[i] == '\n' ||
                line[i] == 0) { // break once you reach linebreak or end of file
                break;

            } else if (!m1) { // state 0

                if (match1[i] != line[i] &&
                    i < sizeof(match1) -
                            1) { // check if fails to match all of match1
                    // printf("break @ i %d\nline[i]: %c\n", i , line[i]);
                    ret.error ^= 8; // set error bit 3 for formatting issue with
                                    // trains.txt
                    break;
                }
                // if(match1[i] == line[i]) continue;

                if (match1[i] != line[i] && !m1) {
                    m1 = true;
                    j = 0;
                    if (1 <= line[i] <= 9) {
                        train = line[i] - '0';
                        ret.route_count++;
                        // printf("i %d\nline[i]: %c\n", i , line[i]);
                    } else // failed to provide valid index for trains
                        ret.error ^= 8; // set error bit 3 for formatting issue
                                        // with trains.txt
                }
            } // end state 0

            else if (!m2) { // state 1

                if (match2[j] != line[i] && j < sizeof(match2) - 1) {
                    printf("break @ i %d\nline[i]: %c\n", i, line[i]);
                    ret.error ^= 8; // set error bit 3 for formatting issue with
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
                    ret.error ^= 8; // set error bit 3 for formatting issue with
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
        if (ret.routes[i][0] == 0 &&
            count != ret.route_count) { // sets an error if you see a zero
                                        // before you expect
            printf("Error: nonconsecutive train numbers\n");
            ret.error ^= 16;
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
            ret.error ^= 32;
            break;
        }
        count++;
    }

    return ret;
}

/*
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
