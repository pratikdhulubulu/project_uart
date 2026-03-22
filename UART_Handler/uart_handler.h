#ifndef UART_HANDLER_H
#define UART_HANDLER_H

#include <stdint.h>

#define UART_TX_BUFFER_SIZE (128U)

void uartHandlerInit(void);
uint8_t uartWriteBytes();
uint8_t uartWriteTxBuffer(uint8_t data);
uint8_t uartIsTxBufferEmpty(void);

#endif