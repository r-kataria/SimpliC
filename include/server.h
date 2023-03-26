#ifndef SERVER_H
#define SERVER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <fcntl.h>

#define PORT 8080
#define BUFFER_SIZE 1024
#define ROOT_DIR "./www"

// Function declarations
int start_server();
void handle_client(int client_fd);

// Utility function to determine MIME type based on file extension
const char* get_mime_type(const char *path);

#endif // SERVER_H
