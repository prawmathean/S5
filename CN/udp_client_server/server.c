#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 8080
#define BUFFER_SIZE 2048

// Structure to pair an abbreviation with its formal translation
typedef struct {
    const char *abbrev;
    const char *formal;
} SlangMap;

// Dictionary mapping slang to formal English
SlangMap dictionary[] = {
    {"tbh", "to be honest"},
    {"ig", "I guess"},
    {"tbf", "to be fair"},
    {"atm", "at the moment"},
    {"irl", "in real life"},
    {"lol", "laughing out loud"},
    {"asap", "as soon as possible"},
    {"omg", "oh my god"},
    {"ttyl", "talk to you later"},
    {"idk", "I do not know"},
    {"nvm", "never mind"}
};

int dict_size = sizeof(dictionary) / sizeof(dictionary[0]);

// Function to look up an abbreviation in the dictionary (case-insensitive)
const char* lookup_slang(const char *word) {
    char lower_word[128];
    int i = 0;
    
    // Convert current word to lowercase for matching
    while (word[i] && i < 127) {
        lower_word[i] = tolower((unsigned char)word[i]);
        i++;
    }
    lower_word[i] = '\0';

    for (int j = 0; j < dict_size; j++) {
        if (strcmp(lower_word, dictionary[j].abbrev) == 0) {
            return dictionary[j].formal;
        }
    }
    return NULL;
}

// Function to parse the sentence word by word and translate it
void translate_sentence(const char *input, char *output) {
    char temp_input[BUFFER_SIZE];
    strcpy(temp_input, input);
    
    output[0] = '\0';
    
    // Tokenize text using whitespace and typical punctuation delimiters
    char *delimiters = " \t\r\n,.!?;:";
    char *word = strtok(temp_input, delimiters);
    const char *current_pos = input;

    while (word != NULL) {
        // Find where this word originally starts in the input text to preserve leading punctuation/spaces
        const char *word_ptr = strstr(current_pos, word);
        
        // Copy everything between the last processed position and the start of the current word
        int gap = word_ptr - current_pos;
        if (gap > 0) {
            strncat(output, current_pos, gap);
        }

        // Check if the isolated word matches an abbreviation
        const char *translation = lookup_slang(word);
        if (translation != NULL) {
            strcat(output, translation);
        } else {
            strcat(output, word); // Keep original word if no slang matches
        }

        // Move target index past the processed word
        current_pos = word_ptr + strlen(word);
        word = strtok(NULL, delimiters);
    }
    
    // Copy any remaining trailing characters/punctuation
    strcat(output, current_pos);
}

int main() {
    int sockfd;
    char buffer[BUFFER_SIZE];
    char translated[BUFFER_SIZE];
    struct sockaddr_in servaddr, cliaddr;
    
    // Creating UDP socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }
    
    memset(&servaddr, 0, sizeof(servaddr));
    memset(&cliaddr, 0, sizeof(cliaddr));
    
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);
    
    // Bind the socket with the server address
    if (bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("Bind failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    
    printf("UDP Translation Server running on port %d...\n", PORT);
    
    while (1) {
        socklen_t len = sizeof(cliaddr);
        int n = recvfrom(sockfd, (char *)buffer, BUFFER_SIZE - 1, 0, (struct sockaddr *)&cliaddr, &len);
        buffer[n] = '\0';
        
        printf("Received slang text: %s\n", buffer);
        
        // Perform slang to formal English translation
        translate_sentence(buffer, translated);
        
        printf("Translated text: %s\n", translated);
        
        // Send formal sentence back to client
        sendto(sockfd, (const char *)translated, strlen(translated), 0, (const struct sockaddr *)&cliaddr, len);
    }
    
    close(sockfd);
    return 0;
}
