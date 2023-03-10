/**
   ******************************************************************************
   * @file     adc.h
   * @brief    This file contains all the function prototypes for
   *           the adc.c file
   ******************************************************************************
   */

#ifndef __ADC_H__
#define __ADC_H__

#ifdef __cplusplus
extern "C" {
#endif


#include <freertos.h>
#include <main.h>


/**
   ******************************************************************************
   * @defgroup     adc ADC
   * @brief        This module is designed to initialize the ADC peripheral
   ******************************************************************************
   * @{
   */


HAL_StatusTypeDef initializeADC (void);

HAL_StatusTypeDef startADC (void);

uint16_t getAdcValue (void);

ADC_HandleTypeDef getADC (void);

void startTaskADC (void *argument);

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* __ADC_H__ */

