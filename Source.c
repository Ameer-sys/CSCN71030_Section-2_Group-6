#pragma warning(disable : 4996)

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "Bucket.h"
#include "User.h"
#include "Task.h"

int main(void) {
	initializeUserList();

	printf("Welcome to To Do List Application\n");

	while (1) {
		if (loggedUser != -1) break;
		printf("\nChoose an option:\n");
		printf("1) Register\n");
		printf("2) Login\n");
		printf("3) Exit\n");
		printf("Enter option: ");
		int choice;
		scanf("%d", &choice);

		switch (choice) {
		case 1:
			registerUser();
			saveUserList();
			break;
		case 2:
			loginUser();
			break;
		case 3:
			printf("Exiting...\n");
			closeUserModule();
			break;
		default:
			printf("Invalid option! Try again.\n");
			break;
		}
	}

	loadBucketData();

	int menuId = 0;
	while (1) {
		if (loggedUser == -1) break;
		if (menuId == 0) {
			printf("\n");
			printf("To choose a function, enter its letter label:\n");
			printf("+ Bucket Management\n");
			printf("a) Display all buckets\n");
			printf("b) Navigate to a bucket\n");
			printf("c) Edit bucket title\n");
			printf("d) Create new bucket\n");
			printf("e) Delete a bucket\n");
			printf("v) View Task details\n");
			printf("\n");

			if (isAdmin) {
				printf("+ Administration\n");
				printf("i) Display all users\n");
				printf("j) Create new user\n");
				printf("k) Edit a user\n");
				printf("l) Delete a user\n");
				printf("m) Search tasks assigned to a user\n");
			}
			else {
				printf("+ User Management\n");
				printf("f) View my profile\n");
				printf("g) Update my profile\n");
				printf("h) Delete My Account\n");
			}
			printf("x) Exit\n");

			char option[2];
			printf("Enter your option from a-i: ");
			scanf("%s.1", option);

			switch (option[0]) {
			case 'a':
				displayAllBuckets();
				break;
			case 'b':
				menuId = navigateToBucket();
				break;
			case 'c':
				editBucketTitle();
				break;
			case 'd':
				createBucket();
				break;
			case 'e':
				deleteBucket();
				break;
			case 'v':
				viewTask();
				break;
			case 'f':
				if (!isAdmin) {
					viewUser();
				}
				break;
			case 'g':
				if (!isAdmin) {
					updateUser();
					saveUserList();
				}
				break;
			case 'h':
				if (!isAdmin) {
					deleteUser();
					saveUserList();
					loggedUser = -1;
				}
				break;
			case 'i':
				if (isAdmin) {
					displayAllUsers();
				}
				break;
			case 'j':
				if (isAdmin) {
					createUser();
					saveUserList();
				}
				break;
			case 'k':
				if (isAdmin) {
					updateUser();
					saveUserList();
				}
				break;
			case 'l':
				if (isAdmin) {
					deleteUser();
					saveUserList();
				}
				break;
			case 'm':
				break;
			case 'x':
				closeBucketModule();
				closeUserModule();
				exit(0);
				break;
			default:
				printf("You've entered invalid option. Please try again!\n");
				break;
			}
		}
		else {
			printf("\n");
			printf("=== %s ===\n", currentBucket->title);
			printf("Choose a function to manipulate tasks:\n");
			printf("a) Display all tasks\n");
			printf("b) Create a new task\n");
			printf("c) Edit a task\n");
			printf("d) Delete a task\n");
			printf("e) View One Task\n");
			printf("f) Back to main menu\n");
			printf("\n");

			char option[2];
			printf("Enter your option from a-e: ");
			scanf("%s.1", option);

			switch (option[0]) {
			case 'a':
				displayTasks();
				break;
			case 'b':
				addTask();
				break;
			case 'c':
				editTask();
				break;
			case 'd':
				deleteTask();
				break;
			case 'e':
				viewTask();
				break;
			case 'f':
				menuId = 0;
				break;
			default:
				printf("You've entered invalid option. Please try again!\n");
				break;
			}
		}
	}
}