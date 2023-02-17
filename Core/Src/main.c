#include "main.h"
#include "cmsis_os.h"
#include "adc.h"
#include "clock.h"
#include "dac.h"
#include "dma.h"
#include "gpio.h"
#include "spi.h"
#include "timer.h"
#include "usart.h"


/**
  * @brief  The application entry point.
  * @retval int
  */
int main (void)
{

    /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
    initializeHAL ();

    /* Configure the system clock */
    initializeClock ();

    /* Initialize all configured peripherals */

    initializeDMA ();
    initializeADC ();
    initializeDAC ();
    initializeTimer6 ();

    initializeSpi ();
    initializeUsart ();
    initializeGPIO ();

    startTimerBase ();

    /* Init scheduler */
    osKernelInitialize ();
    initializeFreeRTOS ();

    /* Start scheduler */
    osKernelStart ();

    while (1)
    {
    }
}


/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void errorHandler (void)
{
    __disable_irq ();
    while (1)
    {
    }
}


