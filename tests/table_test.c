//  Group C
//  Author: Gabe Cornelius
//  Email: gabriel.j.cornelius@okstate.edu
//  Date: 4/6/2025

#include "parse.h"
#include "structures.h"
#include "table.h"
#include <stdbool.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/mman.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

shared_memory_t *m;

int main(){	
	m = mmap(NULL, sizeof(shared_memory_t), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);

		// Modify things
 	m->num_trains = 2;
		
	parse_t input = parse_file("input/intersections.txt", "input/trains.txt", m);
	if(input.error > 0) printf("parse completed with errors\n\n");
	else printf("parse completed without errors\n\n");
	
	table_init(m, input);
	// printf("num_trains: %d\n", mem->num_trains);
	/* 
 	for(int i = 0; i < mem->num_intersections; i++){
		printf("Intersection %c sctn_is_locked: %d\n",i + 'A',table_is_sctn_locked(i));
	}
	*/

	/* 
	for(int i = 0, j = 0; j < 20; i = (i+1)%5, j++){
		
		table_sctn_add_train(i, 0);
		table_sctn_add_train(i, 1);
		printf("Intersection %c num_holding_trains: %d\n",i + 'A',(*m).intersections[i].num_holding_trains);
		table_sctn_rem_train(i, 0);
		printf("Intersection %c num_holding_trains: %d\n",i + 'A',(*m).intersections[i].num_holding_trains);
	
	}
	*/

	table_sctn_add_train(0, 0);
	table_sctn_add_train(0, 1);
	table_sctn_add_train(0, 1);
	table_sctn_add_train(0, 2);
	printf("Intersection %c num_holding_trains: %d\n",0 + 'A',(*m).intersections[0].num_holding_trains);
	
	printf("Intersection %c num_holding_trains: %d\n",0 + 'A',(*m).intersections[0].num_holding_trains);
		
	table_print();
	
	munmap(m, sizeof(shared_memory_t));	
	
	
	return 0;
}
