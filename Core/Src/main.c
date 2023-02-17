
#include "main.h"
#include "cmsis_os.h"
#include "adc.h"
#include "dac.h"
#include "dma.h"
#include "spi.h"
#include "timer.h"
#include "usart.h"
#include "gpio.h"


#include "bmp280.h"
#include "spi.h"

void MX_FREERTOS_Init(void);

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();


  /* Configure the system clock */
  initializeClock();

  /* Initialize all configured peripherals */

  MX_DMA_Init();
  MX_ADC1_Init();
  MX_DAC_Init();
  MX_TIM6_Init();

  initializeSpi();
  initializeUsart();
  MX_GPIO_Init();

  startTimerBase();




  /* Init scheduler */
  osKernelInitialize();  /* Call init function for freertos objects (in freertos.c) */
  MX_FREERTOS_Init();

  /* Start scheduler */
  osKernelStart();

  while (1)
  {

  }
}



/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void errorHandler(void)
{
  __disable_irq();
  while (1)
  {
  }
}


