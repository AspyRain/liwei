#ifndef __TDS_H
#define __TDS_H

#include "stm32f1xx_hal.h"
#include <rtthread.h>

#define DS18B20_PIN        GPIO_PIN_12
#define DS18B20_PORT       GPIOA
#define DS18B20_CLK_ENABLE() __HAL_RCC_GPIOA_CLK_ENABLE()

#define DS18B20_DATA_OUT(value) HAL_GPIO_WritePin(DS18B20_PORT, DS18B20_PIN, (value) ? GPIO_PIN_SET : GPIO_PIN_RESET)
#define DS18B20_DATA_IN() HAL_GPIO_ReadPin(DS18B20_PORT, DS18B20_PIN)

void DS18B20_Init(void);
float DS18B20_Get_Temp(void);

#endif /* __TDS_H */
