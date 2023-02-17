
#ifndef __TIM_H__
#define __TIM_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "../../../Core/Inc/main.h"


void MX_TIM6_Init(void);

void setTimerConfig (uint16_t adcValue);

void startTimerBase(void);

TIM_HandleTypeDef getTimer(void);

#ifdef __cplusplus
}
#endif

#endif /* __TIM_H__ */

