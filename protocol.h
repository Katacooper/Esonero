/*
 * protocol.h
 *
 *  Created on: 23 nov 2024
 *      Author: Iliceto Domenico Francesco
 */

#ifndef PROTOCOL_H
#define PROTOCOL_H

// Define buffer sizes
#define BUFFER_SIZE 128

// Define server default values
#define SERVER_PORT 12345
#define SERVER_IP "127.0.0.1"

// Define the password types for requests
#define PASSWORD_TYPE_NUMERIC 'n'
#define PASSWORD_TYPE_ALPHA 'a'
#define PASSWORD_TYPE_MIXED 'm'
#define PASSWORD_TYPE_SECURE 's'

// Function prototypes for password generation
void generate_numeric(char *password, int length);
void generate_alpha(char *password, int length);
void generate_mixed(char *password, int length);
void generate_secure(char *password, int length);

#endif // PROTOCOL_H
