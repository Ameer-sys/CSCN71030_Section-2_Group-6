#pragma once

#include <iostream>

// Define User Module
typedef struct user {
	char fullName[2][256];
	char alias[64];
	int role;
} User;

// Functions operating in User Module
User createNewUser(char lastName[], char firstName[], int role, char alias[] = NULL);	// Function that adds new user in database
User deleteUser(int userId);	// Function to delete an existed user, return the result of that deleted user
void editUser(int userId, User editValue);	// Function to edit an existed user