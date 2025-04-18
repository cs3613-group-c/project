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

// Start the logger and open the file we’ll write to
void init_logger(const char *filename);

// Stop the logger and close the file when we’re done
void close_logger();

// Write something to the log with the current time
// You can write messages like you do with printf
void log_event(const char *format, ...);

// Add time to the fake clock
void increment_sim_time(int units);

// Get the current fake time
char *get_formatted_sim_time();

#endif
