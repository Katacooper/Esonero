/*
 * password_generation.c
 *
 *  Created on: 23 nov 2024
 *      Author: gilic
 */


#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "protocol.h"  // Include the protocol header for declarations

// Function to generate a numeric password
void generate_numeric(char *password, int length) {
    const char digits[] = "0123456789";
    for (int i = 0; i < length; i++) {
        password[i] = digits[rand() % 10];  // Randomly pick a digit
    }
    password[length] = '\0';  // Null-terminate the string
}

// Function to generate an alphabetic password (lowercase only)
void generate_alpha(char *password, int length) {
    const char alphabet[] = "abcdefghijklmnopqrstuvwxyz";
    for (int i = 0; i < length; i++) {
        password[i] = alphabet[rand() % 26];  // Randomly pick a letter
    }
    password[length] = '\0';  // Null-terminate the string
}

// Function to generate a mixed password (letters and numbers)
void generate_mixed(char *password, int length) {
    const char alpha_numeric[] = "abcdefghijklmnopqrstuvwxyz0123456789";
    for (int i = 0; i < length; i++) {
        password[i] = alpha_numeric[rand() % 36];  // Randomly pick a letter or number
    }
    password[length] = '\0';  // Null-terminate the string
}

// Function to generate a secure password (letters, numbers, and symbols)
void generate_secure(char *password, int length) {
    const char secure_set[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!@#$%^&*()_-+=<>?/";
    for (int i = 0; i < length; i++) {
        password[i] = secure_set[rand() % 72];  // Randomly pick a character from the secure set
    }
    password[length] = '\0';  // Null-terminate the string
}
