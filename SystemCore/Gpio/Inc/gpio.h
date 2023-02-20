/**
   ******************************************************************************
   * @file     gpio.h
   * @brief    This file contains prototype functions to initialize the GPIO
   *           peripheral
   ******************************************************************************
   */

#ifndef __GPIO_H__
#define __GPIO_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"


/**
   ******************************************************************************
   * @defgroup    gpio GPIO
   * @brief       This module is designed to initialize the GPIO peripheral
   ******************************************************************************
   * @{
   */

void initializeGPIO (void);


/**
  * @}
  */

#ifdef __cplusplus
}
#endif
#endif /*__ GPIO_H__ */

