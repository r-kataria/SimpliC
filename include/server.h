#ifndef SERVER_H
#define SERVER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>

#define PORT 8080
#define BUFFER_SIZE 1024
#define ROOT_DIR "./www"
#define THREAD_POOL_SIZE 4  // Number of worker threads in the pool
#define REWRITE_CONFIG "./config/rewrites.conf" // Path to rewrite rules

// Function declarations
int start_server();
void handle_client(int client_fd);
const char* get_mime_type(const char *path);

// Structures for thread-safe request queue
typedef struct RequestNode {
    int client_fd;
    struct RequestNode *next;
} RequestNode;

typedef struct {
    RequestNode *front;
    RequestNode *rear;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
} RequestQueue;

// Initialize the request queue
void init_queue(RequestQueue *queue);

// Enqueue a new client_fd into the request queue
void enqueue(RequestQueue *queue, int client_fd);

// Dequeue a client_fd from the request queue
int dequeue(RequestQueue *queue);

// Worker thread function
void* worker_thread(void *arg);

// Global request queue
extern RequestQueue request_queue;

// Forward declaration of rewrite rules
typedef struct RewriteRule RewriteRule;

// Declare the global rewrite_rules variable
extern RewriteRule *rewrite_rules;

#endif // SERVER_H
