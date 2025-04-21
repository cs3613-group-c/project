#include "rag.h"
#include "logger.h"
#include "test_runner.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h> 
 
#define PROCESS_COUNT 5

#define RESOURCE_COUNT 5
#define MAX_RESOURCE_ALLOCS 3

//Tests to Run
	//Obvious Has deadlock
	//Obvious Does not have deadlock
	//Project Deadlock Example
	
	//Deadlock Resolution Project Example
	//Deadlock Resolution Complicated
	

int main(){
	
	printf("Keep in mind, indexing starts at 0 so train 1 will be train 0, and intersection 3 looks like intersection 2\n");
	
	
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
            return 1;
        }
    }
	
	printf("Test 1: Obvious Deadlock\n");
	
	//make graph
	
	if (graph_assign_edge(&graph, 0, 1, 1) != 0)
        printf("failed to alloc resources to graph");

    if (graph_assign_edge(&graph, 1, 0, 1) != 0)
        printf("failed to alloc resources to graph");

    if (graph_request_edge(&graph, 0, 0, 1) != 0)
        printf("failed to alloc resources to graph");

    if (graph_request_edge(&graph, 1, 1, 1) != 0)
        printf("failed to alloc resources to graph");
	
	
	//print graph
	
	print_graph(&graph);
	
	//print cycle
	int cycle_list[] = {[0 ... 10] = -1};
	memcpy(cycle_list, graph_check_deadlock(&graph, cycle_list), sizeof(cycle_list));
	
	printf("Cycle = [ ");
	for(int i = 0; i < 10; i++)
	{
		printf("%d, ", cycle_list[i]);
	}
	printf(" ]\n");
	
	//print resolution
	int resolution[2];
	memcpy(resolution, resolve_deadlock(&graph, cycle_list, resolution), sizeof(resolution));
	
	printf("Take resource %d from train %d\n\n", resolution[1], resolution[0]);
	
	
	
	//TEST 2
	printf("Test 2: Obvious not deadlock\n");
	
	
	resource_alloc_graph_t graph2;
    graph_init(&graph2);

    // Add processes to graph
    for (int i = 0; i < PROCESS_COUNT; i++) {
        if (graph_add_process(&graph2, 0) != 0) {
            printf("Failed to add process to graph\n");
            return 1;
        }
    }

    // Add resources
    for (int i = 0; i < RESOURCE_COUNT; i++) {
        // Add our resources with their max resource count
        if (graph_add_resource(&graph2, 0, MAX_RESOURCE_ALLOCS) != 0) {
            printf("Failed to add resource to graph\n");
            return 1;
        }
    }
	
	
	//make graph
	
	if (graph_assign_edge(&graph2, 0, 1, 1) != 0)
        printf("failed to alloc resources to graph");

    if (graph_assign_edge(&graph2, 1, 1, 1) != 0)
        printf("failed to alloc resources to graph");

    if (graph_request_edge(&graph2, 0, 0, 1) != 0)
        printf("failed to alloc resources to graph");

    if (graph_request_edge(&graph2, 1, 0, 1) != 0)
        printf("failed to alloc resources to graph");
	//print graph
	print_graph(&graph2);
	
	//print cycle
	
	int cycle_list2[] = {[0 ... 10] = -1};
	memcpy(cycle_list2, graph_check_deadlock(&graph2, cycle_list2), sizeof(cycle_list2));
	
	printf("Cycle = [ ");
	for(int i = 0; i < 10; i++)
	{
		printf("%d, ", cycle_list2[i]);
	}
	printf(" ]\n");
	
	//print resolution
	printf("The resolution should result in -1, -1 because there was no deadlock found\n");
	
	int resolution2[2];
	memcpy(resolution2, resolve_deadlock(&graph2, cycle_list2, resolution2), sizeof(resolution2));
	
	printf("Take resource %d from train %d\n\n", resolution2[1], resolution2[0]);
	
	
	printf("Test 3: Project Deadlock Example\n");

    // Create an example graph
    resource_alloc_graph_t graph3;
    graph_init(&graph3);

    // Add 4 processes to graph
    for (int i = 0; i < 4; i++) {
        if (graph_add_process(&graph3, i) != 0) {
            printf("Failed to add process to graph\n");
            return 1;
        }
    }

    // create 5 resources
    if (graph_add_resource(&graph3, 0, 1) != 0) {
        printf("Failed to add resource to graph\n");
        return 1;
    }
    if (graph_add_resource(&graph3, 1, 2) != 0) {
        printf("Failed to add resource to graph\n");
        return 1;
    }
    if (graph_add_resource(&graph3, 2, 1) != 0) {
        printf("Failed to add resource to graph\n");
        return 1;
    }
    if (graph_add_resource(&graph3, 3, 3) != 0) {
        printf("Failed to add resource to graph\n");
        return 1;
    }
    if (graph_add_resource(&graph3, 4, 1) != 0) {
        printf("Failed to add resource to graph\n");
        return 1;
    }

    // create graph (same graph as shown in the project document under deadlock
    // detection)

    if (graph_assign_edge(&graph3, 0, 0, 1) != 0)
        printf("failed to alloc resources to graph");
    if (graph_assign_edge(&graph3, 0, 1, 1) != 0)
        printf("failed to alloc resources to graph");

    if (graph_assign_edge(&graph3, 1, 1, 1) != 0)
        printf("failed to alloc resources to graph");
    if (graph_assign_edge(&graph3, 1, 3, 1) != 0)
        printf("failed to alloc resources to graph");

    if (graph_assign_edge(&graph3, 2, 2, 1) != 0)
        printf("failed to alloc resources to graph");
    if (graph_assign_edge(&graph3, 2, 3, 1) != 0)
        printf("failed to alloc resources to graph");

    if (graph_assign_edge(&graph3, 3, 4, 1) != 0)
        printf("failed to alloc resources to graph");

    if (graph_request_edge(&graph3, 0, 2, 1) != 0)
        printf("failed to alloc resources to graph");

    if (graph_request_edge(&graph3, 1, 4, 1) != 0)
        printf("failed to alloc resources to graph");

    if (graph_request_edge(&graph3, 2, 0, 1) != 0)
        printf("failed to alloc resources to graph");

    if (graph_request_edge(&graph3, 3, 1, 1) != 0)
        printf("failed to alloc resources to graph");

    //print graph
	print_graph(&graph3);


    //print cycle
	int cycle_list3[] = {[0 ... 10] = -1};
	memcpy(cycle_list3, graph_check_deadlock(&graph3, cycle_list3), sizeof(cycle_list3));
	
	printf("Cycle = [ ");
	for(int i = 0; i < 10; i++)
	{
		printf("%d, ", cycle_list3[i]);
	}
	printf(" ]\n");
	
	//print resolution
	int resolution3[2];
	memcpy(resolution3, resolve_deadlock(&graph3, cycle_list3, resolution3), sizeof(resolution3));
	
	printf("Take resource %d from train %d\n\n", resolution3[1], resolution3[0]);
	
	
	printf("Now to test both together using deadlock_detection, which the main server will use\n");
	printf("Will be in the form of [process,resource]\n");
	memcpy(resolution, deadlock_detection(&graph, resolution), sizeof(resolution));
	memcpy(resolution2, deadlock_detection(&graph2, resolution2), sizeof(resolution));
	memcpy(resolution3, deadlock_detection(&graph3, resolution3), sizeof(resolution));
	
	printf("Test1:[%d,%d]\n", resolution[0], resolution[1]);
	printf("Test2:[%d,%d]\n", resolution2[0], resolution2[1]);
	printf("Test3:[%d,%d]\n", resolution3[0], resolution3[1]);
}









/*bool test_should_have_deadlock() {
    // Create an example graph
    resource_alloc_graph_t graph;
    graph_init(&graph);

    // Add processes to graph
    for (int i = 0; i < PROCESS_COUNT; i++) {
        if (graph_add_process(&graph, 0) != 0) {
            printf("Failed to add process to graph\n");
            return false;
        }
    }

    // Add resources
    for (int i = 0; i < RESOURCE_COUNT; i++) {
        // Add our resources with their max resource count
        if (graph_add_resource(&graph, 0, MAX_RESOURCE_ALLOCS) != 0) {
            printf("Failed to add resource to graph\n");
            return false;
        }
    }

    // Create a deadlock between Process 1 & Process 2, containing a cycle
    // between R1 & R2
    if (graph_assign_edge(&graph, 0, 1, 1) != 0)
        printf("failed to alloc resources to graph");

    if (graph_assign_edge(&graph, 1, 0, 1) != 0)
        printf("failed to alloc resources to graph");

    if (graph_request_edge(&graph, 0, 0, 1) != 0)
        printf("failed to alloc resources to graph");

    if (graph_request_edge(&graph, 1, 1, 1) != 0)
        printf("failed to alloc resources to graph");

	//int has_deadlock[MAX_PROCESSES];
    //memcpy(has_deadlock, graph_check_deadlock(&graph), sizeof(has_deadlock));
	
	int test[2] = {-1, -1};
	memcpy(test, deadlock_detection(&graph, test), sizeof(test));
    if(test[0] >= 0)
		return 1;
    // prints the view of the graph, uncomment for bug fixing
    // print_graph(&graph);
    return 0;
}

bool test_random() {
    srand(time(NULL)); // Seed with current time
    return rand() % 2 != 0;
}

int main() {
    /*test_data_t tests[] = {
        {"Ensure randomness", test_random},
    };
	
    test_all(tests, sizeof(tests) / sizeof(test_data_t));*
	
	//printf("Hello\n");
	if(test_should_have_deadlock())
		printf("Passed\n");
	printf("Done\n");
	
	
	
	
	
	// Test #3
    printf("Test #3: Should have deadlock\n");

    // Create an example graph
    resource_alloc_graph_t graph3;
    graph_init(&graph3);

    // Add 4 processes to graph
    for (int i = 0; i < 4; i++) {
        if (graph_add_process(&graph3, i) != 0) {
            printf("Failed to add process to graph\n");
            return 1;
        }
    }

    // create 5 resources
    if (graph_add_resource(&graph3, 0, 1) != 0) {
        printf("Failed to add resource to graph\n");
        return 1;
    }
    if (graph_add_resource(&graph3, 1, 2) != 0) {
        printf("Failed to add resource to graph\n");
        return 1;
    }
    if (graph_add_resource(&graph3, 2, 1) != 0) {
        printf("Failed to add resource to graph\n");
        return 1;
    }
    if (graph_add_resource(&graph3, 3, 3) != 0) {
        printf("Failed to add resource to graph\n");
        return 1;
    }
    if (graph_add_resource(&graph3, 4, 1) != 0) {
        printf("Failed to add resource to graph\n");
        return 1;
    }

    // create graph (same graph as shown in the project document under deadlock
    // detection)

    if (graph_assign_edge(&graph3, 0, 0, 1) != 0)
        printf("failed to alloc resources to graph");
    if (graph_assign_edge(&graph3, 0, 1, 1) != 0)
        printf("failed to alloc resources to graph");

    if (graph_assign_edge(&graph3, 1, 1, 1) != 0)
        printf("failed to alloc resources to graph");
    if (graph_assign_edge(&graph3, 1, 3, 1) != 0)
        printf("failed to alloc resources to graph");

    if (graph_assign_edge(&graph3, 2, 2, 1) != 0)
        printf("failed to alloc resources to graph");
    if (graph_assign_edge(&graph3, 2, 3, 1) != 0)
        printf("failed to alloc resources to graph");

    if (graph_assign_edge(&graph3, 3, 4, 1) != 0)
        printf("failed to alloc resources to graph");

    if (graph_request_edge(&graph3, 0, 2, 1) != 0)
        printf("failed to alloc resources to graph");

    if (graph_request_edge(&graph3, 1, 4, 1) != 0)
        printf("failed to alloc resources to graph");

    if (graph_request_edge(&graph3, 2, 0, 1) != 0)
        printf("failed to alloc resources to graph");

    if (graph_request_edge(&graph3, 3, 1, 1) != 0)
        printf("failed to alloc resources to graph");

    // prints the view of the graph, uncomment for bug fixing
    // print_graph(&graph3);

    //int has_deadlock = graph_check_deadlock(&graph3);
    // Print results as needed
	int test[2] = {-1, -1};
	memcpy(test, deadlock_detection(&graph3, test), sizeof(test));
    if(test[0] >= 0)
		printf("Passed\n");
	printf("Done\n");
}

/*
int test_main() {
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
            return 1;
        }
    }

    // Create a deadlock between Process 1 & Process 2, containing a cycle
    // between R1 & R2
    if (graph_assign_edge(&graph, 0, 1, 1) != 0)
        printf("failed to alloc resources to graph");

    if (graph_assign_edge(&graph, 1, 0, 1) != 0)
        printf("failed to alloc resources to graph");

    if (graph_request_edge(&graph, 0, 0, 1) != 0)
        printf("failed to alloc resources to graph");

    if (graph_request_edge(&graph, 1, 1, 1) != 0)
        printf("failed to alloc resources to graph");

    int has_deadlock = graph_check_deadlock(&graph);

    // prints the view of the graph, uncomment for bug fixing
    // print_graph(&graph);

    if (has_deadlock != -1) {
        printf("[!] Deadlock test failed containing process %d \n",
               has_deadlock);
    } else
        printf("[*] Deadlock test succeeded\n");

    // Test #2
    printf("Test #2: Should not have deadlock\n");

    // removing everything from graph to make an empty one
    if (graph_remove_assignment(&graph, 0, 1, 1) != 0) {
        printf("Failed to remove assignment");
    }

    if (graph_remove_assignment(&graph, 1, 0, 1) != 0) {
        printf("Failed to remove assignment");
    }

    if (graph_remove_request(&graph, 0, 0, 1) != 0) {
        printf("Failed to remove request");
    }

    if (graph_remove_request(&graph, 1, 1, 1) != 0) {
        printf("Failed to remove request");
    }

    // prints the view of the graph, uncomment for bug fixing
    // print_graph(&graph);

    has_deadlock = graph_check_deadlock(&graph);

    // Print results as needed
    if (has_deadlock != -1) {
        printf("[!] Deadlock test failed containing process %d \n",
               has_deadlock);
    } else
        printf("[*] Deadlock test succeeded\n");

    // Test #3
    printf("Test #3: Should have deadlock\n");

    // Create an example graph
    resource_alloc_graph_t graph3;
    graph_init(&graph3);

    // Add 4 processes to graph
    for (int i = 0; i < 4; i++) {
        if (graph_add_process(&graph3, i) != 0) {
            printf("Failed to add process to graph\n");
            return 1;
        }
    }

    // create 5 resources
    if (graph_add_resource(&graph3, 0, 1) != 0) {
        printf("Failed to add resource to graph\n");
        return 1;
    }
    if (graph_add_resource(&graph3, 1, 2) != 0) {
        printf("Failed to add resource to graph\n");
        return 1;
    }
    if (graph_add_resource(&graph3, 2, 1) != 0) {
        printf("Failed to add resource to graph\n");
        return 1;
    }
    if (graph_add_resource(&graph3, 3, 3) != 0) {
        printf("Failed to add resource to graph\n");
        return 1;
    }
    if (graph_add_resource(&graph3, 4, 1) != 0) {
        printf("Failed to add resource to graph\n");
        return 1;
    }

    // create graph (same graph as shown in the project document under deadlock
    // detection)

    if (graph_assign_edge(&graph3, 0, 0, 1) != 0)
        printf("failed to alloc resources to graph");
    if (graph_assign_edge(&graph3, 0, 1, 1) != 0)
        printf("failed to alloc resources to graph");

    if (graph_assign_edge(&graph3, 1, 1, 1) != 0)
        printf("failed to alloc resources to graph");
    if (graph_assign_edge(&graph3, 1, 3, 1) != 0)
        printf("failed to alloc resources to graph");

    if (graph_assign_edge(&graph3, 2, 2, 1) != 0)
        printf("failed to alloc resources to graph");
    if (graph_assign_edge(&graph3, 2, 3, 1) != 0)
        printf("failed to alloc resources to graph");

    if (graph_assign_edge(&graph3, 3, 4, 1) != 0)
        printf("failed to alloc resources to graph");

    if (graph_request_edge(&graph3, 0, 2, 1) != 0)
        printf("failed to alloc resources to graph");

    if (graph_request_edge(&graph3, 1, 0, 1) != 0)
        printf("failed to alloc resources to graph");

    if (graph_request_edge(&graph3, 2, 4, 1) != 0)
        printf("failed to alloc resources to graph");

    if (graph_request_edge(&graph3, 3, 1, 1) != 0)
        printf("failed to alloc resources to graph");

    // prints the view of the graph, uncomment for bug fixing
    // print_graph(&graph3);

    has_deadlock = graph_check_deadlock(&graph3);
    // Print results as needed
    if (has_deadlock != -1) {
        printf("[!] Deadlock test failed containing process %d \n",
               has_deadlock);
    } else
        printf("[*] Deadlock test succeeded\n");

    return 0;
}*/
