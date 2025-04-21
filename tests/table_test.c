//  Group C
//  Author: Gabe Cornelius
//  Email: gabriel.j.cornelius@okstate.edu
//  Date: 4/20/2025
//  Description: 
//

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
	
		
	if(parse_file("input/intersections.txt", "input/trains.txt", m->intersections, m->trains , &m->num_intersections, &m->num_trains) > 0) printf("parse completed with errors\n\n");
	else printf("parse completed without errors\n\n");
	
	printf("test 1: Add train 1 to intersection A\n");
	table_add_train(&m->intersections[0], &m->trains[0]);
	table_print(m->intersections, m->trains, m->num_intersections);
	
	printf("\n\ntest 2: Add train 1 to intersection A again\n");
	table_add_train(&m->intersections[0], &m->trains[0]);
	table_print(m->intersections, m->trains, m->num_intersections);
	
	
	printf("\n\ntest 3: Remove train 1 to intersection A\n");
	table_rem_train(&m->intersections[0], &m->trains[0]);
	table_print(m->intersections, m->trains, m->num_intersections);
	
	
	table_add_train(&m->intersections[0], &m->trains[0]);
	
	printf("\n");
	intersection_print_status(&m->intersections[0]);
	
	train_print_status(&m->trains[0]);
	
	munmap(m, sizeof(shared_memory_t));	
	
	
	return 0;
}
