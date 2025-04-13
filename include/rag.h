// Author: Em Jordan
#ifndef RAG_H
#define RAG_H

#include "constants.h"
#include <stdbool.h>

typedef struct {
  int id;
  int current_allocs[MAX_PROCESSES];
  int max_allocs[MAX_PROCESSES];
} resource_t;

typedef struct {
  int id;
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
int graph_add_resource(resource_alloc_graph_t *graph, int resource_id);

// Tries to allocate `count` of the given resource at to the given process
// This function will return 0 on success or a negative number on an error 
int graph_alloc(resource_alloc_graph_t *graph, int process_id, int resource_id, int count);

// Checks if the given RAG has a deadlock 
bool graph_check_deadlock(resource_alloc_graph_t *graph);

#endif
