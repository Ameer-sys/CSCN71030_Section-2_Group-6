#pragma once

#include "Task.h"

// Define Bucket structure
typedef struct bucket {
	int id;
	char title[256];
	Task taskList[100];
} Bucket;

// Funtions in Bucket Module
void displayTaskList(Bucket bucket);
void addTask();
void editTask(int taskId);
void deleteTask(int taskId);
