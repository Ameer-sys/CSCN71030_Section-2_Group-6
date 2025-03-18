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

        // Initialize taskIds array and counter
        bucket.taskCount = 0;

        // Load task IDs
        while ((token = strtok(NULL, ",")) != NULL) {
            int taskId = atoi(token);
            if (bucket.taskCount < MAX_TASKS_PER_BUCKET) {
                bucket.taskIds[bucket.taskCount] = taskId;
                bucket.taskCount++;
            }
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
    printf("+--------+----------------------+--------------------------+----------------------+\n");
    printf("|   ID   |        Title         |       Created Date       |       Created By     |\n");
    printf("+--------+----------------------+--------------------------+----------------------+\n");

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
            printf("| %8s (%8s) |\n", user->Name[0], user->role ? "Employee" : "Admin");
        }
    }

    printf("+--------+----------------------+--------------------------+----------------------+\n");
}

void navigateToBucket()
{
    int bucketId;
    printf("Enter ID of bucket you'd like to navigate to: ");
    scanf("%d", &bucketId);

    for (int i = 0; i < bucketCount; i++) {
        if (bucketArray[i].id == bucketId) {
            currentBucket = &bucketArray[i];
            break;
        }
    }
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
    scanf(" %[^\n]s", newTitle); // Read line with spaces

    strcpy(bucketArray[bucketIndex].title, newTitle);
    printf("Bucket title updated successfully.\n");
    saveBucketToFile();
}

void createBucket() {
    srand(time(NULL)); // Seed the random number generator
    char title[256];

    printf("Enter the title for the new bucket: ");
    scanf(" %[^\n]s", title); // Read line with spaces

    // Generate a random ID (you can adjust the range as needed)
    int newId = rand() % 10000 + 10000; // Generates a random number between 1 and 1000

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
    newBucket.createdBy = loggedUser;
    strcpy(newBucket.title, title);
    newBucket.createdDate = time(NULL); // Get current time as time_t

    // Set createdBy to a default user ID (e.g., 0 or 1) or a constant
    // You might want to define a constant like DEFAULT_USER_ID
    newBucket.createdBy = 0; // Or DEFAULT_USER_ID;
    newBucket.taskCount = 0;

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
    // Save the changes to the file
    saveBucketToFile();
}

void closeBucketModule() {
    saveBucketToFile(); //save data before closing
    free(bucketArray);
    bucketArray = NULL;
}

void saveBucketToFile() {
    FILE* dataFile;
    dataFile = fopen("Bucket.txt", "w"); // Open in write mode, overwrites existing

    if (dataFile == NULL) {
        printf("Failed to open 'Bucket.txt' for writing.\n");
        return;
    }

    for (int i = 0; i < bucketCount; i++) {
        fprintf(dataFile, "%d,%s,%ld,%d", bucketArray[i].id, bucketArray[i].title, bucketArray[i].createdDate, bucketArray[i].createdBy);
        for (int j = 0; j < bucketArray[i].taskCount; j++) {
            if (bucketArray[i].taskIds[j] != -1) {
                fprintf(dataFile, ",%d", bucketArray[i].taskIds[j]);
            }
        }
        fprintf(dataFile, "\n");
    }

    fclose(dataFile);
}
