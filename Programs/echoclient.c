#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define MAX_LEN 1024

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <server_ip> \"<message>\"\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int sockfd;
    struct sockaddr_in server_addr;
    char buffer[1024];

    // Create socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    // Server information
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(argv[1]);
    server_addr.sin_port = htons(PORT);

    // Send data to server
    sendto(sockfd, argv[2], strlen(argv[2]), 0, (const struct sockaddr *)&server_addr, sizeof(server_addr));

    // Receive echoed message from server
    int len = recvfrom(sockfd, buffer, 1024, 0, NULL, NULL);
    buffer[len] = '\0';

    printf("Received from server: %s\n", buffer);

    close(sockfd);
    return 0;
}

