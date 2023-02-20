#include "stm32f7xx_hal.h"
#include "stm32f7xx_hal_tim.h"


TIM_HandleTypeDef systemTimer;


/**
   ******************************************************************************
   * @brief        This function configures the TIM1 as a time base source.
   *               The time source is configured  to have 1ms time base with a
   *               dedicated Tick interrupt priority.
   * @note         This function is called  automatically at the beginning of
   *               program after reset by HAL_Init() or at any time when clock
   *               is configured, by HAL_RCC_ClockConfig().
   * @param[in]    TickPriority - Tick interrupt priority.
   * @return       HAL status
   ******************************************************************************
   */

HAL_StatusTypeDef HAL_InitTick (uint32_t TickPriority)
{
    RCC_ClkInitTypeDef clkconfig;
    uint32_t           uwTimclock = 0U;

    uint32_t          uwPrescalerValue = 0U;
    uint32_t          pFLatency;
    HAL_StatusTypeDef status;

    /* Enable TIM1 clock */
    __HAL_RCC_TIM1_CLK_ENABLE ();

    /* Get clock configuration */
    HAL_RCC_GetClockConfig (&clkconfig, &pFLatency);

    /* Compute TIM1 clock */
    uwTimclock = 2 * HAL_RCC_GetPCLK2Freq ();

    /* Compute the prescaler value to have TIM1 counter clock equal to 1MHz */
    uwPrescalerValue = (uint32_t) ((uwTimclock / 1000000U) - 1U);

    /* Initialize TIM1 */
    systemTimer.Instance = TIM1;

    /* Initialize TIMx peripheral as follow:
    + Period = [(TIM1CLK/1000) - 1]. to have a (1/1000) s time base.
    + Prescaler = (uwTimclock/1000000 - 1) to have a 1MHz counter clock.
    + ClockDivision = 0
    + Counter direction = Up
    */
    systemTimer.Init.Period            = (1000000U / 1000U) - 1U;
    systemTimer.Init.Prescaler         = uwPrescalerValue;
    systemTimer.Init.ClockDivision     = 0;
    systemTimer.Init.CounterMode       = TIM_COUNTERMODE_UP;
    systemTimer.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;

    status = HAL_TIM_Base_Init (&systemTimer);
    if (status == HAL_OK)
    {
        /* Start the TIM time Base generation in interrupt mode */
        status = HAL_TIM_Base_Start_IT (&systemTimer);
        if (status == HAL_OK)
        {
            /* Enable the TIM1 global Interrupt */
            HAL_NVIC_EnableIRQ (TIM1_UP_TIM10_IRQn);
            /* Configure the SysTick IRQ priority */
            if (TickPriority < (1UL << __NVIC_PRIO_BITS))
            {
                /* Configure the TIM IRQ priority */
                HAL_NVIC_SetPriority (TIM1_UP_TIM10_IRQn, TickPriority, 0U);
                uwTickPrio = TickPriority;
            }
            else
            {
                status = HAL_ERROR;
            }
        }
    }

    /* Return function status */
    return status;
}


/**
   ******************************************************************************
   * @brief    Suspend Tick increment.
   * @note     Disable the tick increment by disabling TIM1 update interrupt.
   ******************************************************************************
  */

void HAL_SuspendTick (void)
{
    /* Disable TIM1 update Interrupt */
    __HAL_TIM_DISABLE_IT (&systemTimer, TIM_IT_UPDATE);
}


/**
   ******************************************************************************
   * @brief    Resume Tick increment.
   * @note     Enable the tick increment by Enabling TIM1 update interrupt.
   ******************************************************************************
  */

void HAL_ResumeTick (void)
{
    /* Enable TIM1 Update interrupt */
    __HAL_TIM_ENABLE_IT (&systemTimer, TIM_IT_UPDATE);
}


/**
   ******************************************************************************
   * @brief      Getting a handler to system timer
   * @return     Handler to system timer
   ******************************************************************************
  */

TIM_HandleTypeDef getSystemTimer (void)
{
    return systemTimer;
}
