#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define MAX_LEN 1024

// Function to reverse a string
void reverseString(char *str) {
    int len = strlen(str);
    for (int i = 0; i < len / 2; i++) {
        char temp = str[i];
        str[i] = str[len - i - 1];
        str[len - i - 1] = temp;
    }
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[MAX_LEN] = {0};

    // Create socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    // Server configuration
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Bind socket to port
    bind(server_fd, (struct sockaddr *)&address, sizeof(address));

    // Listen for connections
    listen(server_fd, 3);

    printf("Server started...waiting for connection...\n");

    // Accept incoming connection
    new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen);

    // Read string from client and reverse it
    read(new_socket, buffer, MAX_LEN);
    printf("Received from client: %s\n", buffer);
    reverseString(buffer);

    // Send reversed string back to client
    printf("Sending to client: %s\n", buffer);
    send(new_socket, buffer, strlen(buffer), 0);

    close(new_socket);
    close(server_fd);
    return 0;
}

