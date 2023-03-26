#include "server.h"

// Function to determine MIME type based on file extension
const char* get_mime_type(const char *path) {
    const char *ext = strrchr(path, '.');
    if (!ext) return "application/octet-stream";
    ext++; // Skip the dot

    if (strcmp(ext, "html") == 0) return "text/html";
    if (strcmp(ext, "css") == 0) return "text/css";
    if (strcmp(ext, "js") == 0) return "application/javascript";
    if (strcmp(ext, "png") == 0) return "image/png";
    if (strcmp(ext, "jpg") == 0 || strcmp(ext, "jpeg") == 0) return "image/jpeg";
    if (strcmp(ext, "gif") == 0) return "image/gif";
    if (strcmp(ext, "txt") == 0) return "text/plain";
    // Add more MIME types as needed

    return "application/octet-stream";
}

// Function to start the server
int start_server() {
    int server_fd;
    struct sockaddr_in address;
    int opt = 1;

    // Creating socket file descriptor (IPv4, TCP)
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        return -1;
    }

    // Forcefully attaching socket to the PORT
    // Modify this part to set SO_REUSEADDR only
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        perror("setsockopt failed");
        close(server_fd);
        return -1;
    }

    // Define the server address
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY; // Listen on all interfaces
    address.sin_port = htons(PORT);

    // Bind the socket to the address and port
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        close(server_fd);
        return -1;
    }

    // Start listening for incoming connections
    if (listen(server_fd, 10) < 0) { // 10 is the backlog size
        perror("listen failed");
        close(server_fd);
        return -1;
    }

    return server_fd;
}

// Function to handle client requests
void handle_client(int client_fd) {
    char buffer[BUFFER_SIZE];
    memset(buffer, 0, BUFFER_SIZE);

    // Read the client's request
    ssize_t bytes_read = read(client_fd, buffer, BUFFER_SIZE - 1);
    if (bytes_read < 0) {
        perror("read failed");
        return;
    }

    printf("Received request:\n%s\n", buffer);

    // Simple parsing of the HTTP request
    char method[16], path[256];
    sscanf(buffer, "%s %s", method, path);

    // Only handle GET requests
    if (strcmp(method, "GET") != 0) {
        const char *response =
            "HTTP/1.1 501 Not Implemented\r\n"
            "Content-Type: text/plain\r\n"
            "Content-Length: 19\r\n"
            "\r\n"
            "501 Not Implemented";
        write(client_fd, response, strlen(response));
        return;
    }

    // If path is '/', serve 'index.html'
    if (strcmp(path, "/") == 0) {
        strcpy(path, "/index.html");
    }

    // Construct the full file path
    char file_path[512];
    snprintf(file_path, sizeof(file_path), "%s%s", ROOT_DIR, path);

    // Open the file
    int file_fd = open(file_path, O_RDONLY);
    if (file_fd < 0) {
        perror("open failed");
        const char *not_found =
            "HTTP/1.1 404 Not Found\r\n"
            "Content-Type: text/plain\r\n"
            "Content-Length: 13\r\n"
            "\r\n"
            "404 Not Found";
        write(client_fd, not_found, strlen(not_found));
        return;
    }

    // Get file size
    struct stat st;
    if (fstat(file_fd, &st) < 0) {
        perror("fstat failed");
        close(file_fd);
        return;
    }

    // Read file content
    char *file_content = malloc(st.st_size);
    if (!file_content) {
        perror("malloc failed");
        close(file_fd);
        return;
    }

    ssize_t total_read = 0;
    while (total_read < st.st_size) {
        ssize_t r = read(file_fd, file_content + total_read, st.st_size - total_read);
        if (r < 0) {
            perror("read file failed");
            free(file_content);
            close(file_fd);
            return;
        }
        if (r == 0) break; // EOF
        total_read += r;
    }

    close(file_fd);

    // Prepare HTTP response header
    char header[512];
    snprintf(header, sizeof(header),
             "HTTP/1.1 200 OK\r\n"
             "Content-Length: %lld\r\n"  // Changed %ld to %lld
             "Content-Type: %s\r\n"
             "\r\n",
             (long long)st.st_size, get_mime_type(file_path));

    // Send header
    write(client_fd, header, strlen(header));

    // Send file content
    write(client_fd, file_content, st.st_size);

    free(file_content);
}
