#include "../include/message.h"
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define MAX_QUEUE_SIZE 1024
#define CLIENT_COUNT 3

// Global references to our message queue pointers
message_queue_t *request_queue;
message_queue_t *response_queue;
// Whether our server is running or not
bool *running;

// Creates a basic queue to share between processes using `mmap`
message_queue_t *create_queue(size_t capacity) {
  // Create our dynamic array for our queue
  message_t *items = mmap(NULL, capacity * sizeof(message_t),
                          PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);

  // Create a separate mapping in memory for our actual queue
  message_queue_t *queue = mmap(NULL, sizeof(message_queue_t),
                                PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);


  // Initialize our queue data 
  queue_init(queue);



  // Initialize items & capacity of queue 
  queue->items = items;
  queue->capacity = capacity;
  return queue;
}

// Destroys the given message queue
void destroy_queue(message_queue_t *queue) {
  munmap(queue->items, queue->capacity * sizeof(message_t));
  munmap(queue, sizeof(message_queue_t));
}

// Handles SIGINT for our server process
void server_handler(int sig) { *running = false; }

void server() {
  while (*running) { 
    // if we don't have a message to process, ignore it
    if (request_queue->length <= 0)
      continue;

    message_t msg = queue_dequeue(request_queue);
    handle_request(request_queue, msg);
  }
}

// Simple macros that will make logging our processes easy
#define CLIENT_MSG(name, msg) printf("[CLIENT: " name "] " msg "\n")
#define LOG_CLIENT(name, format, ...)                                          \
  printf("[CLIENT: " name "] " format "\n", __VA_ARGS__)

#define SERVER_MSG(name, msg) printf("[SERVER] " msg "\n")
#define LOG_SERVER(format, ...) printf("[SERVER]" format "\n", __VA_ARGS__)

void client_a() {
  CLIENT_MSG("A", "Initialized client");

  // Attempt to acquire Intersection D
  send_request(request_queue, (message_t){.type = REQUEST_ACQUIRE, .src = "A", .dst = "SERVER", .data = {.acquire = "D"}});

  message_t msg = wait_for_response(response_queue, "A"); 
}

void client_b() {
  CLIENT_MSG("B", "Initialized client");

  // Attempt to acquire Intersection E
  send_request(request_queue, (message_t){.type = REQUEST_ACQUIRE, .src = "B", .dst = "SERVER", .data = {.acquire = "E"}});

  message_t msg = wait_for_response(response_queue, "B"); 
}

void client_c() {
  CLIENT_MSG("C", "Initialized client");

  // Attempt to acquire Intersection E
  send_request(request_queue, (message_t){.type = REQUEST_ACQUIRE, .src = "C", .dst = "SERVER", .data = {.acquire = "F"}});

  message_t msg = wait_for_response(response_queue, "C"); 
}

int main() {
  request_queue = create_queue(MAX_QUEUE_SIZE);
  response_queue = create_queue(MAX_QUEUE_SIZE);


  // set our running state to true -- we should care about sync here but this is
  // just a small demonstration
  running =
      mmap(NULL, sizeof(bool), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
  *running = true;

  // Run three separate clients with different behaviors
  pid_t clients[CLIENT_COUNT];

    for (int i = 0; i < CLIENT_COUNT; i++) {
        pid_t result = fork();
        if (result != 0) {
            // Run our clients based off our indices
            switch (i) {
                case 0: {
                    client_a();
                } break;
                case 1: {
                    client_b();
                } break;
                case 2: {
                    client_c();
                } break;
            }
        } else {
            // Store our PIDs in our parent's array
            clients[i] = result;
        }
    }

  // Add a signal handler so we can use CTRL-C to gracefully exit our program
  signal(SIGINT, server_handler);
  // Run server
  server();

  // Wait for clients to close
  for (int i = 0; i < CLIENT_COUNT; i++) {
    waitpid(clients[i], NULL, 0);
  }
  return 0;
}
