#include "server.h"

// Define the global request queue
RequestQueue request_queue;

int main(int argc, char *argv[]) {
    (void)argc; // Suppress unused parameter warning
    (void)argv; // Suppress unused parameter warning

    // Start the server
    int server_fd = start_server();
    if (server_fd < 0) {
        fprintf(stderr, "Failed to start server.\n");
        return EXIT_FAILURE;
    }

    printf("SimpliC server is running on port %d...\n", PORT);

    // Initialize the request queue
    init_queue(&request_queue);

    // Create a pool of worker threads
    pthread_t threads[THREAD_POOL_SIZE];
    for (int i = 0; i < THREAD_POOL_SIZE; i++) {
        if (pthread_create(&threads[i], NULL, worker_thread, NULL) != 0) {
            perror("pthread_create failed");
            close(server_fd);
            return EXIT_FAILURE;
        }
    }

    // Accept loop: continuously accept new client connections
    while (1) {
        struct sockaddr_in client_addr;
        socklen_t addr_len = sizeof(client_addr);

        // Accept an incoming connection
        int client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &addr_len);
        if (client_fd < 0) {
            perror("accept failed");
            continue;
        }

        printf("Accepted connection from %s:%d\n",
               inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

        // Enqueue the new client connection for processing by worker threads
        enqueue(&request_queue, client_fd);
    }

    // Cleanup (unreachable in this simple server)
    close(server_fd);
    return EXIT_SUCCESS;
}
