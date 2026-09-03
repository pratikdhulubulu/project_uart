/**
 * @file    tasks.c
 * @brief   Application Tasks implementation for FreeRTOS
 * @details Implements os init and tasks.
 */

#include "tasks.h"
#include "stm32f446xx.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include "gpio_driver.h"
#include "uart_handler.h"

static unsigned char character = 0x0;

void osInit(void)
{
    /* Create RTOS tasks*/
    xTaskCreate(Task1, "Task1", TASK1_STACK_SIZE, NULL, TASK1_PRIORITY, NULL);
    xTaskCreate(Task2, "Task2", TASK2_STACK_SIZE, NULL, TASK2_PRIORITY, NULL);

    /* Start scheduler */
    vTaskStartScheduler();
}

void Task1(void *pvParameters)
{
    (void)pvParameters;

    while (1)
    {
        uartWriteTxBuffer(character);
        character++;
        vTaskDelay(pdMS_TO_TICKS(200));
    }
}

void Task2(void *pvParameters)
{
    (void)pvParameters;

    while (1)
    {
        uartWriteBytes();
        vTaskDelay(pdMS_TO_TICKS(200));
    }
}
