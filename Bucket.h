#pragma once

#include <time.h>

#define MAX_TASKS_PER_BUCKET 100

// Define Bucket structure
typedef struct bucket {
	int id;
	char title[256];
	time_t createdDate;
	int createdBy;
	int taskIds[MAX_TASKS_PER_BUCKET];
	int taskCount;
} Bucket;

Bucket* bucketArray;
int bucketCount;

// Funtions in Bucket Module
void loadBucketData();
void displayAllBuckets();
void editBucketTitle();
void createBucket();
void deleteBucket();
void closeBucketModule();
void saveBucketToFile();
