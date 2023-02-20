#include "main.h"
#include "adc.h"
#include "clock.h"
#include "dac.h"
#include "dma.h"
#include "freertos.h"
#include "gpio.h"
#include "spi.h"
#include "timer.h"
#include "usart.h"


/**
   ******************************************************************************
   * @brief     The application entry point.
   * @return    int
   ******************************************************************************
   */

int main (void)
{
    HAL_StatusTypeDef halStatus;
    osStatus_t        osStatus;

    /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
    halStatus = initializeHAL ();

    if (halStatus != HAL_OK)
        errorHandler ();


    /* Configure the system clock */
    initializeClock ();

    if (halStatus != HAL_OK)
        errorHandler ();


    /* Initialize all configured peripherals */

    initializeDMA ();

    if (halStatus != HAL_OK)
        errorHandler ();

    initializeADC ();

    if (halStatus != HAL_OK)
        errorHandler ();

    initializeDAC ();

    if (halStatus != HAL_OK)
        errorHandler ();

    initializeTimer6 ();

    if (halStatus != HAL_OK)
        errorHandler ();

    initializeSpi ();

    if (halStatus != HAL_OK)
        errorHandler ();

    initializeUsart ();

    if (halStatus != HAL_OK)
        errorHandler ();

    initializeGPIO ();

    if (halStatus != HAL_OK)
        errorHandler ();

    startTimerBase ();

    if (halStatus != HAL_OK)
        errorHandler ();


    /* Init scheduler */
    osStatus = osKernelInitialize ();

    if (osStatus != osOK)
        errorHandler ();

    osStatus = initializeFreeRTOS ();

    if (osStatus != osOK)
        errorHandler ();


    /* Start scheduler */
    osStatus = osKernelStart ();

    if (osStatus != osOK)
        errorHandler ();

    while (1)
    {
    }
}


/**
   ******************************************************************************
   * @brief     This function is executed in case of error occurrence.
   * @return    None
   ******************************************************************************
   */

void errorHandler (void)
{
    __disable_irq ();

    while (1)
    {
    }
}


