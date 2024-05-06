#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define MAX_LEN 1024

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <server_ip> <string_to_reverse>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int sockfd;
    struct sockaddr_in server_addr;

    // Create socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    // Server information
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(argv[1]);
    server_addr.sin_port = htons(PORT);

    // Connect to server
    connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));

    // Send string to server
    char *str = argv[2];
    printf("Sending to server: %s\n", str);
    send(sockfd, str, strlen(str), 0);

    // Receive reversed string from server
    char buffer[MAX_LEN] = {0};
    read(sockfd, buffer, MAX_LEN);
    printf("Received from server: %s\n", buffer);

    close(sockfd);
    return 0;
}

