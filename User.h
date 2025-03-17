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

User* userList;
int userCount;
int loggedUser;
//User loggedInUser;

//functions
User* registerUser();  //register user
void deleteUser();  //delete user
void updateUser();   //update user
void saveUserList();        //save data
void loadUserList();      //load data
void viewUser();  //view profiles
void initializeUserList(); //initialize list
void loginUser();        //login function
void closeUserModule();
