#ifndef __SYSTICK_H
#define __SYSTICK_H

#include "stm32f1xx_hal.h"

void SysTick_Init(void);
void delay_us(uint32_t us);
void Delay_ms(uint32_t ms);

#endif /* __SYSTICK_H */
