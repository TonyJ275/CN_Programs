#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h> // Add this line to fix the inet_ntoa warning
#include <math.h> // Add this line to fix the pow warning

#define BUFFER_SIZE 1024

double evaluate_expression(char *expression) {
    double operand1, operand2;
    char operator;
    sscanf(expression, "%lf %c %lf", &operand1, &operator, &operand2);

    switch (operator) {
        case '+':
            return operand1 + operand2;
        case '-':
            return operand1 - operand2;
        case '*':
            return operand1 * operand2;
        case '/':
            if (operand2 != 0)
                return operand1 / operand2;
            else
                return -1; // Division by zero
        case '^':
            return pow(operand1, operand2);
        default:
            return -1; // Invalid operator
    }
}

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
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(port);

    // Bind socket
    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Error in binding");
        exit(EXIT_FAILURE);
    }

    // Listen for connections
    if (listen(sockfd, 5) < 0) {
        perror("Error in listening");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d\n", port);

    while (1) {
        // Accept connection
        int client_sockfd = accept(sockfd, (struct sockaddr *)&client_addr, &client_len);
        if (client_sockfd < 0) {
            perror("Error in accepting connection");
            exit(EXIT_FAILURE);
        }

        printf("Client %s on port %d connected\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

        char buffer[BUFFER_SIZE];

        while (1) {
            // Receive data from client
            read(client_sockfd, buffer, BUFFER_SIZE);

            if (strcmp(buffer, "-1\n") == 0 || strcmp(buffer, "-1\r\n") == 0) {
                write(client_sockfd, "Bye!", strlen("Bye!") + 1);
                break;
            }

            // Evaluate expression
            double result = evaluate_expression(buffer);

            // Send result back to client
            sprintf(buffer, "%.2lf", result);
            write(client_sockfd, buffer, strlen(buffer) + 1);
        }

        close(client_sockfd);
        printf("Client closed connection\n");
    }

    close(sockfd);
    return 0;
}

