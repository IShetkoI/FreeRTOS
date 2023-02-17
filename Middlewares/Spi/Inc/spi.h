/**
   ******************************************************************************
   * @file    spi_initialization.h
   * @author  Ivan Shetska
   * @brief   This file contains prototype function to initialize the I2C
   *          interface
   ******************************************************************************
   */


#ifndef SPI_INITIALIZATION_H
#define SPI_INITIALIZATION_H


/* Define to prevent recursive inclusion --------------------------------------*/
#include "../../../Core/Inc/main.h"


#define CRC_POLYNOMIAL 7


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


#endif /* SPI_INITIALIZATION_H: */

