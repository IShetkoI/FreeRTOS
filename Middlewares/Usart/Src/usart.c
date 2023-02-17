/**
   ******************************************************************************
   * @file    usart.c
   * @brief   This is the common part of the USART initialization
   ******************************************************************************
   */

#include "usart.h"

#define BAUD_RATE 115200         ///< Information transfer rate
#define UART_SIZE 1              ///< Package size

static UART_HandleTypeDef usart; ///< Pointer to configuration structure


HAL_StatusTypeDef initializeUsart (void)
{
    usart.Instance                    = USART3;
    usart.Init.BaudRate               = BAUD_RATE;
    usart.Init.WordLength             = UART_WORDLENGTH_8B;
    usart.Init.StopBits               = UART_STOPBITS_1;
    usart.Init.Parity                 = UART_PARITY_NONE;
    usart.Init.Mode                   = UART_MODE_TX_RX;
    usart.Init.HwFlowCtl              = UART_HWCONTROL_NONE;
    usart.Init.OverSampling           = UART_OVERSAMPLING_16;
    usart.Init.OneBitSampling         = UART_ONE_BIT_SAMPLE_DISABLE;
    usart.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;

    return HAL_UART_Init (&usart);
}


/**
   ******************************************************************************
   * @brief  UART MSP Initialization
   *         This function configures the hardware resources used in this example
   * @param  huart - UART handle pointer
   ******************************************************************************
   */

void HAL_UART_MspInit (UART_HandleTypeDef *huart)
{
    GPIO_InitTypeDef         GPIO_InitStruct     = {0};
    RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

    if (huart->Instance == USART3)
    {
        /* Initializes the peripherals clock */
        PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USART3;
        PeriphClkInitStruct.Usart3ClockSelection = RCC_USART3CLKSOURCE_PCLK1;

        ASSERT (HAL_RCCEx_PeriphCLKConfig (&PeriphClkInitStruct) != HAL_OK);

        /* Peripheral clock enable */
        __HAL_RCC_USART3_CLK_ENABLE ();

        __HAL_RCC_GPIOD_CLK_ENABLE ();
        /**USART3 GPIO Configuration
        PD8     ------> USART3_TX
        PD9     ------> USART3_RX
        */
        GPIO_InitStruct.Pin       = GPIO_PIN_8 | GPIO_PIN_9;
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull      = GPIO_NOPULL;
        GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF7_USART3;

        HAL_GPIO_Init (GPIOD, &GPIO_InitStruct);
    }

    return HAL_OK;
}


/**
   ******************************************************************************
   * @brief  UART MSP De-Initialization
   *         This function freeze the hardware resources used in this example
   * @param  huart - UART handle pointer
   ******************************************************************************
   */

void HAL_UART_MspDeInit (UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART3)
    {
        /* Peripheral clock disable */
        __HAL_RCC_USART3_CLK_DISABLE ();

        /**USART3 GPIO Configuration
        PD8     ------> USART3_TX
        PD9     ------> USART3_RX
        */
        HAL_GPIO_DeInit (GPIOD, GPIO_PIN_8 | GPIO_PIN_9);
    }
}


/**
   ******************************************************************************
   * @brief    Sending symbols over the COM port
   * @param    symbol - Symbol to send
   * @returns  Sending symbols over the COM port
   ******************************************************************************
  */

int __io_putchar (int symbol)
{
    /* The symbol that should have been sent */
    uint8_t temp = symbol;

    /* Function to send a symbol via USART */
    HAL_UART_Transmit (&usart, &temp, UART_SIZE, HAL_MAX_DELAY);

    return symbol;
}

