#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define MAX_LEN 1024

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

    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    bind(server_fd, (struct sockaddr *)&address, sizeof(address));
    listen(server_fd, 3);

    printf("Server started...waiting for connection...\n");

    while (1) {
        new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen);
        if (new_socket < 0) {
            perror("Accept failed");
            continue;
        }

        printf("Client connected...forking a new process...\n");

        pid_t pid = fork();
        if (pid < 0) {
            perror("Fork failed");
            close(new_socket); // Close socket in parent process
            continue;
        } else if (pid == 0) { // Child process
            close(server_fd); // Close server socket in child process

            read(new_socket, buffer, MAX_LEN);
            printf("Received from client: %s\n", buffer);
            reverseString(buffer);
            printf("Sending to client: %s\n", buffer);
            send(new_socket, buffer, strlen(buffer), 0);

            close(new_socket); // Close socket in child process
            exit(EXIT_SUCCESS); // Exit child process
        } else { // Parent process
            close(new_socket); // Close socket in parent process
        }
    }

    close(server_fd);
    return 0;
}

