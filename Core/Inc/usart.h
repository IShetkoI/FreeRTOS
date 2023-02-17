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


/* Define to prevent recursive inclusion --------------------------------------*/
#include "main.h"


/**
   ******************************************************************************
   * @defgroup  usart USART
   * @brief     This module is designed to initialize the USART interface
   ******************************************************************************
   * @{
   */

#define BAUD_RATE 115200 ///< Information transfer rate
#define UART_SIZE 1      ///< Package size


/**
   ******************************************************************************
   * @brief    USART initialization function
   * @returns  Status of the initialization
   ******************************************************************************
  */

HAL_StatusTypeDef initializeUsart (void);


UART_HandleTypeDef getUSART(void);
/**
  * @}
  */

#endif /* USART_H: */
