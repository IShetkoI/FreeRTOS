/**
   ******************************************************************************
   * @file    spi.h
   * @brief   This file contains prototype function to initialize the I2C
   *          interface
   ******************************************************************************
   */


#ifndef SPI_INITIALIZATION_H
#define SPI_INITIALIZATION_H

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"


/**
   ******************************************************************************
   * @defgroup  spi SPI
   * @brief     This module is designed to initialize the SPI interface
   ******************************************************************************
   * @{
   */


/**
   ******************************************************************************
   * @brief    SPI initialization function
   * @returns  Status of the initialization
   ******************************************************************************
  */

HAL_StatusTypeDef initializeSpi (void);


/**
   ******************************************************************************
   * @brief    Getting a pointer to all files
   * @returns  Pointer to the descriptor of the selected I2C hardware device
   ******************************************************************************
  */

SPI_HandleTypeDef getPointerSpi (void);


/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* SPI_INITIALIZATION_H: */

