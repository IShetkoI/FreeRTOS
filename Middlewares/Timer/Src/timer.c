/**
  ******************************************************************************
  * @file    timer.c
  * @brief   This file provides code for the configuration
  *          of the TIM instances.
  ******************************************************************************
  */

#include "timer.h"

#define INITIAL_PERIOD_VALUE    215 ///< Initial value of the timer period
#define INITIAL_PRESCALER_VALUE 0   ///< Initial value of the timer prescaler

static TIM_HandleTypeDef timer;     ///< Pointer Timer Handler

static uint16_t interpolation (uint16_t x1, uint16_t x2, uint16_t x3, uint16_t y1, uint16_t y3);


/**
   ******************************************************************************
   * @brief    Timer initialization function
   * @ingroup  timer
   ******************************************************************************
   */

void initializeTimer6 (void)
{
    TIM_MasterConfigTypeDef sMasterConfig = {0};

    timer.Instance               = TIM6;
    timer.Init.Prescaler         = INITIAL_PRESCALER_VALUE;
    timer.Init.CounterMode       = TIM_COUNTERMODE_UP;
    timer.Init.Period            = INITIAL_PERIOD_VALUE;
    timer.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    ASSERT (HAL_TIM_Base_Init (&timer) != HAL_OK);

    sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
    sMasterConfig.MasterSlaveMode     = TIM_MASTERSLAVEMODE_DISABLE;
    ASSERT (HAL_TIMEx_MasterConfigSynchronization (&timer, &sMasterConfig) != HAL_OK);
}


/**
   ******************************************************************************
   * @brief      This function configures the hardware resources used in this
   *             example
   * @ingroup    timer
   * @param[in]  tim_baseHandle - Timer handle pointer
   ******************************************************************************
   */

void HAL_TIM_Base_MspInit (TIM_HandleTypeDef *tim_baseHandle)
{
    if (tim_baseHandle->Instance == TIM6)
    {
        /* TIM6 clock enable */
        __HAL_RCC_TIM6_CLK_ENABLE ();

        /* TIM6 interrupt Init */
        HAL_NVIC_SetPriority (TIM6_DAC_IRQn, 5, 0);
        HAL_NVIC_EnableIRQ (TIM6_DAC_IRQn);
    }
}


/**
   ******************************************************************************
   * @brief      This function freeze the hardware resources used in this example
   * @ingroup    timer
   * @param[in]  tim_baseHandle - Timer handle pointer
   ******************************************************************************
   */

void HAL_TIM_Base_MspDeInit (TIM_HandleTypeDef *tim_baseHandle)
{
    if (tim_baseHandle->Instance == TIM6)
    {
        __HAL_RCC_TIM6_CLK_DISABLE ();
    }
}


/**
   ******************************************************************************
   * @brief      Interpolation calculation function
   * @ingroup    adc
   * @param[in]  x1 - Contiguous value for y1
   * @param[in]  x2 - Contiguous value for y2
   * @param[in]  x3 - Contiguous value for y3
   * @param[in]  y1 - First value of the range
   * @param[in]  y3 - Second value of the range
   * @return     Intermediate value between y1 and y2
   *
   *  \f[
   *  y_2 = y_1 + (x_2 - x_1) * (y_3 - y_1) / (x_3 - x_1);
   *  \f]
   ******************************************************************************
   */

static uint16_t interpolation (uint16_t x1, uint16_t x2, uint16_t x3, uint16_t y1, uint16_t y3)
{
    uint16_t result = y1 + (x2 - x1) * (y3 - y1) / (x3 - x1);
    return result;
}


/**
   ******************************************************************************
   * @brief      Function for setting a new timer configuration
   * @ingroup    adc
   ******************************************************************************
   */

void setTimerConfig (uint16_t adcValue)
{
    uint16_t adcs[11]      = {0, 408, 817, 1227, 1637, 2046, 2456, 2866, 3275, 3685, 4095}; ///< ADC values divided into 10 ranges (Auxiliary/calc_hz.py)
    uint16_t periods[11]   = {249, 1694, 847, 564, 423, 338, 282, 242, 211, 188, 169};      ///< Period values for the timer corresponding to 10 ranges
    uint16_t prescalers[2] = {0, 249};                                                      ///< Prescaler values for the timer

    uint16_t period;
    uint16_t prescaler;

    for (int i = 0; i < 11; i++)
    {
        /* At values from 1 to 250 Hz */
        if (adcValue <= adcs[1])
        {
            period    = periods[1];
            prescaler = interpolation (adcs[1], adcValue, adcs[0], prescalers[0], prescalers[1]);
            break;
        }
            /* At values from 251 to 2499 Hz */
        else if (adcValue < adcs[i])
        {
            period    = interpolation (adcs[i], adcValue, adcs[i - 1], periods[i], periods[i - 1]);
            prescaler = prescalers[0];
            break;
        }
            /* At 2500 Hz */
        else if (adcValue == adcs[10])
        {
            period    = periods[10];
            prescaler = prescalers[0];
            break;
        }
    }

    /* Applying the counted period and the prescaler to the timer */
    __HAL_TIM_SET_AUTORELOAD (&timer, period);
    __HAL_TIM_SET_PRESCALER (&timer, prescaler);
}


/**
   ******************************************************************************
   * @brief      Timer start function
   * @ingroup    timer
   ******************************************************************************
   */

void startTimerBase (void)
{
    HAL_TIM_Base_Start (&timer);
}


/**
   ******************************************************************************
   * @brief      Function for passing a pointer to a timer
   * @ingroup    timer
   * @return     Pointer Timer Handler
   ******************************************************************************
   */

TIM_HandleTypeDef getTimer (void)
{
    return timer;
}

/**
   ******************************************************************************
   * @brief      Period elapsed callback in non blocking mode
   * @note       This function is called  when TIM1 interrupt took place, inside
   *             HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
   *             a global variable "uwTick" used as application time base.
   * @param[in]  htim - TIM handle
   * @retval     None
   ******************************************************************************
   */

void HAL_TIM_PeriodElapsedCallback (TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM1)
    {
        HAL_IncTick ();
    }
}