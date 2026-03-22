#ifndef UART_DRIVER_H
#define UART_DRIVER_H

#include <stddef.h>
#include <stdint.h>
#include "stm32f446xx.h"
#include "gpio_driver.h"

void uartInit();
void uartPutChar(char character);
uint8_t uartGetChar(char *character);

#endif