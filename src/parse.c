#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "../include/structures.h"

 Parse fileparse(const char* intersections_file, const char* trains_file){	
	int route[10][26] = {0};
	// int sctn[26] = {0};
	Parse ret = {{0},{0}, 0};
	
	//File Format: IntersectionName:Capacity
	char intName[256] = "Intersections.txt";
	FILE *intersections = fopen(intersections_file, "r");
	if (intersections == NULL) {
        perror(strcat(intName, " not found"));
		ret.error ^= 1;
        return ret;
    }
	
	char line[256];
	int l = 0; //line number for indexing into our sctn array
	
	while (fgets(line, sizeof(line), intersections)) {
		char match[] = "Intersection";
		char name = ' ';
		int size = 0;
		
		for (int i = 0; i < sizeof(line); i++){
			if(line[i] == match[i] && i < sizeof(match) - 1){//Currently Matching Intersection
				continue;
			}
				
			if ((line[i] != match[i]) && i < sizeof(match) - 1){//fails to match 'Intersection'
				printf("failed to match Intersection");
				ret.error ^= 2;
				return ret;
			} 
			
			if((line[i] != match[i]) && i == sizeof(match) - 1){ // Finished matching 'Intersection', starting assignments
				int j = line[i] - 'A';
				if(!(0 <= j && j < 26)){ //check if the index is valid
					ret.error ^= 2; 
					return ret;
				}
				if (line[i+1] != ':'){ //Fails due to not found ':' at exptected location (two char Int name or missing ':')
					ret.error ^= 2;
					return ret;
				}
				if(0 <= j && j < 26){ //Successful assignment
					ret.sctn[j] = line[i+2] - '0';
					break;
				}
			}
		}
	} // end while
	
	for(int i = 0; i < 26; i++){ //debug print for intersection assignment
		if(ret.sctn[i] == 0) continue;
		printf("Intersection = %c, size = %d\n", i + 'A',ret.sctn[i]);
	}
	
	//Purpose: Define train names and their routes (ordered list of intersections).
	//Format: TrainName:Intersection1,Intersection2,...
	FILE *trains = fopen(trains_file, "r");
	if (trains == NULL) {
        perror("Error");
		ret.error ^=4;
        return ret;
    }
	printf("\n");
	// fgets(line, sizeof(line), trains);
	char *tok;
	
	
	
	{ //scope here to isolate match1[]
		char match1[] = "Train";
		char match2[] = ":Intersection";
		char match3[] = ",Intersection";
		// printf("sizeof(match[2]): %d\n", sizeof(match2));
		while (fgets(line, sizeof(line), trains)) {
			//printf(line);
			bool m1 = false;
			bool m2 = false;
			int train = -1;
			int offset = 0;
			int stop = 0;
			for(int i = 0, j = 0; i < sizeof(line); i++){
				// printf("%c",line[i]);
				if(line[i] == '\n' || line[i] == 0){ // break once you reach linebreak or end of file
					break;
					
				}
				else if(!m1){ //state 0
					
					if(match1[i] != line[i] && i < sizeof(match1) - 1) { //check if fails to match all of match1
						// printf("break @ i %d\nline[i]: %c\n", i , line[i]);
						ret.error ^= 8; //set error bit 3 for formatting issue with trains.txt
						break;
					}		
					// if(match1[i] == line[i]) continue;
					
					if(match1[i] != line[i] && !m1){
						m1 = true;
						j = 0;
						if( 0 <= line[i] <= 9){
							train = line[i] - '0';
							// printf("i %d\nline[i]: %c\n", i , line[i]);
						}
						else //failed to provide valid index for trains
							ret.error ^= 8; //set error bit 3 for formatting issue with trains.txt
					}
				} //end state 0
				
				else if(!m2){ //state 1
					
					if(match2[j] != line[i] && j < sizeof(match2) - 1){
						printf("break @ i %d\nline[i]: %c\n", i , line[i]);
						ret.error ^= 8; //set error bit 3 for formatting issue with trains.txt
						break;
					}
					
					// if(match2[j] == line[i]) continue;
					
					if(match2[j] !=  line[i]){
						m2 = true;
						j = 0;
						// printf("\nTrain: %d\nFirst junct: %d\n",train, line[i] - 'A');
						ret.route[train][stop] = line[i];
						stop++;
						continue; //skip iterating j
					}
					j++; //iterate j while still scanning match
					
				} //end state 1
				
				else{ //state 2, looping reading ",Intersection" / match3
					//i = 20 on first occurence
					
					// printf("I: %d\nJ: %d\n", i, j);
					// printf("Line[I] : %c\n", line[i]);
					// printf("Match[J] : %c\n", match3[j]);
					if(match3[j] != line[i] && j < sizeof(match2) - 1){
						printf("break @ i %d\nline[i]: %c\n", i , line[i]);
						ret.error ^= 8; //set error bit 3 for formatting issue with trains.txt
						break;
					}
					
					if(match3[j] !=  line[i]){
						printf("\nTrain: %djunct: %d\n",train, line[i] - 'A');
						ret.route[train][stop] = line[i];
						stop++;
						j = 0;
						continue;
					}
					
					
					j++;
				}
					
			} //end for each character in line 
			
			
		} //end line reader while
	} //end trains.txt scope
	
	//ret.sctn = sctn;
	//*ret.route = *route;
	
	for(int i = 0; i < 10; i++){
		if(ret.route[i][0] == 0) continue;
		printf("\nTrain %d:\n", i);
		// printf("j: %d", j);
	
		for(int j = 0; j < 26; j++){
			//printf("i: %d, j: %d\n", i, j);
			if(ret.route[i][j] != 0)
				printf("Sctn: %c, \n", ret.route[i][j]);
			
		} 
		
	}
	
	return ret;
}

/* 
Error Codes:
	bit 0: (error & 1) {complete}
		Catastrophic, failed to open intersections 
	bit 1: (error & 2) {complete}
		Catastrophic, formatting issue with intersections
	bit 2: (error & 4) {complete}
		Catastrophic, failed to open trains.txt
	bit 3: (error & 8) {complete}
		Catastrophic, formatting issue with trains.txt
	bit 4: (error & 16) 
		*Catastrophic, illegal assignment with trains
			*may be handled with ignoring illegal junction assignment
	bit 5: 
		Warning only, double assignment with intersections
	bit 6: 
		Warning only, double assignment with trains
	bit ?: 
		check if intersections have non-zero sizes, recoverable dependent on if trains.txt references a stop at a zero intersection
	bit ?:
		check for duplicate route stops
 */

int main(){
// 	printf("Entering fileparser\n");
// 	struct ret parse;
	Parse ret = fileparse("../input/intersections.txt", "../input/trains.txt");
// 	if (parse.error > 0)
// 		printf("Error with config files\n");
// 	else
// 		printf("Normal exit of fileparser\n");
	
	// printf("parse.sctn = %d\n", parse.sctn[0]);
// 	printf("parse.route = %c\n", parse.route[1][0]);
 	return 0;
 }
