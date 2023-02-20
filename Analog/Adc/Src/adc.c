/**
   ******************************************************************************
   * @file     adc.c
   * @brief    This file provides code for the configuration
   *           of the ADC instances.
   ******************************************************************************
   */

#include "adc.h"

#define NUMBERS_IN_FILTER           8      ///< The size of the temporary array for filtering
#define POLL_FOR_CONVERSION_TIMEOUT 100    ///< Poll for conversion timeout

static ADC_HandleTypeDef adc;              ///< Pointer ADC Handler

static uint16_t filter (uint16_t newValue);


/**
   ******************************************************************************
   * @brief      ADC initialization function
   * @ingroup    adc
   * @return     Status of the initialization
   ******************************************************************************
   */

HAL_StatusTypeDef initializeADC (void)
{
    ADC_ChannelConfTypeDef sConfig = {0};

    /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
    */
    adc.Instance                   = ADC1;
    adc.Init.ClockPrescaler        = ADC_CLOCK_SYNC_PCLK_DIV4;
    adc.Init.Resolution            = ADC_RESOLUTION_12B;
    adc.Init.ScanConvMode          = ADC_SCAN_DISABLE;
    adc.Init.ContinuousConvMode    = DISABLE;
    adc.Init.DiscontinuousConvMode = DISABLE;
    adc.Init.ExternalTrigConvEdge  = ADC_EXTERNALTRIGCONVEDGE_NONE;
    adc.Init.ExternalTrigConv      = ADC_SOFTWARE_START;
    adc.Init.DataAlign             = ADC_DATAALIGN_RIGHT;
    adc.Init.NbrOfConversion       = 1;
    adc.Init.DMAContinuousRequests = DISABLE;
    adc.Init.EOCSelection          = ADC_EOC_SINGLE_CONV;

    if (HAL_ADC_Init (&adc) != HAL_OK)
    {
        return HAL_ERROR;
    }

    /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
    */
    sConfig.Channel      = ADC_CHANNEL_3;
    sConfig.Rank         = ADC_REGULAR_RANK_1;
    sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;

    if (HAL_ADC_ConfigChannel (&adc, &sConfig) != HAL_OK)
    {
        return HAL_ERROR;
    }

    return HAL_OK;
}


/**
   ******************************************************************************
   * @brief        This function configures the hardware resources used in this
   *               example
   * @ingroup      adc
   * @param[in]    adcHandle - ADC handle pointer
   ******************************************************************************
   */

void HAL_ADC_MspInit (ADC_HandleTypeDef *adcHandle)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    if (adcHandle->Instance == ADC1)
    {
        /* ADC1 clock enable */
        __HAL_RCC_ADC1_CLK_ENABLE ();

        __HAL_RCC_GPIOA_CLK_ENABLE ();
        /**ADC1 GPIO Configuration
        PA3     ------> ADC1_IN3
        */
        GPIO_InitStruct.Pin  = GPIO_PIN_3;
        GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        HAL_GPIO_Init (GPIOA, &GPIO_InitStruct);
    }
}


/**
   ******************************************************************************
   * @brief        This function freeze the hardware resources used in this
   *               example
   * @ingroup      adc
   * @param[in]    adcHandle - ADC handle pointer
   ******************************************************************************
   */

void HAL_ADC_MspDeInit (ADC_HandleTypeDef *adcHandle)
{
    if (adcHandle->Instance == ADC1)
    {
        /* Peripheral clock disable */
        __HAL_RCC_ADC1_CLK_DISABLE ();

        /**ADC1 GPIO Configuration
        PA3     ------> ADC1_IN3
        */
        HAL_GPIO_DeInit (GPIOA, GPIO_PIN_3);
    }
}


/**
   ******************************************************************************
   * @brief      ADC DMA startup function
   * @ingroup    adc
   * @return     ADC start status
   ******************************************************************************
   */

HAL_StatusTypeDef startADC (void)
{
    return HAL_ADC_Start (&adc);
}


/**
   ******************************************************************************
   * @brief        Input values filtering function
   * @ingroup      adc
   * @param[in]    newValue - New value for filtering from ADC
   * @return       Average value
   ******************************************************************************
   */

static uint16_t filter (uint16_t newValue)
{
    static uint8_t  index   = 0;                  ///< Contains the position in the array of the last value added
    static uint16_t average = 0;                  ///< Stores the sum of values for averaging
    static uint16_t values[NUMBERS_IN_FILTER];    ///< Array for averaging

    if (++index >= NUMBERS_IN_FILTER)
        index = 0;

    average -= values[index];
    average += newValue;
    values[index] = newValue;

    return (average / NUMBERS_IN_FILTER);
}


/**
   ******************************************************************************
   * @brief      Function for taking a value from the ADC
   * @ingroup    adc
   * @return     Value from ADC
   ******************************************************************************
   */

uint16_t getAdcValue (void)
{
    HAL_StatusTypeDef halStatus;
    uint16_t          currentValue = HAL_ADC_GetValue (&adc);
    currentValue = filter (currentValue);

    halStatus = HAL_ADC_Stop (&adc);

    if (halStatus != HAL_OK)
    {
        errorHandler ();
    }

    halStatus = HAL_ADC_Start (&adc);

    if (halStatus != HAL_OK)
    {
        errorHandler ();
    }

    halStatus = HAL_ADC_PollForConversion (&adc, POLL_FOR_CONVERSION_TIMEOUT);

    if (halStatus != HAL_OK)
    {
        errorHandler ();
    }

    return currentValue;
}


/**
   ******************************************************************************
   * @brief      Function for passing a pointer to a ADC
   * @ingroup    adc
   * @return     Pointer ADC Handler
   ******************************************************************************
   */

ADC_HandleTypeDef getADC (void)
{
    return adc;
}


/**
   ******************************************************************************
   * @brief        Function implementing the taskADC thread.
   * @param[in]    argument - Not used
   * @return       None
   ******************************************************************************
   */

void startTaskADC (void *argument)
{
    osStatus_t  osStatus;
    osMutexId_t mutexErrorHandle = getMutexErrorHandle ();

    if (startADC () != HAL_OK)
    {
        osStatus = osMutexAcquire (mutexErrorHandle, osWaitForever);

        if ((osStatus == osErrorParameter) || (osStatus == osErrorISR))
        {
            errorHandler ();
        }
    }

    messageADC_t       messageADC;
    osMessageQueueId_t queueAdcHandle = getQueueAdcHandle ();

    for (;;)
    {
        if (osMutexGetOwner (mutexErrorHandle) == NULL)
        {
            messageADC.Buf = getAdcValue ();
            osStatus = osMessageQueuePut (queueAdcHandle, &messageADC, 0, osWaitForever);

            if (osStatus == osErrorParameter)
            {
                errorHandler ();
            }
        }

        osStatus = osDelay (MINIMUM_DELAY);

        if (osStatus != osOK)
        {
            errorHandler ();
        }
    }
}
