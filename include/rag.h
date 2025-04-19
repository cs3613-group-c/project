/*
 * Group C
 *
 * Authors: Cade Blakeman, Em Jordan
 * Date:
 *
 * Description:
 *  - RAG - Resource Allocation Graph
 *
 *  Creates a resource allocation graph that can be assigned edges depending on
 *  resource allocation or requests Uses the graph to detect cycles within the
 *  trains
 */
#ifndef RAG_H
#define RAG_H

#include "constants.h"
#include <stdbool.h>

typedef struct {
    // The ID of the resource
    int id;
    // The current allocations of this resource by a given resource
    // e.g., if process 0 had 4 instances of this resource, it would look like:
    // current_allocs[0] = 4;
    int current_allocs[MAX_PROCESSES];
    // The current amount of resources allocated
    int current_count;
    // The max number of allocations that this resource can give out
    int max_count;
} resource_t;

typedef struct {
    // The ID of the process
    int id;
    // A mapping of resource_id -> resources requested
    // e.g., this process is requesting 2 instances from resource 4 would be:
    // request_list[4] = 2;
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

//Function to be called from the server periodically
//Checks the graph for deadlocks, if there is one, resolves it and pre-empts the problem process of a resource
int deadlock_detection(resource_alloc_graph_t *graph);

// Initializes the data in our graph to default values
void graph_init(resource_alloc_graph_t *graph);

// Adds a process to our current graph.
// This function will return 0 on success or a negative number on an error
int graph_add_process(resource_alloc_graph_t *graph, int process_id);

// Adds a resource to our current graph
// This function will return 0 on success or a negative number on an error
int graph_add_resource(resource_alloc_graph_t *graph, int resource_id,
                       int max_count);

// Tries to allocate `count` of the given resource at to the given process
// This function will return 0 on success or a negative number on an error
int graph_alloc(resource_alloc_graph_t *graph, int process_id, int resource_id,
                int count);

//creates an assignment edge in the graph
//returns a 0 on success or a negative number on an error
int graph_assign_edge(resource_alloc_graph_t *graph, int process_id, int resource_id,
                int count);

//creates a request edge in the graph
//returns a 0 on success or a negative number on an error
int graph_request_edge(resource_alloc_graph_t *graph, int process_id, int resource_id,
                int count);

//removes an assignment edge in the graph
//returns a 0 on success or a negative number on an error
int graph_remove_assignment(resource_alloc_graph_t *graph, int process_id, int resource_id,
                int count);

//removes a request edge in the graph
//returns a 0 on success or a negative number on an error
int graph_remove_request(resource_alloc_graph_t *graph, int process_id, int resource_id,
                int count);

//prints the graph, mostly for bug fixing
void print_graph(resource_alloc_graph_t *graph);

// Checks the graph for a deadlock, will return 1 if it finds one, or a 0 if it
// does not In order for a deadlock to be guarenteed, it there must be a cycle
// where each resource of that cycle is a mutex NOTE(Cade): This is technically
// more than specifications, which just ask for detecting cycles, so I might
// change this in the future
int *graph_check_deadlock(resource_alloc_graph_t *graph, int* cycle_list);

// Checks if there is a circular reference in our graph
int *graph_detect_cycle(resource_alloc_graph_t *graph, bool *visited,
                       int visited_len, bool *recursed, int recursed_len,
                       int process_id, int *cycle_list, int offset);

int resolve_deadlock(resource_alloc_graph_t *graph, int *cycle_list);
#endif
