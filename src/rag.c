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
	process_t *process = &graph->processes[proccess_id];
	
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
	int visited[graph->processes_len];
	int recursed[graph->processes_len];
	
	//go through every unvisited node and check for cycles
	for(int i = 0; i < graph->processes_len; i++)
	{
		if(!visited[i] && detectCycle(resource_alloc_graph_t *graph, *visited, *recursed, i))
			return true; //cycle found
	}
    return false;
}

bool detectCycle(resource_alloc_graph_t *graph, int[] visited, int[] recursed, int process)
{
	if(visited[process])
		
	if(recursed[process])
		
	visited[process] = true;
	recursed[process] = true;
	
	for(int i = 0; i < graph->resources_len;i++) //for each resource in the process
	{
		if(resources[i] > 0) //if the process is utilizing the resource
		{
			if(detectCycle(resource_alloc_graph_t *graph, int[] visited, int[] recursed, int i))
				return true;
		}
	}
	
	recursed[process] = false;
	return false;
}