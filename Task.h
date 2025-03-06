#pragma once

#include <time.h>
#include "User.h"
#include "Task.h"

// Define Task structure
typedef struct Task {
	char title[256];
	int priority;
	int status;
	time_t startDate;
	time_t dueDate;
	char description[1024];
	User assignedUsers[10];
};

// Functions operating in Task Module
void displayTaskDetails(int taskId);			// Function to display a specific task in the terminal
Task createNewTask();							// Function that receives input from user to create a new task
void editTask(int taskId, Task editedValue);	// Function to edit an existed task
