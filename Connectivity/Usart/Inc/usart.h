/**
   ******************************************************************************
   * @file    usart.h
   * @author  Ivan Shetska
   * @brief   This file contains prototype functions to initialize the USART
   *          interface
   ******************************************************************************
   */

#ifndef USART_H
#define USART_H

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"


/**
   ******************************************************************************
   * @defgroup  usart USART
   * @brief     This module is designed to initialize the USART interface
   ******************************************************************************
   * @{
   */


/**
   ******************************************************************************
   * @brief    USART initialization function
   * @returns  Status of the initialization
   ******************************************************************************
  */

HAL_StatusTypeDef initializeUsart (void);


/**
   ******************************************************************************
   * @brief     Function implementing the taskUSART thread.
   * @param[in] argument - Not used
   * @retval    None
   ******************************************************************************
   */

void startTaskUSART (void *argument);

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* USART_H: */
