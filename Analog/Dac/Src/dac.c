/**
  ******************************************************************************
  * @file    dac.c
  * @brief   This file provides code for the configuration
  *          of the DAC instances.
  ******************************************************************************
  */

#include "dac.h"
#include "freertos.h"
#include "timer.h"
#include <math.h>
#include <stdio.h>

#define DOTS      255                    ///< Number of points to generate a sine wave
#define QUARTERES 4                      ///< Number of quarters of a circle
#define PI        3.1415927              ///< The number of Pi

static uint16_t sine[DOTS * QUARTERES];  ///< Stores the corresponding digital values of the sine points

static DAC_HandleTypeDef dac;            ///< Pointer DAC Handler
static DMA_HandleTypeDef dmaDAC;         ///< Pointer DMA Handler


/**
   ******************************************************************************
   * @brief    DAC initialization function
   * @ingroup  dac
   ******************************************************************************
   */

void initializeDAC (void)
{

    DAC_ChannelConfTypeDef sConfig = {0};

    /** DAC Initialization
    */
    dac.Instance = DAC;

    ASSERT (HAL_DAC_Init (&dac) != HAL_OK);

    /** DAC channel OUT1 config
    */
    sConfig.DAC_Trigger      = DAC_TRIGGER_T6_TRGO;
    sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;

    ASSERT (HAL_DAC_ConfigChannel (&dac, &sConfig, DAC_CHANNEL_1) != HAL_OK);
}


/**
   ******************************************************************************
   * @brief      This function configures the hardware resources used in this
   *             example
   * @ingroup    dac
   * @param[in]  dacHandle - DAC handle pointer
   ******************************************************************************
   */

void HAL_DAC_MspInit (DAC_HandleTypeDef *dacHandle)
{

    GPIO_InitTypeDef GPIO_InitStruct = {0};
    if (dacHandle->Instance == DAC)
    {
        /* DAC clock enable */
        __HAL_RCC_DAC_CLK_ENABLE ();

        __HAL_RCC_GPIOA_CLK_ENABLE ();
        /**DAC GPIO Configuration
        PA4     ------> DAC_OUT1
        */
        GPIO_InitStruct.Pin  = GPIO_PIN_4;
        GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        HAL_GPIO_Init (GPIOA, &GPIO_InitStruct);

        /* DAC DMA Init */
        /* DAC1 Init */
        dmaDAC.Instance                 = DMA1_Stream5;
        dmaDAC.Init.Channel             = DMA_CHANNEL_7;
        dmaDAC.Init.Direction           = DMA_MEMORY_TO_PERIPH;
        dmaDAC.Init.PeriphInc           = DMA_PINC_DISABLE;
        dmaDAC.Init.MemInc              = DMA_MINC_ENABLE;
        dmaDAC.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
        dmaDAC.Init.MemDataAlignment    = DMA_MDATAALIGN_HALFWORD;
        dmaDAC.Init.Mode                = DMA_CIRCULAR;
        dmaDAC.Init.Priority            = DMA_PRIORITY_LOW;
        dmaDAC.Init.FIFOMode            = DMA_FIFOMODE_DISABLE;

        ASSERT (HAL_DMA_Init (&dmaDAC) != HAL_OK);

        __HAL_LINKDMA (dacHandle, DMA_Handle1, dmaDAC);

        /* DAC interrupt Init */
        HAL_NVIC_SetPriority (TIM6_DAC_IRQn, 5, 0);
        HAL_NVIC_EnableIRQ (TIM6_DAC_IRQn);
    }
}


/**
   ******************************************************************************
   * @brief      This function freeze the hardware resources used in this example
   * @ingroup    dac
   * @param[in]  dacHandle - DAC handle pointer
   ******************************************************************************
   */

void HAL_DAC_MspDeInit (DAC_HandleTypeDef *dacHandle)
{
    if (dacHandle->Instance == DAC)
    {
        /* Peripheral clock disable */
        __HAL_RCC_DAC_CLK_DISABLE ();

        /**DAC GPIO Configuration
        PA4     ------> DAC_OUT1
        */
        HAL_GPIO_DeInit (GPIOA, GPIO_PIN_4);

        /* DAC DMA DeInit */
        HAL_DMA_DeInit (dacHandle->DMA_Handle1);
    }
}


/**
   ******************************************************************************
   * @brief      Sinusoidal generation function
   * @ingroup    dac
   ******************************************************************************
   */

static void calculationSine (void)
{
    for (int i = 0; i < DOTS * QUARTERES; i++)
    {
        sine[i] = (sin (i * 2 * PI / DOTS) + 1) * ((0xFFF + 1) / 2);
    }
}


/**
   ******************************************************************************
   * @brief      DAC startup function
   * @ingroup    dac
   * @return     DAC start status
   ******************************************************************************
   */

HAL_StatusTypeDef startDAC (void)
{
    if (HAL_DAC_Start (&dac, DAC_CHANNEL_1) != HAL_OK)
        return HAL_ERROR;

    calculationSine ();

    return HAL_DAC_Start_DMA (&dac, DAC_CHANNEL_1, (uint32_t *) sine, DOTS * QUARTERES, DAC_ALIGN_12B_R);
}


/**
   ******************************************************************************
   * @brief      Function for passing a pointer to a DAC
   * @ingroup    dac
   * @return     Pointer DAC Handler
   ******************************************************************************
   */

DAC_HandleTypeDef getDAC ()
{
    return dac;
}


/**
   ******************************************************************************
   * @brief      Function for passing a pointer to a DAC DMA
   * @ingroup    dac
   * @return     Pointer DAC DMA Handler
   ******************************************************************************
   */

DMA_HandleTypeDef getDmaDAC ()
{
    return dmaDAC;
}


/**
   ******************************************************************************
   * @brief      Function implementing the taskDAC thread.
   * @param[in]  argument - Not used
   * @retval     None
   ******************************************************************************
   */

void startTaskDAC (void *argument)
{
    osMutexId_t mutexErrorHandle = getMutexErrorHandle();

    if (startDAC () != HAL_OK)
    {
        osMutexAcquire (mutexErrorHandle, osWaitForever);
    }

    queueADC_t   messageADC;
    queueUSART_t messageUSART;
    osMessageQueueId_t queueADCHandle = getQueueAdcHandle();
    osMessageQueueId_t queueUSARTHandle = getQueueUsartHandle();


    for (;;)
    {
        if (osMutexGetOwner (mutexErrorHandle) == NULL)
        {
            if (osMessageQueueGetCount (queueADCHandle) != 0)
            {
                if (osMessageQueueGet (queueADCHandle, &messageADC, 0, osWaitForever) == osOK)
                {
                    setTimerConfig (messageADC.Buf);

                    char temp[STRING_SIZE] = "";

                    sprintf (temp, "%d", messageADC.Buf);

                    strcpy (messageUSART.Buf, "ADC value - ");
                    strcat (messageUSART.Buf, temp);

                    osMessageQueuePut (queueUSARTHandle, &messageUSART, 0, osWaitForever);
                }
            }
        }

        osDelay (MINIMUM_DELAY);
    }
}
