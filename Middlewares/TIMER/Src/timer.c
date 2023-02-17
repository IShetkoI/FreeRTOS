
#include "timer.h"

static uint16_t interpolation (uint16_t x1, uint16_t x2, uint16_t x3, uint16_t y1, uint16_t y3);

TIM_HandleTypeDef timer;

/* TIM6 init function */
void MX_TIM6_Init(void)
{


  TIM_MasterConfigTypeDef sMasterConfig = {0};

  timer.Instance = TIM6;
  timer.Init.Prescaler = 0;
  timer.Init.CounterMode = TIM_COUNTERMODE_UP;
  timer.Init.Period = 215;
  timer.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&timer) != HAL_OK)
  {
    errorHandler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&timer, &sMasterConfig) != HAL_OK)
  {
    errorHandler();
  }

}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* tim_baseHandle)
{

  if(tim_baseHandle->Instance==TIM6)
  {
    /* TIM6 clock enable */
    __HAL_RCC_TIM6_CLK_ENABLE();

    /* TIM6 interrupt Init */
    HAL_NVIC_SetPriority(TIM6_DAC_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(TIM6_DAC_IRQn);
  }
}

void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef* tim_baseHandle)
{

  if(tim_baseHandle->Instance==TIM6)
  {

    __HAL_RCC_TIM6_CLK_DISABLE();

  }
}

static uint16_t interpolation (uint16_t x1, uint16_t x2, uint16_t x3, uint16_t y1, uint16_t y3)
{
    uint16_t result = y1 + (x2 - x1) * (y3 - y1) / (x3 - x1);
    return result;
}

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

void startTimerBase(void)
{
	HAL_TIM_Base_Start (&timer);
}

TIM_HandleTypeDef getTimer(void)
{
	return timer;
}
