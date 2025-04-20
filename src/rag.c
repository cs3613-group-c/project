/*
 * Group C
 *
 * Authors: Cade Blakeman, Em Jordan
 * Date:
 * Description:
 *  - RAG - Resource Allocation Graph
 *
 *  Creates a resource allocation graph that can be assigned edges depending on
 *  resource allocation or requests Uses the graph to detect cycles within the
 *  trains
 */
#include "rag.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "logger.h"



//EXTERNAL FUNCTIONS

//detects and corrects deadlocks
//returns the problem process and resource in an int array if a deadlock occured
//returns [-1,-1] if no deadlock occured
int *deadlock_detection(resource_alloc_graph_t *graph, int *output_array){
	
	int cycle_list[] = {[0 ... MAX_PROCESSES] = -1};

	memcpy(cycle_list, graph_check_deadlock(graph, cycle_list), sizeof(cycle_list));
	
	if(cycle_list[0] >= 0)
	{
		char message[1024];
		sprintf(message, "Deadlock Detected! Cycle: Train%d", cycle_list[0]);	
		
		// Send deadlock detected messages -> might get an error out of bounds, ill probably fix this just by increasing the cycle list tbh
		int i = 2;
		while(cycle_list[i] >= 0)
		{
			char addString[64];
			sprintf(addString, "-> Train%d, ", cycle_list[i]);
			strcat(message, addString);
			i += 2;
		}
		log_event(message);
		memcpy(output_array, resolve_deadlock(graph, cycle_list, output_array), sizeof(output_array));
		return output_array;
	}
	output_array[0] = -1;
	output_array[1] = -1;
	return output_array;
}
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
int graph_add_resource(resource_alloc_graph_t *graph, int resource_id, int max_count) {
    if (graph->resources_len >= MAX_RESOURCES) {
        return -1;
    }
    graph->resources[graph->resources_len] =
        (resource_t){.id = resource_id, .max_count = max_count};
    graph->resources_len += 1;
    return 0;
}

// Creates a request edge on the graph from a process asking for x amount of a
// resource (pointer from process to resource)
int graph_request_edge(resource_alloc_graph_t *graph, int process_id, int resource_id, int count) {
    resource_t *resource = &graph->resources[resource_id];
    process_t *process = &graph->processes[process_id];

    process->request_list[resource_id] += count;
    return 0;
}

// Creates an assignment edge on the graph from a resource to a processes
// (pointer from resource to process)
int graph_assign_edge(resource_alloc_graph_t *graph, int process_id, int resource_id, int count) {
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

// Removes x count from an assignment edge
int graph_remove_assignment(
    resource_alloc_graph_t *graph, int process_id, int resource_id, int count) {
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

// Removes x count from an request edge
int graph_remove_request(
    resource_alloc_graph_t *graph, int process_id, int resource_id, int count) {
    resource_t *resource = &graph->resources[resource_id];
    process_t *process = &graph->processes[process_id];

    if (process->request_list[resource_id] >= count) {
        process->request_list[resource_id] -= count;
    } else {
        return -1;
    }
    return 0;
}






//INTERNAL FUNCTIONS


// Function that checks for cycles in a graph. If there is a cycle, it means the
// possibility of a deadlocks If there is a cycle, it will spit out the first
// process in the cycle that it found
int *graph_check_deadlock(resource_alloc_graph_t *graph, int *cycle_list) {
    bool visited[MAX_PROCESSES] = {};
    bool recursed[MAX_PROCESSES] = {};
	
    // Go through every unvisited node and check for cycles
    for (int i = 0; i < graph->processes_len; i++) {
        if (!visited[i]) {
            // printf("Checking deadlocks beginning at process %d. \n", i);

            memcpy(cycle_list, graph_detect_cycle(graph, visited, graph->processes_len, recursed, graph->processes_len, i, cycle_list, 0), sizeof(cycle_list));
            if (cycle_list[1] == true)
			{
				cycle_list[0] = i;
                return cycle_list;
			}
    }
    return cycle_list;
	}
}

// Recursive method that the graph_check_deadlock method calls. Will recursively
// call itself until it finds a processes it has already found. Its a bit
// simplified, but if its found, then you found a cycle, and will return true.
int *graph_detect_cycle(
    resource_alloc_graph_t *graph,
    bool visited[MAX_PROCESSES],
    int visited_len,
    bool recursed[MAX_PROCESSES],
    int recursed_len,
    int process_id,
	int *cycle_list, 
	int offset) {
		
    if (recursed[process_id]) {
        cycle_list[offset] = process_id;
        return cycle_list;
    }
    if (visited[process_id]) {
        return cycle_list;
    }

    visited[process_id] = true;
    recursed[process_id] = true;
	
    // Check each resource to see if it is being requested by the process
    for (int i = 0; i < graph->resources_len; i++) {
        if (graph->processes[process_id].request_list[i] > 0) {
            for (int j = 0; j < graph->processes_len; j++) {
                if (graph->resources[i].current_allocs[j] > 0) 
				{
					cycle_list[offset] = process_id;
					cycle_list[offset + 1] = i;
					
					memmove(cycle_list, graph_detect_cycle(graph, visited, visited_len, recursed, recursed_len, j, cycle_list, offset + 2), sizeof(cycle_list));
					
                    if (cycle_list[offset + 2] >= 0) 
					{
                        return cycle_list;
                    }
					
					cycle_list[offset] = -1;
					cycle_list[offset + 1] = -1;
                }
            }
        }
    }
    recursed[process_id] = false;
    return cycle_list;
}

// prints out the current resource and process states
void print_graph(resource_alloc_graph_t *graph) {
    // print processes
    printf("Process Requests:\n");
    for (int i = 0; i < graph->processes_len; i++) {
        printf("ID: %d [ ", i);
        for (int j = 0; j < graph->processes_len; j++) {
            process_t *process = &graph->processes[i];
            printf(" %d, ", process->request_list[j]);
        }
        printf("]\n");
    }

    // print resources
    printf("Resource Allocations:\n");
    for (int i = 0; i < graph->resources_len; i++) {
        printf("ID: %d [ ", i);
        for (int j = 0; j < graph->resources_len; j++) {
            resource_t *resource = &graph->resources[i];
            printf(" %d, ", resource->current_allocs[j]);
        }
        printf("]\n");
    }
}

//finds the problem, announces it then deletes it. Returns false on an error
int *resolve_deadlock(resource_alloc_graph_t *graph, int *cycle_list, int *output_array)
{
	
	int closest_process_id = 0;
	int closest_processes = 0;
	
	//Find process closest to completion
	for(int i = 0; i < MAX_PROCESSES; i += 2)
	{
		process_t *process = &graph->processes[i];
		int requested_resources = 0;
		
		for(int j = 0; j < graph->resources_len; j++)
		{
			if(process->request_list[j] > 0)
				requested_resources++;
		}
		
		if(requested_resources < closest_processes)
		{
			closest_process_id = i;
			closest_processes = requested_resources;
		}
	}
	int problem_process = closest_process_id + 2;
	
	//Its worth noting that there doesn't need to be any catches for getting a negative integer, because the first process will be repeated at the end of the list due to the cycle.
	//This means that its impossible for index 0 to be the problem process, it will always be the last index in the cycle instead
	int deadlock_fix[2] = {cycle_list[problem_process], cycle_list[problem_process - 1]};
	memcpy(output_array, deadlock_fix, sizeof(output_array));
	//printf("Problem Process: %d\n", deadlock_fix[0]);
	//printf("Problem Resource: %d\n", deadlock_fix[1]);
	return output_array;	
}
