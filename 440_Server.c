#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define MAX_BUFFER_SIZE 1024
#define PORT 8484

void handle_client(int client_socket) {
    char buffer[1024];
    char combined_text[1024];
    time_t current_time;
    struct tm *time_info;
    ssize_t num_bytes;

    while (1) {
        num_bytes = recv(client_socket, buffer, sizeof(buffer), 0);
        if (num_bytes <= 0) {
            break;
        }

        buffer[num_bytes] = '\0';

        // Get the current date and time
        current_time = time(NULL);
        time_info = localtime(&current_time);

        if (strcmp(buffer, "Exit\n") == 0) {
            // Send "Goodbye!" response to the client
            send(client_socket, "Goodbye!", strlen("Goodbye!"), 0);
            break;
        } else {
            // Combine the received text with the current date and time
            strftime(combined_text, sizeof(combined_text), "Text from client received at %d %B %Y, %H:%M: ", time_info);
            strncat(combined_text, buffer, sizeof(combined_text) - strlen(combined_text) - 1);

            // Send the combined string back to the client
            send(client_socket, combined_text, strlen(combined_text), 0);
        }
    }

    printf("Client connection closed.\n");
    close(client_socket);
}

int main() {
    int serverSocket, newSocket, clientLen;
    struct sockaddr_in serverAddr, clientAddr;
    char buffer[MAX_BUFFER_SIZE];

    // Create server socket
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        perror("Failed to create socket");
        exit(EXIT_FAILURE);
    }

    // Set server address parameters
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8484);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    // Bind server socket to the specified address and port
    if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("Failed to bind socket");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(serverSocket, 1) < 0) {
        perror("Failed to listen for connections");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d...\n", PORT);

    // Accept a new connection
    clientLen = sizeof(clientAddr);
    newSocket = accept(serverSocket, (struct sockaddr *)&clientAddr, (socklen_t *)&clientLen);
    if (newSocket < 0) {
        perror("Failed to accept connection");
        exit(EXIT_FAILURE);
    }

    printf("Connected to a client.\n");

    handle_client(newSocket);

    // Close the server socket
    close(serverSocket);

    printf("Server terminated.\n");

    return 0;
}
