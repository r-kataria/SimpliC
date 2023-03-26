#include "server.h"

int main(int argc, char *argv[]) {
    // Start the server
    int server_fd = start_server();
    if (server_fd < 0) {
        fprintf(stderr, "Failed to start server.\n");
        return EXIT_FAILURE;
    }

    printf("SimpliC server is running on port %d...\n", PORT);

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

        // Handle the client request
        handle_client(client_fd);

        // Close the client socket
        close(client_fd);
        printf("Closed connection from %s:%d\n",
               inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
    }

    // Close the server socket (unreachable code in this simple server)
    close(server_fd);
    return EXIT_SUCCESS;
}
