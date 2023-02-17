
#ifndef __DAC_H__
#define __DAC_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "../../../Core/Inc/main.h"

void MX_DAC_Init(void);

HAL_StatusTypeDef startDAC(void);
DAC_HandleTypeDef getDAC(void);
DMA_HandleTypeDef getDmaDAC(void);

#ifdef __cplusplus
}
#endif

#endif /* __DAC_H__ */

