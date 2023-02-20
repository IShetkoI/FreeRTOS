/**
  ******************************************************************************
  * @file     timer.h
  * @brief    This file contains all the function prototypes for
  *           the timer.c file
  ******************************************************************************
  */

#ifndef __TIM_H__
#define __TIM_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"

/**
   ******************************************************************************
   * @defgroup    timer Timer
   * @brief       This module is designed to initialize the Timer
   ******************************************************************************
   * @{
   */

HAL_StatusTypeDef initializeTimer6 (void);

void setTimerConfig (uint16_t adcValue);

HAL_StatusTypeDef startTimerBase (void);

TIM_HandleTypeDef getTimer (void);


/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* __TIM_H__ */

