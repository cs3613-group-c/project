/*
*/
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
int fileparser(){	
	//File Format: IntersectionName:Capacity
	FILE *intersections = fopen("intersections.txt", "r");
	if (intersections == NULL) {
        perror("Error");
        return 1;
    }
	
	char line[256];
	char match[] = "Intersection";
	
	while (fgets(line, sizeof(line), intersections)) {
			char sctn = ' ';
			int size = 0;
			
			for (int i = 0; i < sizeof(line); i++){
				
				if((line[i] != match[i])){
					sctn = line[i];
					size = line[i+2] - '0';
					break;
				}
				
			}
			
			printf("Original line: %s\n", line);
			printf("Intersection = %c, size = %d\n", sctn, size);
			
        }
	
	
	//Purpose: Define train names and their routes (ordered list of intersections).
	//Format: TrainName:Intersection1,Intersection2,...
	FILE *trains = fopen("trains.txt", "r");
	if (trains == NULL) {
        perror("Error");
        return 1;
    }
	
	
	return 0;
}

int main(){
	fileparser();
}