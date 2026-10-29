#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 8080

void identify_matrix_type(int n, int matrix[n][n], char *result) {
    int is_upper = 1;
    int is_lower = 1;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i > j && matrix[i][j] != 0) {
                is_upper = 0; // Elements below diagonal are not zero
            }
            if (i < j && matrix[i][j] != 0) {
                is_lower = 0; // Elements above diagonal are not zero
            }
        }
    }

    if (is_upper && is_lower) {
        strcpy(result, "Diagonal Matrix");
    } else if (is_upper) {
        strcpy(result, "Upper Triangular Matrix");
    } else if (is_lower) {
        strcpy(result, "Lower Triangular Matrix");
    } else {
        strcpy(result, "Normal/Ordinary Matrix (Not triangular or diagonal)");
    }
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

    srand(time(NULL));

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

    if (listen(server_fd, 3) < 0) {
        perror("Listen");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d...\n", PORT);

    while (1) {
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
            perror("Accept");
            exit(EXIT_FAILURE);
        }

        int n;
        if (recv(new_socket, &n, sizeof(n), 0) <= 0) {
            close(new_socket);
            continue;
        }

        printf("Received matrix size N = %d from client.\n", n);

        int matrix[n][n];
        printf("Generated Matrix:\n");
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                // Generates random numbers between 1 and 50
                matrix[i][j] = (rand() % 50) + 1; 
                printf("%2d ", matrix[i][j]);
            }
            printf("\n");
        }

        char result[100];
        identify_matrix_type(n, matrix, result);
        printf("Identified Type: %s\n", result);

        send(new_socket, result, strlen(result) + 1, 0);
        close(new_socket);
        printf("Result sent to client. Connection closed.\n\n");
    }

    close(server_fd);
    return 0;
}
