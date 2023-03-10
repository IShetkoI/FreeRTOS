/**
   ******************************************************************************
   * @file     gpio.c
   * @brief    This is the common part of the GPIO initialization
   ******************************************************************************
   */

#include "gpio.h"
#include "freertos.h"
#include "main.h"
#include "stdbool.h"


/**
   ******************************************************************************
   * @brief      GPIO initialization function
   * @ingroup    gpio
   ******************************************************************************
   */

void initializeGPIO (void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOE_CLK_ENABLE ();
    __HAL_RCC_GPIOC_CLK_ENABLE ();
    __HAL_RCC_GPIOA_CLK_ENABLE ();
    __HAL_RCC_GPIOB_CLK_ENABLE ();
    __HAL_RCC_GPIOD_CLK_ENABLE ();

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin (CRYSTAL_SELECT_GPIO_Port, CRYSTAL_SELECT_Pin, GPIO_PIN_SET);

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin (GPIOB, LED_RED_Pin | LED_BLUE_Pin, GPIO_PIN_RESET);

    /*Configure GPIO pin : PtPin */
    GPIO_InitStruct.Pin   = CRYSTAL_SELECT_Pin;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init (CRYSTAL_SELECT_GPIO_Port, &GPIO_InitStruct);

    /*Configure GPIO pin : PtPin */
    GPIO_InitStruct.Pin  = BUTTON_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init (BUTTON_GPIO_Port, &GPIO_InitStruct);

    /*Configure GPIO pins : PBPin PBPin */
    GPIO_InitStruct.Pin   = LED_RED_Pin | LED_BLUE_Pin;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init (GPIOB, &GPIO_InitStruct);

    /* EXTI interrupt init*/
    HAL_NVIC_SetPriority (EXTI15_10_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ (EXTI15_10_IRQn);

}


/**
   ******************************************************************************
   * @brief        GPIO interrupt handler
   * @ingroup      gpio
   * @param[in]    GPIO_Pin - pin number to be processed
   ******************************************************************************
   */

void HAL_GPIO_EXTI_Callback (uint16_t GPIO_Pin)
{
    if (GPIO_Pin == BUTTON_Pin)
    {
        osStatus_t osStatus;
        static bool     isActive              = true;                   ///< Whether to enable measurement from the BMP280 sensor
        osSemaphoreId_t semaphoreButtonHandle = getSemaphoreButtonHandle();

        if (isActive == true)
        {
            osStatus = osSemaphoreAcquire (semaphoreButtonHandle, 0);

            if (osStatus == osErrorParameter)
            {
                errorHandler();
            }

            isActive = false;
        }
        else
        {
            osStatus = osSemaphoreRelease (semaphoreButtonHandle);

            if (osStatus == osErrorParameter)
            {
                errorHandler();
            }

            isActive = true;
        }

        __HAL_GPIO_EXTI_CLEAR_IT (BUTTON_Pin);
        NVIC_ClearPendingIRQ (EXTI15_10_IRQn);
    }
}
