/*
 * client.c
 *
 *  Created on: 23 nov 2024
 *      Author: Iliceto Domenico Francesco
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include "protocol.h"  // Include the protocol header

// Function to handle input and send it to the server
int handle_input(SOCKET server_socket) {
    char input[BUFFER_SIZE];  // Declare the input variable here
    char password[BUFFER_SIZE]; // Buffer for the received password
    int bytes_received;

    // Read user input for password request
    printf("Enter password type and length (e.g., n 8 for numeric 8-char password): ");
    fgets(input, sizeof(input), stdin);

    // Remove newline character from input (if present)
    input[strcspn(input, "\n")] = 0;

    // If the user enters 'q', close the connection and quit
    if (strcmp(input, "q") == 0) {
        printf("Exiting...\n");
        send(server_socket, input, strlen(input), 0);  // Send 'q' to the server to indicate quitting
        return 1; // Return 1 to indicate that we should quit
    }

    // Send the input to the server
    if (send(server_socket, input, strlen(input), 0) == SOCKET_ERROR) {
        printf("Error sending data to server.\n");
        return 0; // Return 0 to indicate failure
    }

    // Receive the password from the server
    bytes_received = recv(server_socket, password, sizeof(password) - 1, 0);
    if (bytes_received <= 0) {
        printf("Error receiving response from server.\n");
        return 0; // Return 0 if there was an error receiving the password
    }

    password[bytes_received] = '\0'; // Null-terminate the password string
    printf("Generated password: %s\n", password);

    return 0; // Return 0 to continue the loop
}

int main() {
    WSADATA wsaData;
    SOCKET client_socket;
    struct sockaddr_in server_addr;

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("WSAStartup failed\n");
        return 1;
    }

    // Create a socket for the client
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == INVALID_SOCKET) {
        printf("Socket creation failed: %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    // Setup server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);  // Use localhost IP (127.0.0.1) for local testing
    server_addr.sin_port = htons(SERVER_PORT);

    // Connect to the server
    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        printf("Connection failed: %d\n", WSAGetLastError());
        closesocket(client_socket);
        WSACleanup();
        return 1;
    }

    printf("Connected to the server.\n");

    // Handle user input and communication with the server in a loop
    while (1) {
        if (handle_input(client_socket) == 1) {
            break;  // Break the loop when 'q' is entered
        }
    }

    // Cleanup and close the socket
    closesocket(client_socket);
    WSACleanup();
    return 0;
}
