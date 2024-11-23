/*
 * server.c
 *
 *  Created on: 23 nov 2024
 *      Author: Iliceto Domenico Francesco
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include "protocol.h"  // Include the protocol header

// Function to handle client communication
void handle_client(SOCKET client_socket) {
    char input[BUFFER_SIZE];
    char password[BUFFER_SIZE];
    int bytes_received;

    while (1) {
        // Receive the request from the client
        bytes_received = recv(client_socket, input, sizeof(input) - 1, 0);
        if (bytes_received <= 0) {
            printf("Error receiving data or client disconnected.\n");
            break;
        }

        input[bytes_received] = '\0';  // Null-terminate the received string
        printf("Received request: %s\n", input);

        // If the client requested to quit
        if (strcmp(input, "q") == 0) {
            printf("Client requested to quit.\n");
            break;
        }

        // Process the password request based on the input
        char type = input[0]; // First character is the password type
        int length = atoi(&input[2]); // Length follows the space

        if (length < 6 || length > 32) {
            printf("Password length must be between 6 and 32 characters.\n");
            strcpy(password, "ERROR");
        } else {
            switch (type) {
                case PASSWORD_TYPE_NUMERIC:
                    generate_numeric(password, length);
                    break;
                case PASSWORD_TYPE_ALPHA:
                    generate_alpha(password, length);
                    break;
                case PASSWORD_TYPE_MIXED:
                    generate_mixed(password, length);
                    break;
                case PASSWORD_TYPE_SECURE:
                    generate_secure(password, length);
                    break;
                default:
                    printf("Invalid password type.\n");
                    strcpy(password, "ERROR");
                    break;
            }
        }

        // Send the generated password to the client
        send(client_socket, password, strlen(password), 0);
    }

    // Close the connection to the client
    closesocket(client_socket);
}

int main() {
    WSADATA wsaData;
    SOCKET server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    int client_len;

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("WSAStartup failed\n");
        return 1;
    }

    // Create a socket for the server
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == INVALID_SOCKET) {
        printf("Socket creation failed: %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    // Setup server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);  // Use localhost IP (127.0.0.1) for local testing
    server_addr.sin_port = htons(SERVER_PORT);

    // Bind the socket
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        printf("Bind failed: %d\n", WSAGetLastError());
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }

    // Listen for incoming connections
    if (listen(server_socket, 5) == SOCKET_ERROR) {
        printf("Listen failed: %d\n", WSAGetLastError());
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }

    printf("Server is listening for connections...\n");

    // Accept incoming connections
    while (1) {
        client_len = sizeof(client_addr);
        client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_len);
        if (client_socket == INVALID_SOCKET) {
            printf("Accept failed: %d\n", WSAGetLastError());
            continue;
        }

        printf("New connection from %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

        // Handle the client in a separate function
        handle_client(client_socket);
    }

    // Cleanup and close the server socket
    closesocket(server_socket);
    WSACleanup();
    return 0;
}
