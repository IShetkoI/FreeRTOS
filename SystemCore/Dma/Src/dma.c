/**
  ******************************************************************************
  * @file     dma.c
  * @brief    This file provides code for the configuration
  *           of all the requested memory to memory DMA transfers.
  ******************************************************************************
  */

#include "dma.h"


/**
   ******************************************************************************
   * @brief      DMA initialization function
   * @ingroup    dma
   ******************************************************************************
   */

void initializeDMA (void)
{
    /* DMA controller clock enable */
    __HAL_RCC_DMA1_CLK_ENABLE ();

    /* DMA interrupt init */
    /* DMA1_Stream5_IRQn interrupt configuration */
    HAL_NVIC_SetPriority (DMA1_Stream5_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ (DMA1_Stream5_IRQn);
}
