#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LENGTH 100 //defines the name length

//defines roles
#define ROLE_EMPLOYEE 0
#define ROLE_ADMIN 1

//user moduel
typedef struct {
	int userID;
	char Name[2][MAX_LENGTH];  //2d array for first and last name
	char email[MAX_LENGTH];
	char password[MAX_LENGTH];
	int role;
} User;

//functions
User* registerUser(User* List, int* size);  //register user
void deleteUser(User* List, int* size, int loggeduser);  //delete user
void updateUser(User* List, int size, int loggeduser);   //update user
void saveList(User* List, int* size);        //save data
void loadList(User** List, int* size);      //load data
void viewUser(User* List, int size, int loggeduser);  //view profiles
void initializeList(User** List, int* size); //initialize list
int loginUser(User* List, int size);        //login function