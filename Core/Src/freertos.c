#include "freertos.h"

#include "adc.h"
#include "dac.h"
#include "timer.h"
#include "gpio.h"
#include "usart.h"
#include "spi.h"
#include "string.h"
#include "stdio.h"
#include "stdbool.h"
#include "bmp280.h"


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
uint8_t                    queueUSARTBuffer[10 * sizeof (queueUSART_t)];
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
uint8_t                    queueADCBuffer[10 * sizeof (queueADC_t)];
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
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void initializeFreeRTOS (void)
{

    mutexErrorHandle = osMutexNew (&mutexError_attributes);

    if(mutexErrorHandle == NULL)
    {
        errorHandler();
    }

    /* Create the semaphores(s) */
    /* creation of semaphoreButton */
    semaphoreButtonHandle = osSemaphoreNew (1, 1, &semaphoreButton_attributes);

    if(semaphoreButtonHandle == NULL)
    {
        errorHandler();
    }

    /* Create the queue(s) */
    /* creation of queueUSART */
    queueUSARTHandle = osMessageQueueNew (10, sizeof (queueUSART_t), &queueUSART_attributes);

    if(queueUSARTHandle == NULL)
    {
        errorHandler();
    }

    /* creation of queueADC */
    queueADCHandle = osMessageQueueNew (10, sizeof (queueADC_t), &queueADC_attributes);

    if(queueADCHandle == NULL)
    {
        errorHandler();
    }

    /* Create the thread(s) */
    /* creation of taskDefault */
    taskDefaultHandle = osThreadNew (startTaskDefault, NULL, &taskDefault_attributes);

    if(taskDefaultHandle == NULL)
    {
        errorHandler();
    }

    /* creation of taskBMP280 */
    taskBMP280Handle = osThreadNew (startTaskBMP280, NULL, &taskBMP280_attributes);

    if(taskBMP280Handle == NULL)
    {
        errorHandler();
    }

    /* creation of taskADC */
    taskADCHandle = osThreadNew (startTaskADC, NULL, &taskADC_attributes);

    if(taskADCHandle == NULL)
    {
        errorHandler();
    }

    /* creation of taskDAC */
    taskDACHandle = osThreadNew (startTaskDAC, NULL, &taskDAC_attributes);

    if(taskDACHandle == NULL)
    {
        errorHandler();
    }

    /* creation of taskUSART */
    taskUSARTHandle = osThreadNew (startTaskUSART, NULL, &taskUSART_attributes);

    if(taskUSARTHandle == NULL)
    {
        errorHandler();
    }
}

/**
  * @brief  Function implementing the taskDefault thread.
  * @param  argument: Not used
  * @retval None
  */
void startTaskDefault (void *argument)
{
    for (;;)
    {
        if (osMutexGetOwner (mutexErrorHandle) != NULL)
        {
            HAL_GPIO_WritePin (LED_BLUE_GPIO_Port, LED_BLUE_Pin, GPIO_PIN_RESET);
            HAL_GPIO_TogglePin (LED_RED_GPIO_Port, LED_RED_Pin);
            osDelay (FLASHING_PERIOD);
        }

        osDelay (MINIMUM_DELAY);
    }
}

osMutexId_t getMutexErrorHandle(void)
{
    return mutexErrorHandle;
}

osSemaphoreId_t getSemaphoreButtonHandle (void)
{
    return semaphoreButtonHandle;
}

osThreadId_t getTaskAdcHandle(void)
{
    return taskADCHandle;
}

osThreadId_t getTaskBmp280Handle(void)
{
    return taskBMP280Handle;
}

osThreadId_t getTaskDacHandle(void)
{
    return taskDACHandle;
}

osThreadId_t getTaskUsartHandle(void)
{
    return taskUSARTHandle;
}

osMessageQueueId_t getQueueAdcHandle(void)
{
    return queueADCHandle;
}

osMessageQueueId_t getQueueUsartHandle(void)
{
    return queueUSARTHandle;
}
