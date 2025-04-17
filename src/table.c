//  Group C
//  Author: Gabe Cornelius
//  Email: gabriel.j.cornelius@okstate.edu
//  Date: 4/6/2025

#include "table.h"
#include "structures.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

shared_memory_t *mem;


int table_init(shared_memory_t *m, parse_t parse) { 	

	mem = m;
	(*mem).num_intersections = parse.sctn_count;
	(*mem).num_trains = parse.route_count;
	
	for(int i = 0; i < MAX_TRAINS; i++){
		
		(*mem).intersections[i].capacity = parse.sctn[i];
		(*mem).intersections[i].num_holding_trains = 0;
		
		for(int j = 0; j < MAX_TRAINS; j++){ //Train holding initializer
			
			(*mem).intersections[i].table_holding_trains[j] = false;
			
		}
		
	}
	
	return 0;
}

bool table_is_sctn_locked(int i){ //return 1 if locked and 0 otherwise
	return((*mem).intersections[i].num_holding_trains > (*mem).intersections[i].capacity);
}

void table_sctn_add_train(int sctn, int train){
	
	if(!(*mem).intersections[sctn].table_holding_trains[train]){ //FIXME: Fairly sure this makes a race condition and this check may be done somewhere else
	
		(*mem).intersections[sctn].num_holding_trains++;
		(*mem).intersections[sctn].table_holding_trains[train] = true;
		
	}
	
}

void table_sctn_rem_train(int sctn, int train){
	
	if((*mem).intersections[sctn].table_holding_trains[train]){ //FIXME: Fairly sure this makes a race condition and this check may be done somewhere else
	
		(*mem).intersections[sctn].num_holding_trains--;
		(*mem).intersections[sctn].table_holding_trains[train] = false;
	
	}
	
}

void table_print(){
		
	printf("table print\n");

	char lock[] = "locked";
	printf("Intersection: ID | Capacity | Lock State | Holding Trains\n");
	
	for(int i = 0, j = 0; i < (*mem).num_intersections; i++, j = 0){
		
		printf("Intersection: %c  |    %d     | %s |  ", (i +'A'), (*mem).intersections[i].capacity, table_is_sctn_locked(i) ? "  Locked  " : " Unlocked ");
		printf("[");
		if((*mem).intersections[i].table_holding_trains[j])
				printf(" Train %d", j + 1);
		for(int j = 1; j < MAX_TRAINS; j++){
			// printf("%d", (*mem).intersections[i].table_holding_trains[j]);
			if((*mem).intersections[i].table_holding_trains[j])
				printf(", Train %d", j + 1);
			
		}
		printf(" ]\n");
	}
	
}
