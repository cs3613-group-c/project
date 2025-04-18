/*
 * Group C
 *
 * Author: Erin Dunlap
 * Email:
 * Date:
 * Description:
 */
#ifndef LOGGER_H
#define LOGGER_H

<<<<<<< HEAD
#include "structures.h"

=======
>>>>>>> f76832e1e7b404a9d955a6d1499a60dd150fd4bf
// Start the logger and open the file we’ll write to
void init_logger(const char *filename);

// Stop the logger and close the file when we’re done
void close_logger();

// Write something to the log with the current time
void log_event(const char *format, ...);

// Add time to the fake clock
void increment_sim_time(int units);

// Get the current fake time
char *get_formatted_sim_time();

// Using this for the modification to logger file and so that we can call it for shared time
// Note to self: I think we already have handling for the shared_memory_t in header file
void set_shared_time(shared_memory_t*, time_pointer);

#endif
