#include "rag.h"
#include "test_runner.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define PROCESS_COUNT 5

#define RESOURCE_COUNT 5
#define MAX_RESOURCE_ALLOCS 3

bool test_should_have_deadlock() {
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

    // Test #1
    printf("Test #1: Should have deadlock\n");

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
    //
    //
    return has_deadlock != -1;
}

bool test_random() {
    srand(time(NULL)); // Seed with current time
    return rand() % 2 != 0;
}

int main() {
    test_data_t tests[] = {{"Ensure randomness", test_random}};

    test_all(tests, sizeof(tests) / sizeof(test_data_t));
}

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

    // Test #1
    printf("Test #1: Should have deadlock\n");

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
}
