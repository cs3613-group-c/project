// Author: Em Jordan
#ifndef RAG_H
#define RAG_H

#include "constants.h"
#include <stdbool.h>

typedef struct {
  // The ID of the resource
  int id;
  // The current allocations of this resource by a given resource
  // e.g., if process 0 had 4 instances of this resource, it would look like: current_allocs[0] = 4;
  int current_allocs[MAX_PROCESSES];
  // The max number of allocations that this resource can give out
  int max_count;
} resource_t;

typedef struct {
  // The ID of the process
  int id;
  // A mapping of resource_id -> resources requested
  // e.g., this process is requesting 2 instances from resource 4 would be: request_list[4] = 2;
  int request_list[MAX_RESOURCES];
} process_t;

typedef struct {
  // The list of processes used in the program
  process_t processes[MAX_PROCESSES];
  // The number of processes we have in our `processes` 
  int processes_len;
  // The list of resources used in the program
  resource_t resources[MAX_RESOURCES];
  // The number of resources we have in our `resources` 
  int resources_len;
} resource_alloc_graph_t;

// Initializes the data in our graph to default values
void graph_init(resource_alloc_graph_t *graph); 

// Adds a process to our current graph.
// This function will return 0 on success or a negative number on an error 
int graph_add_process(resource_alloc_graph_t *graph, int process_id);

// Adds a resource to our current graph
// This function will return 0 on success or a negative number on an error 
int graph_add_resource(resource_alloc_graph_t *graph, int resource_id, int max_count);

// Tries to allocate `count` of the given resource at to the given process
// This function will return 0 on success or a negative number on an error 
int graph_alloc(resource_alloc_graph_t *graph, int process_id, int resource_id, int count);

// Checks if the given RAG has a deadlock 
bool graph_check_deadlock(resource_alloc_graph_t *graph);

#endif
