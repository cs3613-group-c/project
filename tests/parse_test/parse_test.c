#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "../../include/structures.h"
#include "../../include/parse.h"
int main(){
	Parse ret = fileparse("input/intersections.txt", "input/trains.txt");
	
	for(int i = 0; i < 26; i++){ //debug print for intersection assignment
		if(ret.sctn[i] == 0) continue;
		printf("Intersection = %c, size = %d\n", i + 'A',ret.sctn[i]);
	}
	
	for(int i = 0; i < 10; i++){
		if(ret.route[i][0] == 0) continue;
		printf("\nTrain %d:", i);
		// printf("j: %d", j);
	
		for(int j = 0; j < 26; j++){
			//printf("i: %d, j: %d\n", i, j);
			if(ret.route[i][j] != 0)
				printf("Sctn: %c, ", ret.route[i][j]);
			
		} 
		printf("\n");
		
	}
	
 	return 0;
 }