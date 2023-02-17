
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f7xx_hal.h"

#define CRYSTAL_SELECT_Pin GPIO_PIN_3
#define CRYSTAL_SELECT_GPIO_Port GPIOE
#define BUTTON_Pin GPIO_PIN_13
#define BUTTON_GPIO_Port GPIOC
#define BUTTON_EXTI_IRQn EXTI15_10_IRQn
#define LED_RED_Pin GPIO_PIN_14
#define LED_RED_GPIO_Port GPIOB
#define LED_BLUE_Pin GPIO_PIN_7
#define LED_BLUE_GPIO_Port GPIOB


void errorHandler(void);

/* Display the line number and file name where the error occurred and call the error handler */
#define ASSERT(cond) if( cond ) { printf( "Assertion error line %d, file(%s)\n", \
__LINE__, __FILE__ ); errorHandler();}

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
