/**
   ******************************************************************************
   * @file     usart.h
   * @brief    This file contains prototype functions to initialize the USART
   *           interface
   ******************************************************************************
   */

#ifndef USART_H
#define USART_H

#ifdef __cplusplus
extern "C" {
#endif


#include "main.h"
#include "freertos.h"
#include <stdio.h>
#include <string.h>

/**
   ******************************************************************************
   * @defgroup    usart USART
   * @brief       This module is designed to initialize the USART interface
   ******************************************************************************
   * @{
   */

HAL_StatusTypeDef initializeUsart (void);

void startTaskUSART (void *argument);


/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* USART_H: */
