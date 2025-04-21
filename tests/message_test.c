/*
 * Group C
 *
 * Name: Em Jordan
 * Email: em.jordan@okstate.edu
 * Date: 4/17/2025
 * Description: Ensures that both trains and servers alike send/receive data as they are supposed to
 */
#include "message.h"
#include "test_runner.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define MAX_QUEUE_SIZE 1024
#define CLIENT_COUNT 3

// Simple macros that will make logging our processes easy
#define LOG_MSG(format, ...) printf(format "\n", __VA_ARGS__)
#define CLIENT_MSG_NO_NEWLINE(name, msg) printf("[CLIENT: %s] " msg, name)
#define CLIENT_MSG(name, msg) printf("[CLIENT: %s] " msg "\n", name)
#define LOG_CLIENT(name, format, ...) printf("[CLIENT: ] " format "\n", __VA_ARGS__)

#define SERVER_MSG(msg) printf("[SERVER] " msg "\n")
#define LOG_SERVER(format, ...) printf("[SERVER] " format "\n", __VA_ARGS__)

// Global references to our message queue pointers
message_queue_t *request_queue;
message_queue_t *response_queue;

// Creates a basic queue to share between processes using `mmap`
message_queue_t *create_queue(size_t capacity) {
    // Create our dynamic array for our queue
    message_t *items = mmap(
        NULL,
        capacity * sizeof(message_t),
        PROT_READ | PROT_WRITE,
        MAP_SHARED | MAP_ANONYMOUS,
        -1,
        0);

    // Create a separate mapping in memory for our actual queue
    message_queue_t *queue = mmap(
        NULL, sizeof(message_queue_t), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);

    memset(items, capacity, MESSAGE_OPEN_SLOT);

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

void client(const char *name, const char *intersection) {
    // CLIENT_MSG(name, "Initialized client");

    // Attempt to acquire Intersection D
    send_request(
        request_queue,
        (message_t){
            .type = REQUEST_ACQUIRE, .src = name, .dst = "SERVER", .data = {.acquire = intersection }});

    message_t msg = wait_for_response(response_queue, name);

    // CLIENT_MSG_NO_NEWLINE(name, "Received message from server: ");
    // LOG_MSG("%s", message_type_name(msg.type));
}

bool client_should_receive_message() {
    request_queue = create_queue(MAX_QUEUE_SIZE);
    response_queue = create_queue(MAX_QUEUE_SIZE);

    destroy_queue(request_queue);
    destroy_queue(response_queue);

    return true;
}

bool server_should_handle_three_clients() {
    request_queue = create_queue(MAX_QUEUE_SIZE);
    response_queue = create_queue(MAX_QUEUE_SIZE);

    // Run three separate clients with different behaviors
    pid_t clients[CLIENT_COUNT];

    for (int i = 0; i < CLIENT_COUNT; i++) {
        pid_t result = fork();
        if (result == 0) {
            // Run our clients based off our indices
            switch (i) {
            case 0: {
                client("1", "D");
            } break;
            case 1: {
                client("2", "E");
            } break;
            case 2: {
                client("3", "F");
            } break;
            }
            exit(EXIT_SUCCESS);
        } else {
            // Store our PIDs in our parent's array
            clients[i] = result;
        }
    }

    // Run server
    int messages_left = CLIENT_COUNT;
    while (messages_left > 0) {
        // if we don't have a message to process, ignore it
        if (request_queue->length <= 0 || queue_peek(request_queue).type == MESSAGE_OPEN_SLOT)
            continue;

        message_t msg = queue_dequeue(request_queue);

        // LOG_SERVER("Received message from %s: %s (%d / %d)", msg.src, message_type_name(msg.type), messages_left, CLIENT_COUNT);

        // Give the client whatever they want
        queue_enqueue(response_queue, (message_t){
            .src = "SERVER",
            .dst = msg.src,
            .type = RESPONSE_GRANT,
            .data = (message_data_t){ .grant = msg.data.acquire }
        });
        messages_left -= 1;
    }


    // Wait for clients to close
    for (int i = 0; i < CLIENT_COUNT; i++) {
        waitpid(clients[i], NULL, 0);
    }

    destroy_queue(request_queue);
    destroy_queue(response_queue);
    return true;

}

int main() {
    test_data_t tests[] = {
        (test_data_t){ .name = "Client should receive message", .func = client_should_receive_message },
        (test_data_t){ .name = "Server should handle three clients", .func = server_should_handle_three_clients },
    };
    test_all(tests, sizeof(tests) / sizeof(test_data_t));
}
