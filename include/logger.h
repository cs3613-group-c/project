/*
 * Group C
 *
 * Author: Erin Dunlap
 * Email: erin.dunlap10@okstate.edu
 * Date: 4/6/2025
 * Description: header file that defines the functions for logging events in the simulation.
 * it handles handles opening and closing up the log file, writing messages with timestamps, 
 * and keeping track of a shared simulated clock
 */
#ifndef LOGGER_H
#define LOGGER_H
#include "structures.h"

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

// Using this for the modification to logger file and so that we can call it for
// shared time Note to self: I think we already have handling for the
// shared_memory_t in header file
void set_shared_time(shared_memory_t *shared_mem);

#endif
