/**
   ******************************************************************************
   * @file     clock.h
   * @brief    This file contains prototype function to configure the System
   *           clock
   ******************************************************************************
   */


#ifndef CLOCK_H
#define CLOCK_H

#ifdef __cplusplus
extern "C" {
#endif


#include "main.h"

/**
   ******************************************************************************
   * @defgroup    clock Clock
   * @brief       This module is designed to configure the System clock
   ******************************************************************************
   * @{
   */


/**
   ******************************************************************************
   * @brief      Clock initialization function
   * @return     Status of the initialization
   ******************************************************************************
  */

HAL_StatusTypeDef initializeClock (void);


/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* CLOCK_H: */
