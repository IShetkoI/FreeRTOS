#include "main.h"
#include "stm32f7xx_it.h"
#include "timer.h"
#include "dac.h"


/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler (void)
{
    while (1)
    {
    }
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler (void)
{
    while (1)
    {
    }
}

/**
  * @brief This function handles Memory management fault.
  */
void MemManage_Handler (void)
{
    while (1)
    {
    }
}

/**
  * @brief This function handles Pre-fetch fault, memory access fault.
  */
void BusFault_Handler (void)
{
    while (1)
    {
    }
}

/**
  * @brief This function handles Undefined instruction or illegal state.
  */
void UsageFault_Handler (void)
{
    while (1)
    {
    }
}

/**
  * @brief This function handles Debug monitor.
  */
void DebugMon_Handler (void)
{

}

/******************************************************************************/
/* STM32F7xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f7xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles DMA1 stream5 global interrupt.
  */
void DMA1_Stream5_IRQHandler (void)
{
    DMA_HandleTypeDef dmaDac = getDmaDAC ();
    HAL_DMA_IRQHandler (&dmaDac);
}

/**
  * @brief This function handles TIM1 update interrupt and TIM10 global interrupt.
  */
void TIM1_UP_TIM10_IRQHandler (void)
{
    TIM_HandleTypeDef systemTimer = getSystemTimer ();
    HAL_TIM_IRQHandler (&systemTimer);
}

/**
  * @brief This function handles EXTI line[15:10] interrupts.
  */
void EXTI15_10_IRQHandler (void)
{
    HAL_GPIO_EXTI_IRQHandler (BUTTON_Pin);
}

/**
  * @brief This function handles TIM6 global interrupt, DAC1 and DAC2 underrun error interrupts.
  */
void TIM6_DAC_IRQHandler (void)
{
    DAC_HandleTypeDef dac   = getDAC ();
    TIM_HandleTypeDef timer = getTimer ();
    HAL_DAC_IRQHandler (&dac);
    HAL_TIM_IRQHandler (&timer);
}

