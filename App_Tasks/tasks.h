/**
 * @file    tasks.h
 * @brief   Application Tasks declarations for FreeRTOS
 * @details Contains prototypes for Task One and Task Two.
 */

#ifndef TASKS_H
#define TASKS_H

#include "FreeRTOS.h"
#include "task.h"

#define TASK1_STACK_SIZE   128
#define TASK2_STACK_SIZE   128
#define TASK1_PRIORITY     1
#define TASK2_PRIORITY     1


void osInit(void);
void Task1(void *pvParameters);
void Task2(void *pvParameters);

#endif
