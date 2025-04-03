#pragma once

#include <time.h>

#define MAX_TASKS_PER_BUCKET 100

// Define Bucket structure
typedef struct bucket {
	int id;
	char title[256];
	time_t createdDate;
	int createdBy;
} Bucket;

Bucket* bucketArray;
int bucketCount;
Bucket* currentBucket;

// Funtions in Bucket Module
void loadBucketData();
void displayAllBuckets();
int navigateToBucket();
int isTitleDuplicated(char title[]);
void editBucketTitle();
void createBucket();
void deleteBucket();
void closeBucketModule();
void saveBucketToFile();
