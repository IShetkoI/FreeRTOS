#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

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


#define STACK_SIZE      4096
#define STRING_SIZE     128
#define FLASHING_PERIOD 250U
#define MINIMUM_DELAY   1U

typedef StaticTask_t      osStaticThreadDef_t;
typedef StaticQueue_t     osStaticMessageQDef_t;
typedef StaticSemaphore_t osStaticMutexDef_t;
typedef StaticSemaphore_t osStaticSemaphoreDef_t;


typedef struct
{
    char Buf[STRING_SIZE];
}   queueUSART_t;


typedef struct
{
    uint16_t Buf;
}   queueADC_t;


osSemaphoreId_t getButtonSemaphore (void);


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


void startTaskDefault (void *argument);

void startTaskBMP280 (void *argument);

void startTaskADC (void *argument);

void startTaskDAC (void *argument);

void startTaskUSART (void *argument);

void initializeFreeRTOS (void);

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void initializeFreeRTOS (void)
{

    mutexErrorHandle = osMutexNew (&mutexError_attributes);

    /* Create the semaphores(s) */
    /* creation of semaphoreButton */
    semaphoreButtonHandle = osSemaphoreNew (1, 1, &semaphoreButton_attributes);


    /* Create the queue(s) */
    /* creation of queueUSART */
    queueUSARTHandle = osMessageQueueNew (10, sizeof (queueUSART_t), &queueUSART_attributes);

    /* creation of queueADC */
    queueADCHandle = osMessageQueueNew (10, sizeof (queueADC_t), &queueADC_attributes);


    /* Create the thread(s) */
    /* creation of taskDefault */
    taskDefaultHandle = osThreadNew (startTaskDefault, NULL, &taskDefault_attributes);

    /* creation of taskBMP280 */
    taskBMP280Handle = osThreadNew (startTaskBMP280, NULL, &taskBMP280_attributes);

    /* creation of taskADC */
    taskADCHandle = osThreadNew (startTaskADC, NULL, &taskADC_attributes);

    /* creation of taskDAC */
    taskDACHandle = osThreadNew (startTaskDAC, NULL, &taskDAC_attributes);

    /* creation of taskUSART */
    taskUSARTHandle = osThreadNew (startTaskUSART, NULL, &taskUSART_attributes);

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

/**
* @brief Function implementing the taskBMP280 thread.
* @param argument: Not used
* @retval None
*/
void startTaskBMP280 (void *argument)
{
    queueUSART_t      messageUSART;
    BMP280            bmp280;
    char              temp[STRING_SIZE] = "";
    SPI_HandleTypeDef hspi1             = getPointerSpi ();

    if (initializeBmp280 (&bmp280, &hspi1, CRYSTAL_SELECT_Pin, oversampling_x16, oversampling_x2, mode_normal,
                          filter_coeff_16, standby_time_500us) != BMP280_OK)
    {
        osMutexAcquire (mutexErrorHandle, osWaitForever);
    }

    for (;;)
    {
        if (osMutexGetOwner (mutexErrorHandle) == NULL)
        {
            if (osSemaphoreGetCount (semaphoreButtonHandle) == 0)
            {
                measure (&bmp280);
                strcpy (messageUSART.Buf, "Temperature - ");
                sprintf (temp, "%d", bmp280.measurement.temperature);
                strcat (messageUSART.Buf, temp);

                strcat (messageUSART.Buf, ";   Pressure - ");
                sprintf (temp, "%d", bmp280.measurement.pressure);
                strcat (messageUSART.Buf, temp);

                osMessageQueuePut (queueUSARTHandle, &messageUSART, 0, osWaitForever);

                HAL_GPIO_TogglePin (LED_BLUE_GPIO_Port, LED_BLUE_Pin);
                osDelay (FLASHING_PERIOD);
            }
        }

        osDelay (MINIMUM_DELAY);
    }
}

/**
* @brief Function implementing the taskADC thread.
* @param argument: Not used
* @retval None
*/
void startTaskADC (void *argument)
{
    if (startADC () != HAL_OK)
    {
        osMutexAcquire (mutexErrorHandle, osWaitForever);
    }

    queueADC_t messageADC;

    for (;;)
    {
        if (osMutexGetOwner (mutexErrorHandle) == NULL)
        {
            messageADC.Buf = getAdcValue ();
            osMessageQueuePut (queueADCHandle, &messageADC, 0, osWaitForever);
        }

        osDelay (MINIMUM_DELAY);
    }
}


/**
* @brief Function implementing the taskDAC thread.
* @param argument: Not used
* @retval None
*/
void startTaskDAC (void *argument)
{
    if (startDAC () != HAL_OK)
    {
        osMutexAcquire (mutexErrorHandle, osWaitForever);
    }

    queueADC_t   messageADC;
    queueUSART_t messageUSART;


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


/**
* @brief Function implementing the taskUSART thread.
* @param argument: Not used
* @retval None
*/
void startTaskUSART (void *argument)
{
    queueUSART_t messageUSART;


    for (;;)
    {
        if (osMutexGetOwner (mutexErrorHandle) == NULL)
        {
            if (osMessageQueueGetCount (queueUSARTHandle) != 0)
            {
                if (osMessageQueueGet (queueUSARTHandle, &messageUSART, 0, osWaitForever) == osOK)
                {
                    strcat (messageUSART.Buf, "\r\n\0");
                    printf ("%s", messageUSART.Buf);
                }
            }
        }
        osDelay (MINIMUM_DELAY);
    }
}


osSemaphoreId_t getButtonSemaphore (void)
{
    return semaphoreButtonHandle;
}

