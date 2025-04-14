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
    graph->processes[graph->processes_len] = (process_t){ .id = process_id }; 
    graph->processes_len += 1;
    return 0;
}

// Adds a resource to our current graph
// This function will return 0 on success or a negative number on an error 
int graph_add_resource(resource_alloc_graph_t *graph, int resource_id, int max_count) {
    if (graph->resources_len >= MAX_RESOURCES) {
        return -1; 
    }
    graph->resources[graph->resources_len] = (resource_t){ .id = resource_id, .max_count = max_count }; 
    graph->resources_len += 1;
    return 0;
}

// Tries to allocate `count` of the given resource at to the given process
// This function will return 0 on success or a negative number on an error 
int graph_alloc(resource_alloc_graph_t *graph, int process_id, int resource_id, int count) 
{
	resource_t *resource = &graph->resources[resource_id];
	process_t *process = &graph->processes[process_id];
	
	if(resource->current_count + count <= resource->max_count)
	{
		resource->current_count += count;
		resource->current_allocs[process_id] += count;
		process->request_list[resource_id] += count;
	}
	else
	{
		return -1;
	}
    return 0;
}

// Checks the graph for a deadlock, will return 1 if it finds one, or a 0 if it does not
// In order for a deadlock to be guarenteed, it there must be a cycle where each resource of that cycle is a mutex
	// - This is technically more than specifications, which just ask for detecting cycles, so I might change this in the future
bool graph_check_deadlock(resource_alloc_graph_t *graph) 
{
	int visited[MAX_PROCESSES];
	int recursed[MAX_PROCESSES];
	
	// Go through every unvisited node and check for cycles
	for(int i = 0; i < graph->processes_len; i++) {
		if(!visited[i] && graph_detect_cycle(graph, visited, graph->processes_len, recursed, graph->processes_len, i)) {
			return true;
		}
	}
	return false;
}

bool graph_detect_cycle(resource_alloc_graph_t *graph, int *visited, int visited_len, int *recursed, int recursed_len, int process_id) {
	if(visited[process_id]) { }
	if(recursed[process_id]) { }

	visited[process_id] = true;
	recursed[process_id] = true;
	
	// Check each resource
	for(int i = 0; i < graph->resources_len;i++) {
		// If the current resource has *any* allocations by the given process (e.g., utilizing the resource), check for another cycle
		if(graph->resources[i].current_allocs[process_id] > 0) //if the process is utilizing the resource
		{
			if(graph_detect_cycle(graph, visited, visited_len, recursed, recursed_len, process_id)) {
				return true;
			}
		}
	}	
	recursed[process_id] = false;
	return false;
}
