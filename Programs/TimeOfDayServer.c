#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>

#define PORT 8080
#define MAX_LEN 1024

int main() {
    int sockfd;
    struct sockaddr_in server_addr, client_addr;
    char buffer[MAX_LEN];
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

    while (1) {
        // Receive request from client
        recvfrom(sockfd, buffer, MAX_LEN, 0, (struct sockaddr *)&client_addr, &addr_len);

        // Get current time
        time_t rawtime;
        time(&rawtime);
        snprintf(buffer, MAX_LEN, "%s", ctime(&rawtime));

        // Send time of day information to client
        sendto(sockfd, buffer, strlen(buffer), 0, (const struct sockaddr *)&client_addr, addr_len);

        printf("Time of day information sent to client\n");
    }

    close(sockfd);
    return 0;
}

