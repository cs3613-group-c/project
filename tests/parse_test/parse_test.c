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
 	return 0;
 }