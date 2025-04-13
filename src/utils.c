/*
* Group C
*
* Author:
* Email:
* Date:
* Description:
*
*/
#include <stdio.h>
#include <pthread.h>
#include "../include/utils.h"

extern shared_memory_t *shared_memory;
extern FILE *log_file;

void format_sim_time(int sim_time, char* formatted_time) {
    int hours = sim_time / 3600;
    int minutes = (sim_time % 3600) / 60;
    int seconds = sim_time % 60;
    sprintf(formatted_time, "[%02d:%02d:%02d]", hours, minutes, seconds);
}

void write_log(const char* message) {
    /* TODO: Implement logging
     * 1. Lock the time mutex to safely increment sim_time
     * 2. Format the current simulation time
     * 3. Write the formatted message to both log file and console
     * 4. Flush the log file
     * 
     * Format: [HH:MM:SS] <message>
     * 
     * Use pthread_mutex_lock/unlock for time_mutex
     * Use format_sim_time() for time formatting
     * Write to both log_file and console (printf)
     * Remember to flush the log file
     */
}

int find_intersection(const char* name) {
    /* TODO: Implement intersection lookup
     * 1. Search through shared_memory->intersections array
     * 2. Return the index of the intersection with matching name
     * 3. Return -1 if not found
     * 
     * - Use strcmp()
     * - Check all intersections up to shared_memory->num_intersections
     */
    return -1;
}
