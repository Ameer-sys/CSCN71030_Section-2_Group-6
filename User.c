#pragma warning(disable : 4996)

#include "User.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

User* userList = NULL;
int userCount = 0;
int loggedUser = -1;

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

    int userIndex = -1;
    int isAdmin = 0;

    //checks for admin
    for (int i = 0; i < userCount; i++) {
        if (userList[i].userID == loggedUser) {
            userIndex = i;
            isAdmin = (userList[i].role == ROLE_ADMIN);
            break;
        }
    }

    if (userIndex == -1) {
        printf("User not found!\n");
        return;
    }

    int targetUserID;
    int targetIndex;

    //admin can update any account
    if (isAdmin) {
        printf("Enter user ID to update (enter your own ID to update your account): ");
        scanf_s("%d", &targetUserID);

        //goes to targeted user
        targetIndex = -1;
        for (int i = 0; i < userCount; i++) {
            if (userList[i].userID == targetUserID) {
                targetIndex = i;
                break;
            }
        }

        if (targetIndex == -1) {
            printf("Target user not found!\n");
            return;
        }
    }
    else {
        //users can only update their own account
        targetIndex = userIndex;
    }

    printf("Enter new last name: ");
    scanf_s("%s", userList[targetIndex].Name[0], MAX_LENGTH);

    printf("Enter new first name: ");
    scanf_s("%s", userList[targetIndex].Name[1], MAX_LENGTH);

    printf("Enter new email: ");
    scanf_s("%s", userList[targetIndex].email, MAX_LENGTH);

    printf("Enter new password: ");
    scanf_s("%s", userList[targetIndex].password, MAX_LENGTH);

    //only admins can change roles
    if (isAdmin) {
        int newRole;
        printf("Enter new role (0 for Employee, 1 for Admin): ");
        scanf_s("%d", &newRole);

        //
        if (userList[targetIndex].role == ROLE_ADMIN && newRole != ROLE_ADMIN) {
            int adminCount = 0;
            for (int j = 0; j < userCount; j++) {
                if (userList[j].role == ROLE_ADMIN) {
                    adminCount++;
                }
            }

            if (adminCount <= 1) {
                printf("Cannot demote the only admin account! Role unchanged.\n");
            }
            else {
                userList[targetIndex].role = newRole;
            }
        }
        else {
            userList[targetIndex].role = newRole;
        }
    }

    saveUserList(userList, &userCount);  //save data
    printf("User updated successfully!\n");
}

void viewUser() {
    if (loggedUser == -1) {
        printf("You must be logged in to view profiles\n");
        return;
    }

    int isAdmin = 0;

    //checks if the admin is the one logged in
    for (int i = 0; i < userCount; i++) {
        if (userList[i].userID == loggedUser && userList[i].role == ROLE_ADMIN) {
            isAdmin = 1;
            break;
        }
    }

    if (isAdmin) {
        //admin can choose between all users or a speficifc one
        int choice;
        printf("1. View all users\n");
        printf("2. View specific user\n");
        printf("Choose an option: ");
        scanf_s("%d", &choice);

        if (choice == 1) {//this will print all users
            printf("\nAll Users:\n");
            printf("%-10s %-20s %-20s %-30s %-10s\n", "ID", "Last Name", "First Name", "Email", "Role");
            printf("--------------------------------------------------------------------------------\n");

            for (int i = 0; i < userCount; i++) {
                printf("%-10d %-20s %-20s %-30s %-10s\n",
                    userList[i].userID,
                    userList[i].Name[0],
                    userList[i].Name[1],
                    userList[i].email,
                    userList[i].role == ROLE_ADMIN ? "Admin" : "Employee");
            }
            return;
        }
    }

    // 
    int userID;
    if (isAdmin) {
        printf("Enter user ID to view: ");
        scanf_s("%d", &userID);
    }
    else {
        //users can only view their own profile
        userID = loggedUser;
    }

    for (int i = 0; i < userCount; i++) {
        if (userList[i].userID == userID) {
            printf("\nUser Profile:\n");
            printf("User ID: %d\n", userList[i].userID);
            printf("Name: %s %s\n", userList[i].Name[1], userList[i].Name[0]);
            printf("Email: %s\n", userList[i].email);
            printf("Role: %s\n", userList[i].role == ROLE_ADMIN ? "Administrator" : "Employee");
            return;
        }
    }

    if (isAdmin) {
        printf("User not found!\n");
    }
}

void initializeUserList() {
    userList = NULL;  //make userList empty
    userCount = 0;
    loadUserList(userList, userCount);  //loads data that might already exist
}

void loginUser() {
    char email[MAX_LENGTH], password[MAX_LENGTH];//asks for email and password
    printf("Enter email: ");
    scanf_s("%s", email, MAX_LENGTH);
    printf("Enter password: ");
    scanf_s("%s", password, MAX_LENGTH);

    for (int i = 0; i < userCount; i++) {//goes through each user and checks if the passowrd and email match
        if (strcmp(userList[i].email, email) == 0 && strcmp(userList[i].password, password) == 0) {//if the email and passowrd of the userList match the email and passowrd inputed it logs you in
            printf("Login successful! Welcome, %s %s\n", userList[i].Name[1], userList[i].Name[0]);
            if (userList[i].role == ROLE_ADMIN) {//if your role is admin it printd its out
                printf("You are logged in as Administrator\n");
            }
            else {
                printf("You are logged in as Employee\n");//if not admin this
            }
            loggedUser = userList[i].userID;  //and when login is succsful it returns the userid and puts it into loggedUser so that other actions are verifiable
            return;
        }
    }

    printf("Invalid credentials!\n");
    loggedUser = -1;
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