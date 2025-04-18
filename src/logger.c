/*
 * Group C
 *
 * Author: Erin Dunlap
 * Email:
 * Date:
 * Description:
 */
#include "logger.h"
#include <pthread.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

// Static variables used internally by the logger
static FILE *log_file = NULL; // pointer to the log file
static int sim_time = 0;      // simulated time counter in seconds
static pthread_mutex_t time_mutex =
    PTHREAD_MUTEX_INITIALIZER; // This lock makes sure only one thing changes
                               // the time at a time

// Start the logger by opening the file where we’ll write stuff
void init_logger(const char *filename) {
    log_file = fopen(filename, "w");
    if (!log_file) {
        perror("Failed to open log file");
        exit(1);
    }
}

// Closes the log file when logging is done
void close_logger() {
    // Return early if we don't have a log file
    if (!log_file)
        return;
    fclose(log_file);
    log_file = NULL;
}

// Add time to our fake stopwatch
void increment_sim_time(int units) {
    pthread_mutex_lock(
        &time_mutex);  // Lock the stopwatch so no one else changes it
    sim_time += units; // Add time (like 1 or 2 seconds)
    pthread_mutex_unlock(&time_mutex); // Unlock when done
}

// Current simulated time is returned in the [HH:MM:SS] format
char *get_formatted_sim_time() {
    static char buffer[16];
    pthread_mutex_lock(
        &time_mutex); // Lock time so it doesn't change while we read

    int hours = sim_time / 3600;
    int minutes = (sim_time % 3600) / 60;
    int seconds = sim_time % 60;

    pthread_mutex_unlock(&time_mutex); // Unlock time again
    snprintf(buffer, sizeof(buffer), "[%02d:%02d:%02d]", hours, minutes,
             seconds);
    return buffer;
}

// This will log the formatted message with the current simulated time
void log_event(const char *format, ...) {
    if (!log_file)
        return;

    char buffer[1024]; // Make space for the message
    va_list args;

    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format,
              args); // Formatting the message with passed arguments
    va_end(args);

    fprintf(log_file, "%s %s\n", get_formatted_sim_time(), buffer);
    fflush(log_file); // Implementing to immediately write to file
}
