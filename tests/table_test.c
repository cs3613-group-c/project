//  Group C
//  Author: Gabe Cornelius
//  Email: gabriel.j.cornelius@okstate.edu
//  Date: 4/6/2025

#include <stdio.h>
#include <stdbool.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <unistd.h>
#include "../include/structures.h"
#include "../include/parse.h"
#include "../include/table.h"
#include <sys/mman.h>
#include <sys/types.h>

shared_memory_t *shared_memory;

int main(){	
	shared_memory = mmap(NULL, sizeof(shared_memory_t), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);

	// Modify things
 	shared_memory->num_trains = 2;
	
 	// Unmap
	shared_memory->num_trains = 2;
	
	Parse input = fileparse("input/intersections.txt", "input/trains.txt");
	if(input.error > 0) printf("parse completed with errors\n\n");
	else printf("parse completed without errors\n\n");
	
	table_init(shared_memory, input);
	// printf("num_trains: %d\n", shared_memory->num_trains);
	table_print();
 	
	munmap(shared_memory, sizeof(shared_memory_t));	
	
	return 0;
}
