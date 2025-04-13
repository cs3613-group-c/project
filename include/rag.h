// Author: Em Jordan
#ifndef RAG_H
#define RAG_H
#include "constants.h"

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

// Adds a process to our current graph
void add_process(resource_alloc_graph_t *graph);

#endif
