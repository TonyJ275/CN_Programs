#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define MAX_LEN 1024

int main() {
    int sockfd;
    struct sockaddr_in server_addr, client_addr;
    char buffer[1024];
    socklen_t addr_len = sizeof(client_addr);

    // Create socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    // Server information
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Bind socket
    bind(sockfd, (const struct sockaddr *)&server_addr, sizeof(server_addr));

    printf("Server started...waiting for connection...\n");

    // Receive data from client and echo back
    while (1) {
        int len = recvfrom(sockfd, buffer, 1024, 0, (struct sockaddr *)&client_addr, &addr_len);
        buffer[len] = '\0';
        printf("Received from client: %s\n", buffer);
        
        // Echo back to client
        sendto(sockfd, buffer, strlen(buffer), 0, (const struct sockaddr *)&client_addr, addr_len);
    }

    close(sockfd);
    return 0;
}

