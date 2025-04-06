#include <stdio.h>
#include <string.h>
#include <stdbool.h>
int main(){
	
	bool e0 = 1;
	bool e1 = 0;
	bool e2 = 0;
	
	int error = 0;
	
	if(e0) error ^= e0;
	if(e1) error ^= (e1<<1);
	
	printf("error code: %d\n", error);
	if(error & 1) printf("e0\n");
	if(error & 2) printf("e1\n");
	return 0;
}