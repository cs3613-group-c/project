// Authors: Cade Blakeman & Em Jordan
#include "rag.h"
#include <stdbool.h>

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
    graph->processes[graph->processes_len] = (process_t){.id = process_id};
    graph->processes_len += 1;
    return 0;
}

// Adds a resource to our current graph
// This function will return 0 on success or a negative number on an error
int graph_add_resource(resource_alloc_graph_t *graph, int resource_id,
                       int max_count) {
    if (graph->resources_len >= MAX_RESOURCES) {
        return -1;
    }
    graph->resources[graph->resources_len] =
        (resource_t){.id = resource_id, .max_count = max_count};
    graph->resources_len += 1;
    return 0;
}

// Tries to allocate `count` of the given resource at to the given process
// This function will return 0 on success or a negative number on an error
int graph_alloc(resource_alloc_graph_t *graph, int process_id, int resource_id,
                int count) {
    resource_t *resource = &graph->resources[resource_id];
    process_t *process = &graph->processes[process_id];

    // If the request + our current count exceeds our max count, return err
    if (resource->current_count + count > resource->max_count) {
        return -1;
    }

    resource->current_count += count;
    resource->current_allocs[process_id] += count;
    // TODO: Should the request list hold what the current process *holds*?
    process->request_list[resource_id] += count;
    return 0;
}

//Creates a request edge on the graph from a process asking for x amount of a resource (pointer from process to resource)
int graph_request_edge(resource_alloc_graph_t *graph, int process_id, int resource_id, int count){
	resource_t *resource = &graph->resources[resource_id];
    process_t *process = &graph->processes[process_id];
	
    process->request_list[resource_id] += count;
	return 0;
}

//Creates an assignment edge on the graph from a resource to a processes (pointer from resource to process)
int graph_assign_edge(resource_alloc_graph_t *graph, int process_id, int resource_id, int count){
	resource_t *resource = &graph->resources[resource_id];
    process_t *process = &graph->processes[process_id];

    if (resource->current_count + count <= resource->max_count) {
        resource->current_count += count;
        resource->current_allocs[process_id] += count;
    } else {
        return -1;
    }
    return 0;
}

//Removes x count from an assignment edge
int graph_remove_assignment(resource_alloc_graph_t *graph, int process_id, int resource_id, int count){
	resource_t *resource = &graph->resources[resource_id];
    process_t *process = &graph->processes[process_id];

    if (resource->current_allocs[process_id] >= count) {
        resource->current_count -= count;
        resource->current_allocs[process_id] -= count;
    } else {
        return -1;
    }
    return 0;
}

//Removes x count from an request edge
int graph_remove_request(resource_alloc_graph_t *graph, int process_id, int resource_id, int count){
	resource_t *resource = &graph->resources[resource_id];
    process_t *process = &graph->processes[process_id];

    if (process->request_list[resource_id] >= count) {
        process->request_list[resource_id] -= count;
    } else {
        return -1;
    }
    return 0;
}

int graph_check_deadlock(resource_alloc_graph_t *graph) {
    int visited[MAX_PROCESSES] = { 0 };
    int recursed[MAX_PROCESSES] = { 0 };

    // Go through every unvisited node and check for cycles
    for (int i = 0; i < graph->processes_len; i++) {	
        if (!visited[i]){
			//printf("Checking deadlocks beginning at process %d. \n", i);

			if(graph_detect_cycle(graph, visited, graph->processes_len, recursed, graph->processes_len, i))
				return i;
        }
    }
    return -1;
}

int graph_detect_cycle(resource_alloc_graph_t *graph,
                        int visited[MAX_PROCESSES], int visited_len,
                        int recursed[MAX_PROCESSES], int recursed_len,
                        int process_id) {
    if (recursed[process_id]) {
        return true;
    }
    if (visited[process_id]) {
        return false;
    }

    visited[process_id] = 1;
    recursed[process_id] = 1; 
	
    // Check each resource to see if it is being requested by the process
    for (int i = 0; i < graph->resources_len; i++) {
        if (graph->processes[process_id].request_list[i] > 0) {
            for (int j = 0; i < graph->processes_len; j++) {
                if (graph->resources[i].current_allocs[j] > 0){
                    if (graph_detect_cycle(graph, visited, visited_len, recursed, recursed_len, j)) {
                        return true;
                    }
				}
			}
		}
    }
    recursed[process_id] = false;
    return false;
}

//prints out the current resource and process states
void print_graph(resource_alloc_graph_t *graph){
	//print processes
	printf("Process Requests:\n");
	for(int i = 0; i < graph->processes_len; i++){
		printf("ID: %d [ ", i);
		for(int j = 0; j < graph->processes_len; j++){
			process_t *process = &graph->processes[i];
			printf(" %d, ", process->request_list[j]);
		}
		printf("]\n");
	}
	
	//print resources
	printf("Resource Allocations:\n");
	for(int i = 0; i < graph->resources_len; i++){
		printf("ID: %d [ ", i);
		for(int j = 0; j < graph->resources_len; j++){
			resource_t *resource = &graph->resources[i];
			printf(" %d, ", resource->current_allocs[j]);
		}
		printf("]\n");
	}
}
