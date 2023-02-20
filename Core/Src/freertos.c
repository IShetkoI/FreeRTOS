#include "freertos.h"

#include "adc.h"
#include "dac.h"
#include "usart.h"
#include "bmp280.h"


#define QUEUE_SIZE 10


/* Definitions for taskDefault */
osThreadId_t         taskDefaultHandle;
uint32_t             taskDefaultBuffer[STACK_SIZE];
osStaticThreadDef_t  taskDefaultControlBlock;
const osThreadAttr_t taskDefault_attributes =
        {
        .name = "taskDefault",
        .cb_mem = &taskDefaultControlBlock,
        .cb_size = sizeof (taskDefaultControlBlock),
        .stack_mem = &taskDefaultBuffer[0],
        .stack_size = sizeof (taskDefaultBuffer),
        .priority = (osPriority_t) osPriorityNormal,
        };

/* Definitions for taskBMP280 */
osThreadId_t         taskBMP280Handle;
uint32_t             taskBMP280Buffer[STACK_SIZE];
osStaticThreadDef_t  taskBMP280ControlBlock;
const osThreadAttr_t taskBMP280_attributes =
        {
        .name = "taskBMP280",
        .cb_mem = &taskBMP280ControlBlock,
        .cb_size = sizeof (taskBMP280ControlBlock),
        .stack_mem = &taskBMP280Buffer[0],
        .stack_size = sizeof (taskBMP280Buffer),
        .priority = (osPriority_t) osPriorityLow,
        };

/* Definitions for taskADC */
osThreadId_t         taskADCHandle;
uint32_t             taskADCBuffer[STACK_SIZE];
osStaticThreadDef_t  taskADCControlBlock;
const osThreadAttr_t taskADC_attributes =
        {
        .name = "taskADC",
        .cb_mem = &taskADCControlBlock,
        .cb_size = sizeof (taskADCControlBlock),
        .stack_mem = &taskADCBuffer[0],
        .stack_size = sizeof (taskADCBuffer),
        .priority = (osPriority_t) osPriorityLow,
        };

/* Definitions for taskDAC */
osThreadId_t         taskDACHandle;
uint32_t             taskDACBuffer[STACK_SIZE];
osStaticThreadDef_t  taskDACControlBlock;
const osThreadAttr_t taskDAC_attributes =
        {
        .name = "taskDAC",
        .cb_mem = &taskDACControlBlock,
        .cb_size = sizeof (taskDACControlBlock),
        .stack_mem = &taskDACBuffer[0],
        .stack_size = sizeof (taskDACBuffer),
        .priority = (osPriority_t) osPriorityLow,
        };


/* Definitions for taskUSART */
osThreadId_t         taskUSARTHandle;
uint32_t             taskUSARTBuffer[STACK_SIZE];
osStaticThreadDef_t  taskUSARTControlBlock;
const osThreadAttr_t taskUSART_attributes =
        {
        .name = "taskUSART",
        .cb_mem = &taskUSARTControlBlock,
        .cb_size = sizeof (taskUSARTControlBlock),
        .stack_mem = &taskUSARTBuffer[0],
        .stack_size = sizeof (taskUSARTBuffer),
        .priority = (osPriority_t) osPriorityLow,
        };

/* Definitions for queueUSART */
osMessageQueueId_t         queueUSARTHandle;
uint8_t                    queueUSARTBuffer[QUEUE_SIZE * sizeof (messageUSART_t)];
osStaticMessageQDef_t      queueUSARTControlBlock;
const osMessageQueueAttr_t queueUSART_attributes =
        {
        .name = "queueUSART",
        .cb_mem = &queueUSARTControlBlock,
        .cb_size = sizeof (queueUSARTControlBlock),
        .mq_mem = &queueUSARTBuffer,
        .mq_size = sizeof (queueUSARTBuffer)
        };
/* Definitions for queueADC */
osMessageQueueId_t         queueADCHandle;
uint8_t                    queueADCBuffer[QUEUE_SIZE * sizeof (messageADC_t)];
osStaticMessageQDef_t      queueADCControlBlock;
const osMessageQueueAttr_t queueADC_attributes   =
        {
        .name = "queueADC",
        .cb_mem = &queueADCControlBlock,
        .cb_size = sizeof (queueADCControlBlock),
        .mq_mem = &queueADCBuffer,
        .mq_size = sizeof (queueADCBuffer)
        };

/* Definitions for mutexUSART */
osMutexId_t         mutexErrorHandle;
osStaticMutexDef_t  mutexErrorControlBlock;
const osMutexAttr_t mutexError_attributes =
        {
        .name = "mutexError",
        .cb_mem = &mutexErrorControlBlock,
        .cb_size = sizeof (mutexErrorControlBlock),
        };

/* Definitions for semaphoreButton */
osSemaphoreId_t         semaphoreButtonHandle;
osStaticSemaphoreDef_t  semaphoreButtonControlBlock;
const osSemaphoreAttr_t semaphoreButton_attributes =
        {
        .name = "semaphoreButton",
        .cb_mem = &semaphoreButtonControlBlock,
        .cb_size = sizeof (semaphoreButtonControlBlock),
        };


/**
   ******************************************************************************
   * @brief      FreeRTOS initialization
   * @ingroup    freertos
   * @return     Status of initialization
   ******************************************************************************
   */

osStatus_t initializeFreeRTOS (void)
{
    mutexErrorHandle = osMutexNew (&mutexError_attributes);

    if (mutexErrorHandle == NULL)
    {
        return osError;
    }

    /* Create the semaphores(s) */
    /* creation of semaphoreButton */
    semaphoreButtonHandle = osSemaphoreNew (1, 1, &semaphoreButton_attributes);

    if (semaphoreButtonHandle == NULL)
    {
        return osError;
    }

    /* Create the queue(s) */
    /* creation of queueUSART */
    queueUSARTHandle = osMessageQueueNew (QUEUE_SIZE, sizeof (messageUSART_t), &queueUSART_attributes);

    if (queueUSARTHandle == NULL)
    {
        return osError;
    }

    /* creation of queueADC */
    queueADCHandle = osMessageQueueNew (QUEUE_SIZE, sizeof (messageADC_t), &queueADC_attributes);

    if (queueADCHandle == NULL)
    {
        return osError;
    }

    /* Create the thread(s) */
    /* creation of taskDefault */
    taskDefaultHandle = osThreadNew (startTaskDefault, NULL, &taskDefault_attributes);

    if (taskDefaultHandle == NULL)
    {
        return osError;
    }

    /* creation of taskBMP280 */
    taskBMP280Handle = osThreadNew (startTaskBMP280, NULL, &taskBMP280_attributes);

    if (taskBMP280Handle == NULL)
    {
        return osError;
    }

    /* creation of taskADC */
    taskADCHandle = osThreadNew (startTaskADC, NULL, &taskADC_attributes);

    if (taskADCHandle == NULL)
    {
        return osError;
    }

    /* creation of taskDAC */
    taskDACHandle = osThreadNew (startTaskDAC, NULL, &taskDAC_attributes);

    if (taskDACHandle == NULL)
    {
        return osError;
    }

    /* creation of taskUSART */
    taskUSARTHandle = osThreadNew (startTaskUSART, NULL, &taskUSART_attributes);

    if (taskUSARTHandle == NULL)
    {
        return osError;
    }

    return osOK;
}


/**
   ******************************************************************************
   * @brief        Function implementing the taskDefault thread.
   * @ingroup      freertos
   * @param[in]    argument - Not used
   ******************************************************************************
   */

void startTaskDefault (void *argument)
{
    osStatus_t osStatus;

    for (;;)
    {
        if (osMutexGetOwner (mutexErrorHandle) != NULL)
        {
            HAL_GPIO_WritePin (LED_BLUE_GPIO_Port, LED_BLUE_Pin, GPIO_PIN_RESET);
            HAL_GPIO_TogglePin (LED_RED_GPIO_Port, LED_RED_Pin);

            osStatus = osDelay (FLASHING_PERIOD);

            if (osStatus != osOK)
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


/**
   ******************************************************************************
   * @brief      Getting a handler to mutexErrorHandle
   * @ingroup    freertos
   * @return     Handler to mutexErrorHandle
   ******************************************************************************
  */

osMutexId_t getMutexErrorHandle (void)
{
    return mutexErrorHandle;
}


/**
   ******************************************************************************
   * @brief      Getting a handler to semaphoreButtonHandle
   * @ingroup    freertos
   * @return     Handler to semaphoreButtonHandle
   ******************************************************************************
  */

osSemaphoreId_t getSemaphoreButtonHandle (void)
{
    return semaphoreButtonHandle;
}


/**
   ******************************************************************************
   * @brief      Getting a handler to taskADCHandle
   * @ingroup    freertos
   * @return     Handler to taskADCHandle
   ******************************************************************************
  */

osThreadId_t getTaskAdcHandle (void)
{
    return taskADCHandle;
}


/**
   ******************************************************************************
   * @brief      Getting a handler to taskBMP280Handle
   * @ingroup    freertos
   * @return     Handler to taskBMP280Handle
   ******************************************************************************
  */

osThreadId_t getTaskBmp280Handle (void)
{
    return taskBMP280Handle;
}


/**
   ******************************************************************************
   * @brief      Getting a handler to taskDACHandle
   * @ingroup    freertos
   * @return     Handler to taskDACHandle
   ******************************************************************************
  */

osThreadId_t getTaskDacHandle (void)
{
    return taskDACHandle;
}


/**
   ******************************************************************************
   * @brief      Getting a handler to taskUSARTHandle
   * @ingroup    freertos
   * @return     Handler to taskUSARTHandle
   ******************************************************************************
  */

osThreadId_t getTaskUsartHandle (void)
{
    return taskUSARTHandle;
}


/**
   ******************************************************************************
   * @brief      Getting a handler to queueADCHandle
   * @ingroup    freertos
   * @return     Handler to queueADCHandle
   ******************************************************************************
  */

osMessageQueueId_t getQueueAdcHandle (void)
{
    return queueADCHandle;
}


/**
   ******************************************************************************
   * @brief      Getting a handler to queueUSARTHandle
   * @ingroup    freertos
   * @return     Handler to queueUSARTHandle
   ******************************************************************************
  */

osMessageQueueId_t getQueueUsartHandle (void)
{
    return queueUSARTHandle;
}
