#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "Task.h"
#include "Bucket.h"

// Global variables
Task tasks[MAX_TASKS];  // Fixed size array for tasks
int taskCount = 0;      // Counter for number of tasks


int isDigit(char c) {
    return c >= '0' && c <= '9';
}

int isValidDate(const char* date) {
    if (strlen(date) != 10 || date[4] != '-' || date[7] != '-') return 0;
    for (int i = 0; i < 10; i++) {
        if ((i == 4 || i == 7)) continue;
        if (!isDigit(date[i])) return 0;
    }
    return 1;
}

int generateUniqueTaskId() {
    int id, exists;
    do {
        exists = 0;
        id = rand() % 10000;
        for (int i = 0; i < taskCount; i++) {
            if (tasks[i].task_id == id) {
                exists = 1;
                break;
            }
        }
    } while (exists);
    return id;
}

void addTask() {
    if (taskCount >= MAX_TASKS) {
        printf("Task capacity reached. Cannot add more tasks.\n");
        return;
    }

    Task newTask;
    newTask.task_id = generateUniqueTaskId();
    newTask.bucketId = currentBucket->id;

    printf("Enter task title: ");
    scanf(" %[^\n]s", newTask.title);

    // Get task priority
    do {
        printf("Enter task priority (1-5): ");
        int ret = scanf_s("%d", &newTask.priority);
        getchar();  // Clear the newline character from the input buffer
        if (ret != 1 || newTask.priority < 1 || newTask.priority > 5) {
            printf("Invalid priority. Please enter a number between 1 and 5.\n");
        }
    } while (newTask.priority < 1 || newTask.priority > 5);

    // Get task status
    do {
        printf("Enter task status (0: Pending, 1: In Progress, 2: Completed): ");
        int ret = scanf_s("%d", &newTask.status);
        getchar();  // Clear the newline character from the input buffer
        if (ret != 1 || newTask.status < 0 || newTask.status > 2) {
            printf("Invalid status. Please enter 0, 1, or 2.\n");
        }
    } while (newTask.status < 0 || newTask.status > 2);

    // Get task description
    printf("Enter task description: ");
    fgets(newTask.description, sizeof(newTask.description), stdin);
    strtok(newTask.description, "\n");

    // Get task start and due date
    do {
        printf("Enter task start date (YYYY-MM-DD): ");
        fgets(newTask.startDate, sizeof(newTask.startDate), stdin);
        strtok(newTask.startDate, "\n");
    } while (!isValidDate(newTask.startDate));

    do {
        printf("Enter task due date (YYYY-MM-DD): ");
        fgets(newTask.dueDate, sizeof(newTask.dueDate), stdin);
        strtok(newTask.dueDate, "\n");
    } while (!isValidDate(newTask.dueDate));

    tasks[taskCount++] = newTask;  // Add the new task to the tasks array
    printf("Task added successfully! Task ID: %d\n", newTask.task_id);

    // Save tasks immediately after adding a new task
    saveTasksToFile("task.txt");
}

void editTask() {
    int task_id, found = 0;
    printf("Enter task ID to edit: ");
    int ret = scanf_s("%d", &task_id);
    getchar();  // Clear the newline character from the input buffer

    if (ret != 1) {
        printf("Invalid input.\n");
        return;
    }

    for (int i = 0; i < taskCount; i++) {
        if (tasks[i].task_id == task_id) {
            found = 1;
            printf("Editing Task: %s\n", tasks[i].title);
            int choice;
            do {
                printf("\n1. Edit Title\n2. Edit Priority\n3. Edit Status\n4. Edit Description\n5. Edit Start Date\n6. Edit Due Date\n7. Exit\nEnter choice: ");
                ret = scanf_s("%d", &choice);
                getchar();  // Clear the newline character from the input buffer
                if (ret != 1) {
                    printf("Invalid choice.\n");
                    continue;
                }
                switch (choice) {
                case 1:
                    printf("New Title: ");
                    fgets(tasks[i].title, sizeof(tasks[i].title), stdin);
                    strtok(tasks[i].title, "\n");
                    break;
                case 2:
                    do {
                        printf("New Priority (1-5): ");
                        ret = scanf_s("%d", &tasks[i].priority);
                        getchar();
                    } while (tasks[i].priority < 1 || tasks[i].priority > 5);
                    break;
                case 3:
                    do {
                        printf("New Status (0: Pending, 1: In Progress, 2: Completed): ");
                        ret = scanf_s("%d", &tasks[i].status);
                        getchar();
                    } while (tasks[i].status < 0 || tasks[i].status > 2);
                    break;
                case 4:
                    printf("New Description: ");
                    fgets(tasks[i].description, sizeof(tasks[i].description), stdin);
                    strtok(tasks[i].description, "\n");
                    break;
                case 5:
                    do {
                        printf("New Start Date (YYYY-MM-DD): ");
                        fgets(tasks[i].startDate, sizeof(tasks[i].startDate), stdin);
                        strtok(tasks[i].startDate, "\n");
                    } while (!isValidDate(tasks[i].startDate));
                    break;
                case 6:
                    do {
                        printf("New Due Date (YYYY-MM-DD): ");
                        fgets(tasks[i].dueDate, sizeof(tasks[i].dueDate), stdin);
                        strtok(tasks[i].dueDate, "\n");
                    } while (!isValidDate(tasks[i].dueDate));
                    break;
                case 7:
                    printf("Changes saved.\n");
                    saveTasksToFile("task.txt");  // Save immediately after editing
                    break;
                default:
                    printf("Invalid choice.\n");
                }
            } while (choice != 7);
            saveTasksToFile("task.txt");
            return;
        }
    }
    if (!found) printf("Task ID %d not found.\n", task_id);
}

void displayTasks() {
    if (taskCount == 0) {
        printf("No tasks available.\n");
        return;
    }

    printf("\n%-5s | %-40s | %-8s | %-12s | %-12s | %-12s\n",
        "ID", "Title", "Priority", "Status", "Start Date", "Due Date");
    printf("------------------------------------------------------------------------------------------------------\n");

    for (int i = 0; i < taskCount; i++) {
        if (tasks[i].bucketId != currentBucket->id) continue;

        // Ensure startDate and dueDate are not NULL or empty
        char* start = (tasks[i].startDate[0] != '\0') ? tasks[i].startDate : "N/A";
        char* due = (tasks[i].dueDate[0] != '\0') ? tasks[i].dueDate : "N/A";


        char* statusStr;
        switch (tasks[i].status) {
        case 0: statusStr = "Pending"; break;
        case 1: statusStr = "In Progress"; break;
        case 2: statusStr = "Completed"; break;
        default: statusStr = "Unknown"; break;
        }

        printf("%-5d | %-40s | %-8d | %-12s | %-12s | %-12s\n",
            tasks[i].task_id, tasks[i].title, tasks[i].priority,
            statusStr, start, due);
    }
}
void viewTask() {
    int task_id, found = 0;
    printf("Enter task ID to view: ");
    int ret = scanf_s("%d", &task_id, sizeof(task_id));
    getchar();  // Clear the newline character from the input buffer

    if (ret != 1) {
        printf("Invalid input.\n");
        return;
    }

    for (int i = 0; i < taskCount; i++) {
        if (tasks[i].task_id == task_id) {
            found = 1;
            // Display task details
            printf("\nTask ID: %d\n", tasks[i].task_id);
            printf("Title: %s\n", tasks[i].title);
            printf("Priority: %d\n", tasks[i].priority);
            printf("Status: %s\n", (tasks[i].status == 0 ? "Pending" : (tasks[i].status == 1 ? "In Progress" : "Completed")));
            printf("Start Date: %s\n", tasks[i].startDate);
            printf("Due Date: %s\n", tasks[i].dueDate);
            printf("Description: %s\n", tasks[i].description);
            break;
        }
    }

    if (!found) {
        printf("Task ID %d not found.\n", task_id);
    }
}

void deleteTask() {
    int task_id;

    printf("Enter task ID to delete: ");
    int ret = scanf_s("%d", &task_id);
    getchar();  // Clear the newline character from the input buffer

    if (ret != 1) {
        printf("Invalid input.\n");
        return;
    }

    deleteTaskById(task_id);
}

void deleteTaskById(int taskId) {
    int taskFound = 0;

    for (int i = 0; i < taskCount; i++) {
        if (tasks[i].task_id == taskId) {
            taskFound = 1;
            for (int j = i; j < taskCount - 1; j++) {
                tasks[j] = tasks[j + 1];
            }

            taskCount--;  // Reduce the task count
            printf("Task ID %d deleted successfully.\n", taskId);

            // Save tasks immediately after deletion
            saveTasksToFile("task.txt");
            return;
        }
    }

    if (!taskFound) {
        printf("Task ID %d not found.\n", taskId);
    }
}

// Function to save tasks to file
void saveTasksToFile(const char* filePath) {
    FILE* file = fopen(filePath, "w");
    if (!file) {
        printf("Error opening file for saving.\n");
        return;
    }

    for (int i = 0; i < taskCount; i++) {
        fprintf(file, "%d|%d|%s|%d|%d|%s|%s|%s\n",
            tasks[i].task_id,
            tasks[i].bucketId,
            tasks[i].title,
            tasks[i].priority,
            tasks[i].status,
            tasks[i].startDate,
            tasks[i].dueDate,
            tasks[i].description);
    }

    fclose(file);
    printf("Tasks successfully saved to %s!\n", filePath);
}

void loadTasksFromFile(const char* filePath) {
    FILE* file = fopen(filePath, "r");
    if (!file) {
        printf("No saved tasks found.\n");
        return;
    }

    char line[1024];  // Buffer for reading a line
    taskCount = 0;    // Reset task count before loading

    while (fgets(line, sizeof(line), file)) {
        if (taskCount >= MAX_TASKS) {
            printf("Warning: Task limit reached. Some tasks may not be loaded.\n");
            break;
        }

        int scanned = sscanf(line, "%d|%d|%255[^|]|%d|%d|%255[^|]|%255[^|]|%1023[^\n]",
            &tasks[taskCount].task_id,
            &tasks[taskCount].bucketId,
            tasks[taskCount].title,
            &tasks[taskCount].priority,
            &tasks[taskCount].status,
            tasks[taskCount].startDate,
            tasks[taskCount].dueDate,
            tasks[taskCount].description);

        if (scanned != 7) {
            printf("ERROR: Malformed line in file. Skipping...\n");
            continue;  // Skip invalid lines
        }

        taskCount++;  // Only increment if successfully read
    }

    fclose(file);
    printf("Loaded %d tasks from file.\n", taskCount);
}
void exitProgram() {
    printf("Exiting the program...\n");
    exit(0);
}

// Function to delete all tasks of a bucket
void deleteAllTasksInBucket() {
    if (currentBucket == NULL) return;
    for (int i = 0; i < taskCount; i++) {
        if (tasks[i].bucketId == currentBucket->id) {
            deleteTaskById(tasks[i].task_id);
        }
    }
    printf("All tasks in bucket with ID %d are deleted.\n", currentBucket->id);
}