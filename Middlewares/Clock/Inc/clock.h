/**
   ******************************************************************************
   * @file    clock.h
   * @author  Ivan Shetska
   * @brief   This file contains prototype function to configure the System
   *          clock
   ******************************************************************************
   */


#ifndef CLOCK_H
#define CLOCK_H


/* Define to prevent recursive inclusion --------------------------------------*/
#include "main.h"

/**
   ******************************************************************************
   * @defgroup clock Clock
   * @brief    This module is designed to configure the System clock
   ******************************************************************************
   * @{
   */

/**
   ******************************************************************************
   * @brief   Clock initialization function
   * @param   None
   * @returns Status of the initialization
   ******************************************************************************
  */

void initializeClock (void);

/**
  * @}
  */

#endif /* CLOCK_H: */
