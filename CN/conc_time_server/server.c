#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

#define PORT 8080
#define BUFFER_SIZE 2048

// Zombie process cleanup handler
void sigchld_handler(int s) {
    (void)s; // Avoid unused parameter warning
    while(waitpid(-1, NULL, WNOHANG) > 0);
}

void handle_client(int client_socket) {
    char filename[BUFFER_SIZE];
    char buffer[BUFFER_SIZE];
    FILE *file;
    pid_t my_pid = getpid();

    // 1. Send the handling process's PID to the client first
    send(client_socket, &my_pid, sizeof(my_pid), 0);

    // 2. Receive the filename requested by the client
    int bytes_received = recv(client_socket, filename, BUFFER_SIZE - 1, 0);
    if (bytes_received <= 0) {
        close(client_socket);
        exit(0);
    }
    filename[bytes_received] = '\0';
    
    // Strip trailing newlines or spaces if any
    filename[strcspn(filename, "\r\n")] = '\0';
    printf("[Child Process %d] Client requested file: %s\n", my_pid, filename);

    // 3. Open the file and send its content, or handle missing file
    file = fopen(filename, "rb");
    if (file == NULL) {
        printf("[Child Process %d] File not found. Notifying client.\n", my_pid);
        strcpy(buffer, "ERROR: File not found on server.");
        send(client_socket, buffer, strlen(buffer), 0);
    } else {
        printf("[Child Process %d] File found. Transferring contents...\n", my_pid);
        size_t bytes_read;
        while ((bytes_read = fread(buffer, 1, BUFFER_SIZE, file)) > 0) {
            send(client_socket, buffer, bytes_read, 0);
        }
        fclose(file);
        printf("[Child Process %d] File transfer complete.\n", my_pid);
    }

    close(client_socket);
    exit(0); // Exit child process safely
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    struct sigaction sa;

    // Create TCP Socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 10) < 0) {
        perror("Listen");
        exit(EXIT_FAILURE);
    }

    // Configure signal handler to clean up terminated child processes (prevent zombies)
    sa.sa_handler = sigchld_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

    printf("Concurrent TCP File Server listening on port %d...\n", PORT);

    while (1) {
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
            perror("Accept");
            continue;
        }

        // Fork a child process to handle concurrency natively
        pid_t pid = fork();

        if (pid < 0) {
            perror("Fork failed");
            close(new_socket);
        } 
        else if (pid == 0) {
            // Child Process context
            close(server_fd); // The child does not need the master listening socket
            handle_client(new_socket);
        } 
        else {
            // Parent Process context
            close(new_socket); // The parent processes do not keep individual client sockets open
        }
    }

    close(server_fd);
    return 0;
}
