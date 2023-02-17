#include "adc.h"            ///< The size of the temporary array for filtering

static uint16_t filter (uint16_t newValue);

ADC_HandleTypeDef adc;

/* ADC1 init function */
void MX_ADC1_Init(void)
{

  ADC_ChannelConfTypeDef sConfig = {0};

  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
  */
  adc.Instance = ADC1;
  adc.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
  adc.Init.Resolution = ADC_RESOLUTION_12B;
  adc.Init.ScanConvMode = ADC_SCAN_DISABLE;
  adc.Init.ContinuousConvMode = DISABLE;
  adc.Init.DiscontinuousConvMode = DISABLE;
  adc.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  adc.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  adc.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  adc.Init.NbrOfConversion = 1;
  adc.Init.DMAContinuousRequests = DISABLE;
  adc.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  if (HAL_ADC_Init(&adc) != HAL_OK)
  {
    errorHandler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_3;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
  if (HAL_ADC_ConfigChannel(&adc, &sConfig) != HAL_OK)
  {
    errorHandler();
  }
}

void HAL_ADC_MspInit(ADC_HandleTypeDef* adcHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(adcHandle->Instance==ADC1)
  {

    /* ADC1 clock enable */
    __HAL_RCC_ADC1_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**ADC1 GPIO Configuration
    PA3     ------> ADC1_IN3
    */
    GPIO_InitStruct.Pin = GPIO_PIN_3;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  }
}

void HAL_ADC_MspDeInit(ADC_HandleTypeDef* adcHandle)
{

  if(adcHandle->Instance==ADC1)
  {
    /* Peripheral clock disable */
    __HAL_RCC_ADC1_CLK_DISABLE();

    /**ADC1 GPIO Configuration
    PA3     ------> ADC1_IN3
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_3);
  }
}


HAL_StatusTypeDef startADC (void)
{
	return HAL_ADC_Start(&adc);
}

static uint16_t filter (uint16_t newValue)
{
    static uint8_t  index   = 0;                   ///< Contains the position in the array of the last value added
    static uint16_t values[NUMBERS_IN_FILTER];     ///< Array for averaging
    static uint16_t average = 0;                   ///< Stores the sum of values for averaging

    if (++index >= NUMBERS_IN_FILTER)
        index = 0;

    average -= values[index];
    average += newValue;
    values[index] = newValue;

    return (average / NUMBERS_IN_FILTER);
}

uint16_t getAdcValue(void)
{

	uint16_t currentValue = HAL_ADC_GetValue(&adc);
	currentValue = filter(currentValue);
	HAL_ADC_Stop(&adc);
	HAL_ADC_Start(&adc);
	HAL_ADC_PollForConversion(&adc, 100);

	return currentValue;
}

ADC_HandleTypeDef getADC(void)
{
	return adc;
}
