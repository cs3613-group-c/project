//  Group C
//  Author: Gabe Cornelius
//  Email: gabriel.j.cornelius@okstate.edu
//  Date: 4/6/2025

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "../include/structures.h"
#include "../include/table.h"


// shared_memory_t *mem_store;
Parse parse;


int table_init(shared_memory_t *mem, Parse input) { 	
	parse = input;
	mem->num_intersections = input.sctn_count;
	mem->num_trains = input.route_count;
	//continue assigning 
	return 0;
}

void table_print() {
	char lock[] = "locked";
	printf("Intersection: ID | Capacity | Lock State | Holding Trains\n");
	for(int i = 0; i < parse.sctn_count; i++){
		// printf("\n", parse.route_count);
		printf("Intersection: %c |    %d     | %s | Holding Trains\n", (i +'A'), parse.sctn[i], lock);
	}
}
