#pragma warning(disable : 4996)

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "Bucket.h"

int main(void) {
	loadBucketData();

	printf("Welcome to To Do List Application\n");

	while (1) {
		printf("\n");
		printf("To choose a function, enter its letter label:\n");
		printf("+ Bucket Management\n");
		printf("a) Display all buckets\n");
		printf("b) Navigate to a bucket\n");
		printf("c) Edit bucket title\n");
		printf("d) Create new bucket\n");
		printf("e) Delete a bucket\n");
		printf("\n");
		
		printf("+ User Management\n");
		printf("f) Create new user\n");
		printf("g) Edit user info\n");
		printf("h) Delete a user\n");
		printf("i) Search tasks assigned to a user\n");
		printf("k) Exit\n");

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
			break;
		case 'g':
			break;
		case 'h':
			break;
		case 'i':
			break;
		case 'k':
			closeBucketModule();
			exit(0);
			break;
		default:
			printf("You've entered invalid option. Please try again!\n");
			break;
		}
	}
}