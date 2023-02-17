/**
  ******************************************************************************
  * @file    stm32f7xx_it.h
  * @brief   This file contains the headers of the interrupt handlers.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
 ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32F7xx_IT_H
#define __STM32F7xx_IT_H

#ifdef __cplusplus
extern "C" {
#endif

void NMI_Handler (void);

void HardFault_Handler (void);

void MemManage_Handler (void);

void BusFault_Handler (void);

void UsageFault_Handler (void);

void DebugMon_Handler (void);

void DMA1_Stream5_IRQHandler (void);

void TIM1_UP_TIM10_IRQHandler (void);

void EXTI15_10_IRQHandler (void);

void TIM6_DAC_IRQHandler (void);


#ifdef __cplusplus
}
#endif

#endif /* __STM32F7xx_IT_H */
