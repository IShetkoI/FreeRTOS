/**
   ******************************************************************************
   * @file     spi.h
   * @brief    This file contains prototype function to initialize the SPI
   *           interface
   ******************************************************************************
   */


#ifndef SPI_H
#define SPI_H

#ifdef __cplusplus
extern "C" {
#endif


#include "main.h"


/**
   ******************************************************************************
   * @defgroup    spi SPI
   * @brief       This module is designed to initialize the SPI interface
   ******************************************************************************
   * @{
   */


/**
   ******************************************************************************
   * @brief     SPI initialization function
   * @return    Status of the initialization
   ******************************************************************************
  */

HAL_StatusTypeDef initializeSpi (void);


/**
   ******************************************************************************
   * @brief     Getting a pointer to all files
   * @return    Pointer to the descriptor of the selected I2C hardware device
   ******************************************************************************
  */

SPI_HandleTypeDef getPointerSpi (void);


/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* SPI_H: */

