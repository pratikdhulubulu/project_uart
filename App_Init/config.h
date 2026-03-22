
#ifndef CONFIG_H
#define CONFIG_H

#include "stm32f446xx.h"
#include "gpio_driver.h"
#include "exti_driver.h"
#include "rcc_driver.h"

const RCC_SYS_CFG clk_cfg =
{
    .CLK_SOURCE = RCC_CLK_SRC_PLL,
    .FLASH_LATENCY = FLASH_ACR_LATENCY_0WS,
    .AHB_PRESCALER  = RCC_CFGR_HPRE_DIV1,
    .APB1_PRESCALER = RCC_CFGR_PPRE1_DIV1,
    .APB2_PRESCALER = RCC_CFGR_PPRE2_DIV1,

    .PLL =
    {
        .SRC = RCC_CLK_SRC_HSE,
        .M   = 8U,
        .N   = 64U,
        .P   = 4U,
        .Q   = 4U
    }
};

/**
 * @brief GPIO configuration for LED on PA5.
 */
const GPIO_CFG led_cfg =
{
    .ptr_port  = GPIOA,
    .pin       = PIN_5,
    .mode      = GPIO_MODE_OUTPUT,
    .otype     = GPIO_OTYPE_PP,
    .speed     = GPIO_SPEED_HIGH,
    .pupd      = GPIO_PUPD_NONE,
    .alt_func  = 0U
};

/**
 * @brief GPIO configuration for Button on PC13.
 */
const GPIO_CFG button_cfg =
{
    .ptr_port  = GPIOC,
    .pin       = PIN_13,
    .mode      = GPIO_MODE_INPUT,
    .otype     = GPIO_OTYPE_PP,
    .speed     = GPIO_SPEED_HIGH,
    .pupd      = GPIO_PUPD_UP,
    .alt_func  = 0U
};

/**
 * @brief EXTI Cinfiguration for Button on PA13
 */
const EXTI_CONFIG exti_cfg =
{
    .ptr_port   = GPIOC,
    .line       = EXTI_LINE_13,
    .trigger    = EXTI_TRIGGER_FALLING,
    .priority   = 2
};


const GPIO_CFG uart2_tx_cfg =
{
    .ptr_port  = GPIOA,
    .pin       = PIN_2,
    .mode      = GPIO_MODE_ALT,
    .otype     = GPIO_OTYPE_PP,
    .speed     = GPIO_SPEED_HIGH,
    .pupd      = GPIO_PUPD_UP,
    .alt_func  = 7
};

const GPIO_CFG uart2_rx_cfg =
{
    .ptr_port  = GPIOA,
    .pin       = PIN_3,
    .mode      = GPIO_MODE_ALT,
    .otype     = GPIO_OTYPE_PP,
    .speed     = GPIO_SPEED_HIGH,
    .pupd      = GPIO_PUPD_UP,
    .alt_func  = 7
};
#endif
