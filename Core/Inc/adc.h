#ifndef __ADC_H__
#define __ADC_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"

#define NUMBERS_IN_FILTER 8

void MX_ADC1_Init(void);

HAL_StatusTypeDef startADC (void);

uint16_t getAdcValue(void);

ADC_HandleTypeDef getADC(void);

#ifdef __cplusplus
}
#endif

#endif /* __ADC_H__ */

