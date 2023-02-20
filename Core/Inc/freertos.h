/**
   ******************************************************************************
   * @file     freertos.h
   * @brief    This file contains prototype functions to freertos
   ******************************************************************************
   */

#ifndef FREERTOS_H
#define FREERTOS_H

#ifdef __cplusplus
extern "C" {
#endif


#include "FreeRTOS.h"
#include <cmsis_os2.h>

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
} messageUSART_t;

typedef struct
{
    uint16_t Buf;
} messageADC_t;


/**
   ******************************************************************************
   * @defgroup    freertos FreeRTOS
   * @brief       This module is designed to FreeRTOS
   ******************************************************************************
   * @{
   */

void startTaskDefault (void *argument);

osStatus_t initializeFreeRTOS (void);

osMutexId_t getMutexErrorHandle (void);

osSemaphoreId_t getSemaphoreButtonHandle (void);

osThreadId_t getTaskAdcHandle (void);

osThreadId_t getTaskBmp280Handle (void);

osThreadId_t getTaskUsartHandle (void);

osMessageQueueId_t getQueueAdcHandle (void);

osMessageQueueId_t getQueueUsartHandle (void);


/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* FREERTOS_H: */
