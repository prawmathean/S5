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
    int sockfd;
    char buffer[BUFFER_SIZE];
    char sentence[BUFFER_SIZE];
    struct sockaddr_in servaddr;
    
    // Creating UDP socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }
    
    memset(&servaddr, 0, sizeof(servaddr));
    
    // Server configuration
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    
    printf("Enter a new-generation sentence: ");
    if (fgets(sentence, sizeof(sentence), stdin) == NULL) {
        close(sockfd);
        return 0;
    }
    
    // Strip trailing newline character if present
    sentence[strcspn(sentence, "\n")] = '\0';
    
    // Send sentence to UDP server
    sendto(sockfd, (const char *)sentence, strlen(sentence), 0, (const struct sockaddr *)&servaddr, sizeof(servaddr));
    printf("Sent slang sentence to server.\n");
    
    // Receive formal translation back from server
    socklen_t len = sizeof(servaddr);
    int n = recvfrom(sockfd, (char *)buffer, BUFFER_SIZE - 1, 0, (struct sockaddr *)&servaddr, &len);
    buffer[n] = '\0';
    
    printf("Formal English translation: %s\n", buffer);
    
    close(sockfd);
    return 0;
}
