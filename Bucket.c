#pragma warning(disable : 4996)

#include "Bucket.h"
#include "User.h"
#include "Task.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h> // Include time.h

Bucket* bucketArray = NULL;
int bucketCount = 0; // Keep track of the number of buckets
Bucket* currentBucket = NULL;

void loadBucketData() {
    FILE* dataFile;
    dataFile = fopen("Bucket.txt", "r");

    if (dataFile == NULL) {
        printf("Failed to open 'Bucket.txt' file\n");
        return;
    }

    char lineTxt[1024];
    while (fgets(lineTxt, 1024, dataFile) != NULL) {
        // Initialize a bucket
        Bucket bucket;

        char* token = strtok(lineTxt, ",");
        if (token != NULL) {
            bucket.id = atoi(token);
        }

        token = strtok(NULL, ",");
        if (token != NULL) {
            strcpy(bucket.title, token);
        }

        token = strtok(NULL, ",");
        if (token != NULL) {
            bucket.createdDate = atol(token);
        }

        token = strtok(NULL, ",");
        if (token != NULL) {
            bucket.createdBy = atoi(token);
        }

        if (bucketCount == 0) {
            bucketArray = (Bucket*)malloc(sizeof(Bucket));
        }
        else {
            bucketArray = (Bucket*)realloc(bucketArray, sizeof(Bucket) * (bucketCount + 1));
        }

        if (bucketArray != NULL) {
            bucketArray[bucketCount] = bucket;
            bucketCount++;
        }
    }

    fclose(dataFile);
}

void displayAllBuckets() {
    if (bucketCount == 0) {
        printf("No buckets to display.\n");
        return;
    }

    printf("--- Bucket List ---\n");
    printf("+--------+----------------------+--------------------------+------------------+\n");
    printf("|   ID   |        Title         |       Created Date       |     Created By   |\n");
    printf("+--------+----------------------+--------------------------+------------------+\n");

    for (int i = 0; i < bucketCount; i++) {
        printf("| %6d | %20s | ", bucketArray[i].id, bucketArray[i].title);

        // Display created date
        time_t rawtime = bucketArray[i].createdDate;
        struct tm* timeinfo = localtime(&rawtime);

        // Format the date and time
        char formattedTime[20];
        strftime(formattedTime, sizeof(formattedTime), "%m/%d/%Y %H:%M:%S", timeinfo);
        printf("%24s ", formattedTime);

        User* user = getUserById(bucketArray[i].createdBy);
        if (user != NULL){
            printf("| %8s (%5s) |\n", user->Name[0], user->role == ROLE_ADMIN ? "Admin" : "Emplo");
        }
    }

    printf("+--------+----------------------+--------------------------+------------------+\n");
}

int navigateToBucket()
{
    int bucketId;
    printf("Enter ID of bucket you'd like to navigate to: ");
    scanf("%d", &bucketId);

    for (int i = 0; i < bucketCount; i++) {
        if (bucketArray[i].id == bucketId) {
            currentBucket = &bucketArray[i];
            return 1;
        }
    }

    printf("Bucket with ID %d not found!\n", bucketId);
    return 0;
}

int isTitleDuplicated(char title[]) {
    for (int i = 0; i < bucketCount; i++) {
        if (strcmp(bucketArray[i].title, title) == 0) return 1;
    }
    return 0;
}

void editBucketTitle() {
    int bucketId;
    printf("Enter the ID of the bucket to edit: ");
    scanf("%d", &bucketId);

    int bucketIndex = -1;
    for (int i = 0; i < bucketCount; i++) {
        if (bucketArray[i].id == bucketId) {
            bucketIndex = i;
            break;
        }
    }

    if (bucketIndex == -1) {
        printf("Bucket with ID %d not found!\n", bucketId);
        return;
    }

    char newTitle[256];
    printf("Enter the new title for the bucket: ");
    scanf(" %[^\n]s", newTitle);

    if (isTitleDuplicated(newTitle)) {
        printf("Bucket with title '%s' already exists! Please try again later.\n", newTitle);
        return;
    }

    strcpy(bucketArray[bucketIndex].title, newTitle);
    printf("Bucket title updated successfully.\n");
    saveBucketToFile();
}

void createBucket() {
    char title[256];

    printf("Enter the title for the new bucket: ");
    scanf(" %[^\n]s", title);

    if (isTitleDuplicated(title)) {
        printf("Bucket with title '%s' already exists! Please try again later.\n", title);
        return;
    }

    for (int i = 0; i < bucketCount; i++) {
        if (strcmp(bucketArray[i].title, title) == 0) {
            // If ID exists, try generating a new one
            printf("Bucket title '%s' already exists!\n", title);
            return;
        }
    }

    srand(time(NULL)); // Seed the random number generator
    int newId = rand() % 10000 + 10000; // Generates a random number between 10000 and 19999

    // Check if the generated ID already exists
    for (int i = 0; i < bucketCount; i++) {
        if (bucketArray[i].id == newId) {
            // If ID exists, try generating a new one
            newId = rand() % 10000 + 10000;
            break;
        }
    }

    Bucket newBucket;
    newBucket.id = newId;
    strcpy(newBucket.title, title);
    newBucket.createdDate = time(NULL); // Get current time as time_t
    newBucket.createdBy = loggedUser;

    if (bucketCount == 0) {
        bucketArray = (Bucket*)malloc(sizeof(Bucket));
    }
    else {
        bucketArray = (Bucket*)realloc(bucketArray, sizeof(Bucket) * (bucketCount + 1));
    }

    if (bucketArray != NULL) {
        bucketArray[bucketCount] = newBucket;
        bucketCount++;
        printf("Bucket created with ID: %d\n", newId);
        saveBucketToFile(); // Save the new bucket to the file
    }
}

void deleteBucket() {
    int bucketId;

    printf("Enter the ID of the bucket to delete: ");
    scanf("%d", &bucketId);

    int bucketIndex = -1;
    for (int i = 0; i < bucketCount; i++) {
        if (bucketArray[i].id == bucketId) {
            bucketIndex = i;
            break;
        }
    }

    if (bucketIndex == -1) {
        printf("Bucket with ID %d not found!\n", bucketId);
        return;
    }

    deleteAllTasksInBucket();

    // Shift remaining elements to overwrite the deleted one
    for (int i = bucketIndex; i < bucketCount - 1; i++) {
        bucketArray[i] = bucketArray[i + 1];
    }

    bucketCount -= 1;
    bucketArray = (Bucket*)realloc(bucketArray, sizeof(Bucket) * bucketCount);

    printf("Bucket with ID %d deleted.\n", bucketId);
    saveBucketToFile();  // Save the changes to the file
}

void closeBucketModule() {
    saveBucketToFile(); //save data before closing
    free(bucketArray);
}

void saveBucketToFile() {
    FILE* dataFile;
    dataFile = fopen("Bucket.txt", "w"); // Open in write mode, overwrites existing

    if (dataFile == NULL) {
        printf("Failed to open 'Bucket.txt' for writing.\n");
        return;
    }

    for (int i = 0; i < bucketCount; i++) {
        fprintf(dataFile, "%d,%s,%ld,%d",
            bucketArray[i].id,
            bucketArray[i].title,
            bucketArray[i].createdDate,
            bucketArray[i].createdBy);
        fprintf(dataFile, "\n");
    }

    fclose(dataFile);
}
