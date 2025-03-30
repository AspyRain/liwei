#include "systick.h"
#include "stm32f1xx_hal.h"
#include "tim.h"
/**
  * @brief  微秒级延时
  * @param  us: 延时时间 (微秒)
  * @retval 无
  */
 void delay_us(uint16_t _usTime)
 {
     __HAL_TIM_SetCounter(&htim3,0);
     __HAL_TIM_ENABLE(&htim3);
     while(__HAL_TIM_GetCounter(&htim3) < _usTime);
     __HAL_TIM_DISABLE(&htim3);
 }
 
