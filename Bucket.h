#pragma once

// Define Bucket structure
typedef struct bucket {
	int id;
	char title[256];
	int taskIds[100];
} Bucket;

Bucket* bucketArray;

// Funtions in Bucket Module
void loadBucketData();
void displayTaskList();
void editBucketTitle(int bucketId, char newTitle[]);
void createBucket(char title[]);
void deleteBucket(int bucketId);
void closeBucketModule();
