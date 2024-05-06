#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int port = atoi(argv[1]);

    // Create socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Error in socket creation");
        exit(EXIT_FAILURE);
    }

    // Specify server address
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Replace "your_ip_address" with your actual IP address
    server_addr.sin_port = htons(port);

    // Connect to server
    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Error in connecting to server");
        exit(EXIT_FAILURE);
    }

    printf("TCP client connected to %s on port %d\n", inet_ntoa(server_addr.sin_addr), port);

    char buffer[BUFFER_SIZE];

    while (1) {
        printf("Enter an expression in the following format:\noperand1 operator operand2\nValid operators are + - * / ^.\nTo quit, enter -1.\n");
        fgets(buffer, BUFFER_SIZE, stdin);

        // Send expression to server
        write(sockfd, buffer, strlen(buffer));

        // Receive answer from server
        read(sockfd, buffer, BUFFER_SIZE);

        printf("ANS: %s\n", buffer);

        if (strcmp(buffer, "Bye!") == 0) {
            break;
        }
    }

    close(sockfd);
    return 0;
}

