#pragma warning(disable : 4996)

#include "Bucket.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Bucket* bucketArray = NULL;

void loadBucketData()
{
	FILE* dataFile;
	dataFile = fopen("Bucket.txt", "r");

	if (dataFile == NULL) {
		printf("Failed to open 'Bucket.txt' file\n");
		return;
	}

	int count = 0;
	char lineTxt[1024];
	while (fgets(lineTxt, 1024, dataFile) != NULL) {
		// Initialize a bucket
		Bucket bucket;
		// 
		char* token = strtok(lineTxt, ",");
		bucket.id = atoi(token);

		token = strtok(NULL, ",");
		strcpy(bucket.title, token);

		// Keep printing tokens while one of the
		// delimiters present in str[].
		int taskIndex = 0;
		while ((token = strtok(NULL, ",")) != NULL)
		{
			printf("%s\n", token);
			bucket.taskIds[taskIndex] = atoi(token);
			taskIndex++;
		}
		if (count == 0) {
			bucketArray = (Bucket*)malloc(sizeof(Bucket));
			if (bucketArray != NULL) *bucketArray = bucket;
		}
		else {
			bucketArray = (Bucket*)realloc(bucketArray, sizeof(Bucket) * (count + 1));
			if (bucketArray != NULL) bucketArray[count] = bucket;
		}
		count++;
	}

	fclose(dataFile);
}

void displayTaskList()
{

}

void editBucketTitle(int bucketId, char newTitle[])
{
}

void createBucket(char title[])
{
}

void deleteBucket(int bucketId)
{
}

void closeBucketModule()
{
	free(bucketArray);
}
