#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 2048

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char filename[BUFFER_SIZE];
    char buffer[BUFFER_SIZE];
    pid_t server_pid;

    printf("Enter the filename to request from server: ");
    if (fgets(filename, sizeof(filename), stdin) == NULL) {
        return 0;
    }
    filename[strcspn(filename, "\n")] = '\0';

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        printf("\nInvalid address/ Address not supported \n");
        close(sock);
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed \n");
        close(sock);
        return -1;
    }

    // 1. Receive the PID of the server process handling our request
    if (recv(sock, &server_pid, sizeof(server_pid), 0) > 0) {
        printf("Connected to Server Process PID: %d\n", server_pid);
    }

    // 2. Send the target filename to the server
    send(sock, filename, strlen(filename), 0);
    printf("Requested file: '%s'\n\n--- File Content Received ---\n", filename);

    // 3. Read data from server until the socket closes
    int bytes_received;
    int is_empty = 1;
    
    while ((bytes_received = recv(sock, buffer, BUFFER_SIZE - 1, 0)) > 0) {
        buffer[bytes_received] = '\0';
        printf("%s", buffer);
        is_empty = 0;
    }
    printf("\n-----------------------------\n");

    if (is_empty) {
        printf("Server closed connection without data stream response.\n");
    }

    close(sock);
    return 0;
}
