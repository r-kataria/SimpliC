# SimpliC

SimpliC is a simple, multithreaded HTTP web server written in C. It serves static files from a designated directory (`www`) and supports concurrent client connections using a thread pool.

## Getting Started

### Prerequisites

- GCC
- pthreads

### Building the Server

The project uses a `Makefile` to streamline the build process. All build files are placed within the `build/` directory.

```bash
mkdir build
make
./simipliC
```


## How It Works

1. **Server Initialization:**

   - The server starts by initializing a socket, binding it to the specified port (`8080`), and begins listening for incoming connections.

2. **Thread Pool Creation:**

   - A pool of worker threads is created at startup. Each thread runs the `worker_thread` function, which continuously dequeues and handles client requests.

3. **Accepting Connections:**

   - The main thread enters a loop where it accepts incoming client connections and enqueues their file descriptors into the request queue.

4. **Handling Requests:**
   - Worker threads dequeue client connections and process their HTTP `GET` requests, serving the requested static files with appropriate HTTP responses.



## Data Structures

### Request Queue

A thread-safe queue implemented using a linked list to manage incoming client connections. It employs mutexes and condition variables to synchronize access between the main thread (which enqueues client connections) and worker threads (which dequeue and process connections).

- **`RequestNode`:** Represents each client connection with the file descriptor.
- **`RequestQueue`:** Maintains pointers to the front and rear of the queue, along with a mutex and condition variable.

### Thread Pool

A fixed-size pool of worker threads that continuously listen for and process client requests from the request queue.

- **`THREAD_POOL_SIZE`:** Defines the number of worker threads in the pool.

