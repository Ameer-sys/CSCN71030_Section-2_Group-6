#pragma warning(disable : 4996)

#include <stdio.h>
#include <stdlib.h>
#include "Bucket.h"

int main(void) {
	loadBucketData();

	printf("Welcome to To Do List Application\n");

	while (1) {
		printf("\n");
		printf("To choose a function, enter its letter label:\n");
		printf("+ Bucket Management\n");
		printf("a) Display all buckets\n");
		printf("e) Navigate to a bucket\n");
		printf("b) Edit bucket title\n");
		printf("c) Create new bucket\n");
		printf("d) Delete a bucket\n");
		printf("\n");
		
		printf("+ User Management\n");
		printf("e) Create new user\n");
		printf("f) Edit user info\n");
		printf("g) Delete a user\n");
		printf("h) Search tasks assigned to a user\n");
		printf("i) Exit\n");

		char option[2];
		printf("Enter your option from a-i: ");
		scanf("%s.1", option);

		switch (option[0]) {
		case 'a':
			break;
		case 'b':
			break;
		case 'c':
			break;
		case 'd':
			break;
		case 'e':
			break;
		case 'f':
			break;
		case 'g':
			break;
		case 'h':
			break;
		case 'i':
			exit(0);
			break;
		default:
			printf("You've entered invalid option. Please try again!\n");
			break;
		}
	}
}