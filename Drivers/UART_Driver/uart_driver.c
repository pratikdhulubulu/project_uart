#include "uart_driver.h"
#include "rcc_driver.h"

void uartInit()
{
    rccEnableAPB1((uint32_t)RCC_APB1RSTR_USART2RST);

    USART2->CR1 = 0U;
    USART2->CR2 = 0U;
    USART2->CR3 = 0U;

    USART2->CR1 |= 0 << 15U;
    USART2->BRR = 0x45;
    USART2->CR1 |= 0 << 12U;
    USART2->CR1 |= 0 << 10U;
    USART2->CR1 |= 0 << 9U;
    USART2->CR1 |= 1 << 2U;
    USART2->CR1 |= 1 << 3U;
    USART2->CR1 |= 1 << 13U;
}

void uartPutChar(char character)
{
    if(USART2->SR & USART_SR_TXE)
    {
        USART2->DR = character;
    }
}

uint8_t uartGetChar(char *character)
{
    if(USART2->SR & USART_SR_RXNE){
        *character = (char)USART2->DR;
        return 1;
    }
    return 0;
}