/*
*/
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
struct ret{
	int route[10][26], sctn[26], error;
}; // this struct declaration is probably wrong
struct ret ret;
bool errCode = 0;

struct ret fileparser(){	
	struct ret;
	int route[10][26] = {0};
	bool error;
	//File Format: IntersectionName:Capacity
	FILE *intersections = fopen("intersections.txt", "r");
	if (intersections == NULL) {
        perror("Error");
		ret.error = 1;
        return ret;
    }
	
	char line[256];
	char match[] = "Intersection";
	int sctn[26] = {0};
	int l = 0; //line number for indexing into our sctn array
	
	while (fgets(line, sizeof(line), intersections)) {
		char name = ' ';
		int size = 0;
		
		for (int i = 0; i < sizeof(line); i++){
			if(line[i] == match[i] && i < sizeof(match) - 1){//Currently Matching Intersection
				continue;
			}
				
			if ((line[i] != match[i]) && i < sizeof(match) - 1){//fails to match 'Intersection'
				printf("failed to match Intersection");
				ret.error = 2;
				return ret;
			} 
			
			if((line[i] != match[i]) && i == sizeof(match) - 1){ // Finished matching 'Intersection', then takes infromations
				int j = line[i] - 'A';
				if(!(0 <= j && j < 26)){ //check if the index is valid
					ret.error = 2; 
					return ret;
				}
				if (line[i+1] != ':'){
					ret.error = 2;
					return ret;
				}
				if(0 <= j && j < 26){ 
					// printf("\n\nj: %d\n", j);
					sctn[j] = line[i+2] - '0';
					break;
				}
			}
		}
	} // end while
	
	for(int i = 0; i < 26; i++){ //debug print for intersection assignment
		if(sctn[i] == 0) continue;
		printf("Intersection = %c, size = %d\n", i + 'A',sctn[i]);
	}
	
	//Purpose: Define train names and their routes (ordered list of intersections).
	//Format: TrainName:Intersection1,Intersection2,...
	FILE *trains = fopen("trains.txt", "r");
	if (trains == NULL) {
        perror("Error");
		ret.error=1;
        return ret;
    }
	printf("\n");
	// fgets(line, sizeof(line), trains);
	char *tok;
	
	for (int i = 0; fgets(line, sizeof(line), trains); i++) {
		//printf(strtok(line, "Train:Intersection,"));
		tok = strtok(line, "Train:");
		printf(tok); //replace this with an appropriate assign //FIXME: check if this is numerical
		
		//initial iterator to the loop so 'Train0' doesn't cause early termination when checking tok!=0
		tok = strtok(0, "ntersection,");
		tok = strtok(0, "ntersection,"); //call it twice to skip over the I
		printf(tok); //replace this with appropriate assign
		route[i][0] = tok;
		
		for(int j = 1; (tok != 0 && j < 26); j++){
			tok = strtok(0, "ntersection,");
			tok = strtok(0, "ntersection,"); //call it twice to skip over the I
			printf(tok); //replace this with appropriate assign
			route[i][j] = tok;
			
		}
		
	}
	
	for(int i = 0, j; i < 10; i++, j = 0){
		if(route[i][0] =r= 0) continue;
		printf("\nTrain: %d", i);
		for(; j< 26; j++){
			
			printf("");
			
		}
		
	}
	
	return ret;
}

int main(){
	fileparser();
}