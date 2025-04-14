#include "../include/rag.h"
#include <stdio.h>
#include <stdbool.h>

// #include "../src/rag.c"


#define PROCESS_COUNT 5

#define RESOURCE_COUNT 5
#define MAX_RESOURCE_ALLOCS 3

int main() {
  // Create an example graph
  resource_alloc_graph_t graph;
  graph_init(&graph);

  // Add processes to graph
  for (int i = 0; i < PROCESS_COUNT; i++) {
    if (graph_add_process(&graph, 0) != 0) {
      printf("Failed to add process to graph\n");
      return 1;
    }
  }

  // Add resources
  for (int i = 0; i < RESOURCE_COUNT; i++) {
    // Add our resources with their max resource count
    if (graph_add_resource(&graph, 0, MAX_RESOURCE_ALLOCS) != 0) {
      printf("Failed to add resource to graph\n");
    }
  }
  
  /* TODO: Setup resource state
	int graph_alloc(resource_alloc_graph_t *graph, int process_id, int resource_id, int count)
	for(int i = 0; i < RESOURCE_COUNT; i++)
	{
		if(graph_alloc(&graph, i, 1, 1) != 0)
		printf("failed to alloc resources to graph");
	}*/
	
	if(graph_alloc(&graph, 0, 1, 1) != 0)
		printf("failed to alloc resources to graph");
	
	if(graph_alloc(&graph, 1, 1, 1) != 0)
		printf("failed to alloc resources to graph");
	
	if(graph_alloc(&graph, 1, 1, 1) != 0)
		printf("failed to alloc resources to graph");
	
  // Check for deadlock
  bool has_deadlock = graph_check_deadlock(&graph);

  // Print results as needed
  if (has_deadlock) {
    printf("[!] Deadlock test failed \n");
    return 1;
  }

  printf("[*] Deadlock test succeeded\n");
  return 0;
}
