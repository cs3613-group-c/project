#include <stdbool.h>
#include "../include/rag.h"

// Initializes the data in our graph to default values
void graph_init(resource_alloc_graph_t *graph) {
    graph->resources_len = 0;
    graph->processes_len = 0;
} 

// Adds a process to our current graph.
// This function will return 0 on success or a negative number on an error 
int graph_add_process(resource_alloc_graph_t *graph, int process_id) {
    if (graph->processes_len >= MAX_PROCESSES) {
        return -1; 
    }
    // graph->processes[graph->processes_len] = {}; 
    return 0;
}

// Adds a resource to our current graph
// This function will return 0 on success or a negative number on an error 
int graph_add_resource(resource_alloc_graph_t *graph, int resource_id) {
    return 0;
}

// Tries to allocate `count` of the given resource at to the given process
// This function will return 0 on success or a negative number on an error 
int graph_alloc(resource_alloc_graph_t *graph, int process_id, int resource_id, int count) {

    return 0;
}

// Tries to allocate `count` of the given resource at to the given process
// This function will return 0 on success or a negative number on an error 
bool graph_check_deadlock(resource_alloc_graph_t *graph) {

    return false;
}
