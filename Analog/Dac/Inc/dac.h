/**
  ******************************************************************************
  * @file     dac.h
  * @brief    This file contains all the function prototypes for
  *           the dac.c file
  ******************************************************************************
  */

#ifndef __DAC_H__
#define __DAC_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"
#include "freertos.h"
#include "timer.h"
#include <math.h>
#include <stdio.h>
#include <string.h>


/**
   ******************************************************************************
   * @defgroup    dac DAC
   * @brief       This module is designed to initialize the DAC peripheral
   ******************************************************************************
   * @{
   */


HAL_StatusTypeDef initializeDAC (void);

HAL_StatusTypeDef startDAC (void);

DAC_HandleTypeDef getDAC (void);

DMA_HandleTypeDef getDmaDAC (void);

void startTaskDAC (void *argument);

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* __DAC_H__ */

