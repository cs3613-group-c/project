#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "../include/structures.h"
#include "../include/parse.h"
int main(){
	Parse ret = fileparse("input/intersections.txt", "input/trains.txt");
	
	if(ret.error > 0) printf("parse completed with errors\n\n");
	else printf("parse completed without errors\n\n");
	
	for(int i = 0; i < ret.sctn_count; i++){ //debug print for intersection assignment
		if(ret.sctn[i] == 0) continue;
		printf("Intersection = %c, size = %d\n", i + 'A',ret.sctn[i]);
	}
	
	for(int i = 0; i < ret.route_count; i++){
		printf("\nTrain %d:", i + 1);
		// printf("j: %d", j);
	
		for(int j = 0; j < 26; j++){
			//printf("i: %d, j: %d\n", i, j);
			if(ret.route[i][j] != 0)
				printf("Sctn: %c, ", ret.route[i][j]);
			
		} 
		printf("\n");
		
	}
	
	printf("route count: %d\nsctn count: %d\n\n", ret.route_count, ret.sctn_count);
	
 	return 0;
 }