#pragma warning(disable : 4996)

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "Bucket.h"
#include "User.h"

int main(void) {
	loadBucketData();
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


	while (1) {
		if (loggedUser == -1) break;
		printf("\n");
		printf("To choose a function, enter its letter label:\n");
		printf("+ Bucket Management\n");
		printf("a) Display all buckets\n");
		printf("b) Navigate to a bucket\n");
		printf("c) Edit bucket title\n");
		printf("d) Create new bucket\n");
		printf("e) Delete a bucket\n");
		printf("\n");

		if (isAdmin) {
			printf("+ Administration\n");
			printf("i) Create new user\n");
			printf("j) Edit a user\n");
			printf("k) Delete a user\n");
			printf("l) Search tasks assigned to a user\n");
			printf("m) Exit\n");
		}
		else {
			printf("+ User Management\n");
			printf("f) View my profile\n");
			printf("g) Update my profile\n");
			printf("h) Delete My Account\n");
			printf("i) Exit\n");
		}
		

		char option[2];
		printf("Enter your option from a-i: ");
		scanf("%s.1", option);

		switch (option[0]) {
		case 'a':
			displayAllBuckets();
			break;
		case 'b':
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
				createUser();
				saveUserList();
			}
			break;
		case 'j':
			if (isAdmin) {
				updateUser();
				saveUserList();
			}
			break;
		case 'k':
			if (isAdmin) {
				deleteUser();
				saveUserList();
			}

			break;
		case 'l':
			break;
		case 'm':
			closeBucketModule();
			closeUserModule();
			exit(0);
			break;
		default:
			printf("You've entered invalid option. Please try again!\n");
			break;
		}
	}
}