#include <stdio.h> // for printf
#include <time.h> // for clock()
#include <unistd.h> // for fork
#include <stdlib.h> // for exit(1)
#include <sys/ipc.h> // Used by IPC maechanisms: messages, shared memory and semaphores
#include <string.h> // for string operation: strlen
#include<sys/wait.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <semaphore.h>
#define QUEUE_NAME "/addition"
#define PERMISSIONS 0660
#define MAX_MESSAGES 10
#define MAX_MSG_SIZE 256
#define MSG_BUFFER_SIZE MAX_MSG_SIZE + 10
#define MAX 9000000000 // 9 billion times

/*Shared Memory Setup:
Create shared memory segments to store intersection states (e.g., pthread_mutex_t for
mutexes, sem_t for semaphores).

After parsing the input text files, the parent configures shared memory segments to manage intersection
resources. These segments store mutexes (implemented as pthread_mutex_t structures) for intersections
with a capacity of 1, ensuring exclusive access for single-track sections like narrow bridges. For multi-track
intersections (capacity > 1), the parent initializes counting semaphores (sem_t), setting their values to
match the specified capacities—for example, a semaphore initialized to 3 for an intersection that can
accommodate three trains simultaneously. 

Additionally, the shared memory includes a resource
allocation table, which tracks active locks (e.g., IntersectionB: [Train2, Train5]), and synchronization
metadata detailing intersection types (mutex/semaphore), capacities, and real-time lock states. This setup
enables the server to enforce access rules and monitor resource usage efficiently. */

/* The Resource Allocation Table should look something like this

	ID		Type		Capacity		Lock State		Holding Trains

	A       Mutex 		1				Locked			[Train 1]
	B		Semaphore 	2				Locked			[Train1, Train2]
	C 		Mutex 		1				Locked 			[Train 3]
	D		Semaphore 	3				Locked 			[Train 2, Train 3]
	E 		Mutex 		1 				Locked 			[Train 4]
*/


//Input will be in the form of an array of Vectors. I will pretend this array is called inputs.
//the first input is the name of the intersection, the second input is the # of trains it can handle

int main()
{
	printf("Hello\n");
	
	const char *inputNames[] = {"A","B","C"};
	int amount[] = {3,1,2};
	//const char *names[input.length];
	
	int fd;
	int SIZE = 1024;
	for(int i = 0; i < 3; i++) //hard coded, but fix
	{
		fd = shm_open(inputNames[i], O_CREAT | O_RDWR, 0666);
			
		/* Set the shared memory object size */
		ftruncate(fd, SIZE);
			
		if(amount[i] > 1)
		{
			printf("Printing Sem\n");
			sem_t * ptr;
			
			/* Map the shared memory object into the current address space */
			ptr = mmap(0, SIZE, PROT_WRITE, MAP_SHARED, fd, 0);
			
			if(sem_init(ptr,0, amount[i]) == -1)
				printf("Semaphore Failed\n");
		}
		else //mutex
		{
			printf("Printing Mutex\n");
			pthread_mutex_t * ptr;
			
			ptr = mmap(0, SIZE, PROT_WRITE, MAP_SHARED, fd, 0);
		
			if(pthread_mutex_init(ptr, NULL) == -1)
				printf("Mutex failed\n");
		
		}
	
		//munmap(ptr,size);
		
		close(fd);
	}	

	//TESTING

	/*fork();
	fork();
	printf("Hello 2\n");
	for(int i = 0; i < 3; i++)
	{
		fd = shm_open(inputNames[0], O_CREAT | O_RDWR, 0666);
		sem_t * ptr = (sem_t *)mmap(0, SIZE, PROT_WRITE, MAP_SHARED, fd, 0);
		sem_wait(ptr);
		printf("%d\n",i);
		sem_post(ptr);
		munmap(ptr,SIZE);
		close(fd);
	}
	
	for(int i = 0; i < 2; i++)
	{
		fd = shm_open(inputNames[1], O_CREAT | O_RDWR, 0666);
		pthread_mutex_t * ptr (pthread_mutex_t *) mmap(0, SIZE, PROT_WRITE, MAP_SHARED, fd, 0);
		m
	}
	return 0;*/
}
