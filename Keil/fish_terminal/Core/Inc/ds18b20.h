#ifndef DS18B20_H
#define DS18B20_H

#include "stm32f1xx_hal.h"

// 定义用于连接的单总线引脚
#define DS18B20_PIN GPIO_PIN_12
#define DS18B20_PORT GPIOA

// 函数声明
void ds18b20_init(void);
float ds18b20_read_temperature(void);
void delay_us(uint32_t time);
#endif // DS18B20_H
