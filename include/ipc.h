/*
 * Group C
 *
 * Description: Set of IPC functions in the system. Add as needed.
 *
 */
#ifndef IPC_H
#define IPC_H

#include "structures.h"

// Parse config files
void parse_config_files(const char *intersections_file,
                        const char *trains_file);

// Train process
void train_process(train_t *train);

// Server process
void server_process();

// Detect deadlock
int detect_deadlock();
void resolve_deadlock();

#endif
