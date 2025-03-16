#include "User.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

User* registerUser(User* List, int* size) {
    *size += 1;  //increases the size of the list for each person the registers

    //uses realloc to rezie the list
    List = realloc(List, *size * sizeof(User));

    if (List == NULL) {
        printf("Memory allocation failed.\n");
        exit(1);
    }

    //generates userIDs randomly
    List[*size - 1].userID = rand() % 100;
    printf("Your UserID is: %d\n", List[*size - 1].userID);

    printf("Enter last name: ");
    scanf_s("%s", List[*size - 1].Name[0], MAX_LENGTH);

    printf("Enter first name: ");
    scanf_s("%s", List[*size - 1].Name[1], MAX_LENGTH);

    printf("Enter email: ");
    scanf_s("%s", List[*size - 1].email, MAX_LENGTH);

    printf("Enter password: ");
    scanf_s("%s", List[*size - 1].password, MAX_LENGTH);

    //sets roles
    if (*size == 1) {
        List[*size - 1].role = ROLE_ADMIN;  //first user becomes admin
        printf("You are registered as the Administrator.\n");
    }
    else {
        List[*size - 1].role = ROLE_EMPLOYEE;  //everyone else are employees
        printf("You are registered as an Employee.\n");
    }

    saveList(List, size);
    printf("User registered successfully!\n");
    return List;
}

void deleteUser(User* List, int* size, int loggeduser) {
    if (loggeduser == -1) {//this checks if the user is logged in
        printf("You must be logged in to delete profile\n");
        return;
    }

    int userIndex = -1;//holds the position of the loged in user, right now its set to -1 as here is no logged in user
    int isAdmin = 0;//defines isadmin as zero meaning there an admin

    //checkes if the logged in user is an admin
    for (int i = 0; i < *size; i++) {//goes through the whole list
        if (List[i].userID == loggeduser) {//if the userid in the list matches with the loggeduser it continues
            userIndex = i;//userindex is now at i where the loged user is
            isAdmin = (List[i].role == ROLE_ADMIN);//checks if its an admin
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
        for (int i = 0; i < *size; i++) {//goes through the list
            if (List[i].userID == targetUserID) {//unitl it finds and id that matches with the one choosen
                //admin account cannot be deleted
                if (List[i].role == ROLE_ADMIN) {//checks of the picked user is an admin
                    int adminCount = 0;
                    for (int j = 0; j < *size; j++) {
                        if (List[j].role == ROLE_ADMIN) {
                            adminCount++;
                        }
                    }

                    if (adminCount <= 1) {
                        printf("Cannot delete the only admin account!\n");
                        return;
                    }
                }


                for (int j = i; j < *size - 1; j++) {
                    List[j] = List[j + 1];
                }

                *size -= 1;  // Decrease size
                List = realloc(List, *size * sizeof(User));  //resize array after deletion

                if (List == NULL && *size > 0) {
                    printf("Memory reallocation failed.\n");
                    exit(1);
                }

                printf("User deleted successfully!\n");
                saveList(List, size);  //save new list

                //if user deletes themselves they go to logout
                if (targetUserID == loggeduser) {
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
        for (int j = userIndex; j < *size - 1; j++) {
            List[j] = List[j + 1];
        }

        *size -= 1;
        List = realloc(List, *size * sizeof(User));

        if (List == NULL && *size > 0) {
            printf("Memory reallocation failed.\n");
            exit(1);
        }

        printf("User deleted successfully!\n");
        saveList(List, size);
        return;
    }
}

void updateUser(User* List, int size, int loggeduser) {
    if (loggeduser == -1) {
        printf("You must be logged in to update your profile\n");
        return;
    }

    int userIndex = -1;
    int isAdmin = 0;

    //checks for admin
    for (int i = 0; i < size; i++) {
        if (List[i].userID == loggeduser) {
            userIndex = i;
            isAdmin = (List[i].role == ROLE_ADMIN);
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
        for (int i = 0; i < size; i++) {
            if (List[i].userID == targetUserID) {
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
    scanf_s("%s", List[targetIndex].Name[0], MAX_LENGTH);

    printf("Enter new first name: ");
    scanf_s("%s", List[targetIndex].Name[1], MAX_LENGTH);

    printf("Enter new email: ");
    scanf_s("%s", List[targetIndex].email, MAX_LENGTH);

    printf("Enter new password: ");
    scanf_s("%s", List[targetIndex].password, MAX_LENGTH);

    //only admins can change roles
    if (isAdmin) {
        int newRole;
        printf("Enter new role (0 for Employee, 1 for Admin): ");
        scanf_s("%d", &newRole);

        //
        if (List[targetIndex].role == ROLE_ADMIN && newRole != ROLE_ADMIN) {
            int adminCount = 0;
            for (int j = 0; j < size; j++) {
                if (List[j].role == ROLE_ADMIN) {
                    adminCount++;
                }
            }

            if (adminCount <= 1) {
                printf("Cannot demote the only admin account! Role unchanged.\n");
            }
            else {
                List[targetIndex].role = newRole;
            }
        }
        else {
            List[targetIndex].role = newRole;
        }
    }

    saveList(List, &size);  //save data
    printf("User updated successfully!\n");
}

void viewUser(User* List, int size, int loggeduser) {
    if (loggeduser == -1) {
        printf("You must be logged in to view profiles\n");
        return;
    }

    int isAdmin = 0;

    //checks if the admin is the one logged in
    for (int i = 0; i < size; i++) {
        if (List[i].userID == loggeduser && List[i].role == ROLE_ADMIN) {
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

            for (int i = 0; i < size; i++) {
                printf("%-10d %-20s %-20s %-30s %-10s\n",
                    List[i].userID,
                    List[i].Name[0],
                    List[i].Name[1],
                    List[i].email,
                    List[i].role == ROLE_ADMIN ? "Admin" : "Employee");
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
        userID = loggeduser;
    }

    for (int i = 0; i < size; i++) {
        if (List[i].userID == userID) {
            printf("\nUser Profile:\n");
            printf("User ID: %d\n", List[i].userID);
            printf("Name: %s %s\n", List[i].Name[1], List[i].Name[0]);
            printf("Email: %s\n", List[i].email);
            printf("Role: %s\n", List[i].role == ROLE_ADMIN ? "Administrator" : "Employee");
            return;
        }
    }

    if (isAdmin) {
        printf("User not found!\n");
    }
}

void initializeList(User** List, int* size) {
    *List = NULL;  //make list empty
    *size = 0;
    loadList(List, size);  //loads data that might already exist
}

int loginUser(User* List, int size) {
    char email[MAX_LENGTH], password[MAX_LENGTH];//asks for email and password
    printf("Enter email: ");
    scanf_s("%s", email, MAX_LENGTH);
    printf("Enter password: ");
    scanf_s("%s", password, MAX_LENGTH);

    for (int i = 0; i < size; i++) {//goes through each user and checks if the passowrd and email match
        if (strcmp(List[i].email, email) == 0 && strcmp(List[i].password, password) == 0) {//if the email and passowrd of the list match the email and passowrd inputed it logs you in
            printf("Login successful! Welcome, %s %s\n", List[i].Name[1], List[i].Name[0]);
            if (List[i].role == ROLE_ADMIN) {//if your role is admin it printd its out
                printf("You are logged in as Administrator\n");
            }
            else {
                printf("You are logged in as Employee\n");//if not admin this
            }
            return List[i].userID;  //and when login is succsful it returns the userid and puts it into loggeduser so that other actions are verifiable
        }
    }

    printf("Invalid credentials!\n");
    return -1;
}

//save data
void saveList(User* List, int* size) {
    FILE* file = fopen("users.dat", "wb");
    if (file == NULL) {
        printf("Error opening file for writing!\n");
        return;
    }

    //write number of users
    fwrite(size, sizeof(int), 1, file);

    //write each user
    fwrite(List, sizeof(User), *size, file);

    fclose(file);
    printf("User data saved successfully!\n");
}

//load data
void loadList(User** List, int* size) {
    FILE* file = fopen("users.dat", "rb");
    if (file == NULL) {
        printf("No existing user data found. Starting with empty list.\n");
        return;
    }

    //read users
    fread(size, sizeof(int), 1, file);

    // allocate memory for the users
    *List = (User*)malloc(*size * sizeof(User));
    if (*List == NULL && *size > 0) {
        printf("Memory allocation failed!\n");
        fclose(file);
        exit(1);
    }

    //read users
    fread(*List, sizeof(User), *size, file);

    fclose(file);
    printf("Loaded %d users from file.\n", *size);
}