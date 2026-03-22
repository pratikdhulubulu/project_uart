/**
 * @file    main.c
 * @brief   Main entry for Application.
 * @details Initializes MCU, configures peripherals, and starts FreeRTOS tasks.
 */

#include "tasks.h"
#include "FreeRTOS.h"
#include "task.h"
#include "stm32f446xx.h"
#include "config.h"
#include "uart_driver.h"
#include "uart_handler.h"
#include "rcc_driver.h"

static void buttonPushIsr(void);

int main(void)
{
    /* Initialize System Clock */
    rccSystemClockConfig(&clk_cfg);
    
    /* Initialize GPIO for LED on PA5 */
    gpioInit((GPIO_CFG *)&led_cfg);

    /* Initialize GPIO for Tx and Rx */
    gpioInit((GPIO_CFG *)&uart2_tx_cfg);
    gpioInit((GPIO_CFG *)&uart2_rx_cfg);

    uartInit();
    uartHandlerInit();

    /* Create tasks and start os scheduler */
    osInit();

    /* Infinite loop - control should never reach here */
    while (1)
    {
    }
}