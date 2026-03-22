#include "uart_handler.h"
#include "uart_driver.h"

static uint8_t  uart_tx_buffer[UART_TX_BUFFER_SIZE];
static uint16_t uart_tx_head = 0U;
static uint16_t uart_tx_tail = 0U;

void uartHandlerInit(void)
{
    uint16_t index;

    for (index = 0U; index < UART_TX_BUFFER_SIZE; index++)
    {
        uart_tx_buffer[index] = 0U;
    }

    uart_tx_head = 0U;
    uart_tx_tail = 0U;
}

uint8_t uartWriteTxBuffer(uint8_t data)
{
    uint16_t next_head;

    next_head = (uart_tx_head + 1U) % UART_TX_BUFFER_SIZE;

    if (next_head == uart_tx_tail)
    {
        return 0U;
    }

    uart_tx_buffer[uart_tx_head] = data;
    uart_tx_head = next_head;

    return 1U;
}

uint8_t uartWriteBytes()
{
    uint8_t status = 1U;

    while (!uartIsTxBufferEmpty())
    {
        uartPutChar(uart_tx_buffer[uart_tx_tail]);

        uart_tx_tail = (uart_tx_tail + 1U) % UART_TX_BUFFER_SIZE;
    }

    return status;
}

uint8_t uartIsTxBufferEmpty(void)
{
    if (uart_tx_head == uart_tx_tail)
    {
        return 1U;
    }

    return 0U;
}