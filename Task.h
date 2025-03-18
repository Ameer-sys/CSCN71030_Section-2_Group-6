#pragma once
#include <time.h>
#include "User.h"

// Maximum number of users that can be assigned
#define MAX_USERS 10
#define MAX_TASKS 100

// Define Task structure
typedef struct task {
    int task_id;
    char title[256];
    int priority;
    int status;
    char startDate[11];
    char dueDate[11];
    char description[1024];
    User assignedUsers[MAX_USERS];
    int userCount;                  // Count of users assigned to this task
    int bucketId;                   // Additional field for grouping tasks, if needed
} Task;

// Declare tasks array with fixed size
extern Task tasks[MAX_TASKS];
extern int taskCount;


// Function declarations
int loadUsersFromFile(User users[]);  // Loads users from a file
int generateUniqueTaskId();           // Generates a unique task ID
void addTask();                       // Adds a new task
void editTask();                      // Edits an existing task
void deleteTask();                    // Deletes a task
void saveTasksToFile(const char* filePath);  // Saves tasks to a file
void loadTasksFromFile(const char* filePath);
void displayTasks();                  // Displays the list of tasks
void viewTask();
void exitProgram();                   // Exits the program and saves tasks 
