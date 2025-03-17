#pragma warning(disable : 4996)

#include "User.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

User* userList = NULL;
int userCount = 0;
int loggedUser = -1;
int isAdmin = 0;


User* getUserById(int userId)
{
    for (int i = 0; i < userCount; i++) {
        if (userList[i].userID == userId) {
            return &userList[i];
        }
    }

    return NULL;
}

User* registerUser() {
    srand(time(NULL));
    userCount += 1;  //increases the userCount of the userList for each person the registers

    //uses realloc to rezie the userList
    userList = realloc(userList, userCount * sizeof(User));

    if (userList == NULL) {
        printf("Memory allocation failed.\n");
        exit(1);
    }

    //generates userIDs randomly
    userList[userCount - 1].userID = rand() % 100;
    printf("Your UserID is: %d\n", userList[userCount - 1].userID);

    printf("Enter last name: ");
    scanf_s("%s", userList[userCount - 1].Name[0], MAX_LENGTH);

    printf("Enter first name: ");
    scanf_s("%s", userList[userCount - 1].Name[1], MAX_LENGTH);

    printf("Enter email: ");
    scanf_s("%s", userList[userCount - 1].email, MAX_LENGTH);

    printf("Enter password: ");
    scanf_s("%s", userList[userCount - 1].password, MAX_LENGTH);

    //sets roles
    if (userCount == 1) {
        userList[userCount - 1].role = ROLE_ADMIN;  //first user becomes admin
        printf("You are registered as the Administrator.\n");
    }
    else {
        userList[userCount - 1].role = ROLE_EMPLOYEE;  //everyone else are employees
        printf("You are registered as an Employee.\n");
    }

    saveUserList(userList, userCount);
    printf("User registered successfully!\n");
    return userList;
}

void displayAllUsers()
{
    printf("--- User List ---\n");
    printf("+--------+----------------------+--------------------------+--------------+\n");
    printf("|   ID   |         Name         |           Email          |      Role    |\n");
    printf("+--------+----------------------+--------------------------+--------------+\n");

    for (int i = 0; i < userCount; i++) {
        char roleStr[16];
        if (userList[i].role == 0) {
            strcpy(roleStr, "Admin");
        }
        else {
            strcpy(roleStr, "Employee");
        }
        printf("| %6d | %10s %9s | %24s | %12s |\n", userList[i].userID, userList[i].Name[0], userList[i].Name[1], userList[i].email, roleStr);
    }

    printf("+--------+----------------------+--------------------------+--------------+\n");
}

void deleteUser() {
    if (loggedUser == -1) {//this checks if the user is logged in
        printf("You must be logged in to delete profile\n");
        return;
    }

    int userIndex = -1;//holds the position of the loged in user, right now its set to -1 as here is no logged in user
    int isAdmin = 0;//defines isadmin as zero meaning there an admin

    //checkes if the logged in user is an admin
    for (int i = 0; i < userCount; i++) {//goes through the whole userList
        if (userList[i].userID == loggedUser) {//if the userid in the userList matches with the loggedUser it continues
            userIndex = i;//userindex is now at i where the loged user is
            isAdmin = (userList[i].role == ROLE_ADMIN);//checks if its an admin
            break;
        }
    }

    if (userIndex == -1) {//if userindex isnt chagned means there is no user there
        printf("User not found!\n");
        return;
    }

    int targetUserID;//this signifies what user the admin wants to delete

    //admin can delete any account
    if (isAdmin) {//makes sure admin is here
        printf("Enter user ID to delete (enter your own ID to delete your account): ");
        scanf_s("%d", &targetUserID);

        //fins user
        for (int i = 0; i < userCount; i++) {//goes through the userList
            if (userList[i].userID == targetUserID) {//unitl it finds and id that matches with the one choosen
                //admin account cannot be deleted
                if (userList[i].role == ROLE_ADMIN) {//checks of the picked user is an admin
                    int adminCount = 0;
                    for (int j = 0; j < userCount; j++) {
                        if (userList[j].role == ROLE_ADMIN) {
                            adminCount++;
                        }
                    }

                    if (adminCount <= 1) {
                        printf("Cannot delete the only admin account!\n");
                        return;
                    }
                }


                for (int j = i; j < userCount - 1; j++) {
                    userList[j] = userList[j + 1];
                }

                userCount -= 1;  // Decrease userCount
                userList = realloc(userList, userCount * sizeof(User));  //reuserCount array after deletion

                if (userList == NULL && userCount > 0) {
                    printf("Memory reallocation failed.\n");
                    exit(1);
                }

                printf("User deleted successfully!\n");
                saveUserList(userList, userCount);  //save new userList

                //if user deletes themselves they go to logout
                if (targetUserID == loggedUser) {
                    return;
                }
                return;
            }
        }
        printf("Target user not found!\n");
        return;
    }
    else {
        //user can only delete themselves
        for (int j = userIndex; j < userCount - 1; j++) {
            userList[j] = userList[j + 1];
        }

        userCount -= 1;
        userList = realloc(userList, userCount * sizeof(User));

        if (userList == NULL && userCount > 0) {
            printf("Memory reallocation failed.\n");
            exit(1);
        }

        printf("User deleted successfully!\n");
        saveUserList(userList, userCount);
        return;
    }
}

void updateUser() {
    if (loggedUser == -1) {
        printf("You must be logged in to update your profile\n");
        return;
    }

    if (isAdmin) {
      
        
    }
    else {
       
        printf("Enter new last name: ");
        scanf_s("%s", loggedInUser.Name[0], MAX_LENGTH);

        printf("Enter new first name: ");
        scanf_s("%s", loggedInUser.Name[1], MAX_LENGTH);

        printf("Enter new email: ");
        scanf_s("%s", loggedInUser.email, MAX_LENGTH);

        printf("Enter new password: ");
        scanf_s("%s", loggedInUser.password, MAX_LENGTH);

        
        for (int i = 0; i < userCount; i++) {
            if (userList[i].userID == loggedInUser.userID) {
                userList[i] = loggedInUser;
                break;
            }
        }

        saveUserList();
        printf("User updated successfully!\n");
    }
}

void viewUser() {
    if (loggedUser == -1) {
        printf("You must be logged in to view profiles\n");
        return;
    }

    if (isAdmin) {
      
    }
    else {
        
        printf("\nUser Profile:\n");
        printf("User ID: %d\n", loggedInUser.userID);
        printf("Name: %s %s\n", loggedInUser.Name[1], loggedInUser.Name[0]);
        printf("Email: %s\n", loggedInUser.email);
        printf("Role: %s\n", loggedInUser.role == ROLE_ADMIN ? "Administrator" : "Employee");
    }
}

void initializeUserList() {
    userList = NULL;  //make userList 
    userCount = 0;
    loadUserList(userList, userCount);  //loads data that might already exist
}

void loginUser() {
    char email[MAX_LENGTH], password[MAX_LENGTH];
    printf("Enter email: ");
    scanf_s("%s", email, MAX_LENGTH);
    printf("Enter password: ");
    scanf_s("%s", password, MAX_LENGTH);

    for (int i = 0; i < userCount; i++) {
        if (strcmp(userList[i].email, email) == 0 && strcmp(userList[i].password, password) == 0) {
           
            loggedUser = userList[i].userID;

        
            loggedInUser = userList[i];

        
            isAdmin = (loggedInUser.role == ROLE_ADMIN);

            printf("Login successful! Welcome, %s %s\n",
                loggedInUser.Name[1], loggedInUser.Name[0]);

            if (isAdmin) {
                printf("You are logged in as Administrator\n");
            }
            else {
                printf("You are logged in as Employee\n");
            }
            return;
        }
    }

    printf("Invalid credentials!\n");
    loggedUser = -1;
    isAdmin = 0;
   //rests to zero
    memset(&loggedInUser, 0, sizeof(User));
}

int createUser() {
    srand(time(NULL));
    userCount += 1;  

    // uses realloc to resize the userList
    userList = realloc(userList, userCount * sizeof(User));

    if (userList == NULL) {
        printf("Memory allocation failed.\n");
        exit(1);
    }

    // generates userIDs randomly
    userList[userCount - 1].userID = rand() % 100;
    printf("New UserID is: %d\n", userList[userCount - 1].userID);

    printf("Enter last name: ");
    scanf_s("%s", userList[userCount - 1].Name[0], MAX_LENGTH);

    printf("Enter first name: ");
    scanf_s("%s", userList[userCount - 1].Name[1], MAX_LENGTH);

    printf("Enter email: ");
    scanf_s("%s", userList[userCount - 1].email, MAX_LENGTH);

    printf("Enter password: ");
    scanf_s("%s", userList[userCount - 1].password, MAX_LENGTH);

    
    int roleChoice;
    printf("Enter role (0 for Employee, 1 for Admin): ");
    scanf_s("%d", &roleChoice);

    if (roleChoice == ROLE_ADMIN || roleChoice == ROLE_EMPLOYEE) {
        userList[userCount - 1].role = roleChoice;
    }
    else {
       
        userList[userCount - 1].role = ROLE_EMPLOYEE;
        printf("Invalid role input. Setting as Employee by default.\n");
    }

   
    saveUserList(userList, userCount);
    printf("User created successfully!\n");

    return 1;
}
void closeUserModule()
{
    saveUserList();
    free(userList);
}

//save data
void saveUserList() {
    FILE* file = fopen("users.dat", "wb");
    if (file == NULL) {
        printf("Error opening file for writing!\n");
        return;
    }

    //write number of users
    fwrite(&userCount, sizeof(int), 1, file);

    //write each user
    fwrite(userList, sizeof(User), userCount, file);

    fclose(file);
    printf("User data saved successfully!\n");
}

//load data
void loadUserList() {
    userCount = 0;

    FILE* file = fopen("users.dat", "rb");
    if (file == NULL) {
        printf("No existing user data found. Starting with empty userList.\n");
        return;
    }

    //read users
    fread(&userCount, sizeof(int), 1, file);

    // allocate memory for the users
    userList = (User*)malloc(userCount * sizeof(User));
    if (userList == NULL && userCount > 0) {
        printf("Memory allocation failed!\n");
        fclose(file);
        exit(1);
    }

    //read users
    fread(userList, sizeof(User), userCount, file);

    fclose(file);
    printf("Loaded %d users from file.\n", userCount);
}