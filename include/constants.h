/*
* Group C
*
* Description: Set of constants in the system. Add as needed.
*
*/
#ifndef CONSTANTS_H
#define CONSTANTS_H

#define MAX_INTERSECTIONS 20
#define MAX_TRAINS 20
#define MAX_NAME_LENGTH 50
#define MAX_ROUTE_LENGTH 20
#define MAX_MESSAGE_SIZE 100
#define MAX_LOG_SIZE 200

// Intersection types
#define MUTEX_TYPE 1
#define SEMAPHORE_TYPE 2

// Message types
#define ACQUIRE_REQUEST 1
#define RELEASE_REQUEST 2
#define GRANT_RESPONSE 3
#define WAIT_RESPONSE 4
#define DENY_RESPONSE 5

#endif
