#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define MAX_LEN 1024

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <server_ip> <server_port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int sockfd;
    struct sockaddr_in server_addr;

    // Create socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    // Server information
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(argv[1]);
    server_addr.sin_port = htons(atoi(argv[2]));

    // Send request to server
    sendto(sockfd, "TIME_REQUEST", sizeof("TIME_REQUEST"), 0, (const struct sockaddr *)&server_addr, sizeof(server_addr));

    char buffer[MAX_LEN];
    socklen_t addr_len = sizeof(server_addr);

    // Receive time of day information from server
    recvfrom(sockfd, buffer, MAX_LEN, 0, (struct sockaddr *)&server_addr, &addr_len);

    printf("Received time of day information from server: %s\n", buffer);

    close(sockfd);
    return 0;
}

