#ifndef __DS18B20_H
#define __DS18B20_H 
#include "stm32f1xx_hal.h"
#include "main.h"
   	
uint8_t  DS18B20_Init(void);//³õÊ¼»¯DS18B20
short DS18B20_Get_Temp(void);//»ñÈ¡ÎÂ¶È
void DS18B20_Start(void);//¿ªÊ¼ÎÂ¶È×ª»»
void DS18B20_Write_Byte(uint8_t dat);//Ð´ÈëÒ»¸ö×Ö½Ú
uint8_t  DS18B20_Read_Byte(void);//¶Á³öÒ»¸ö×Ö½Ú
uint8_t  DS18B20_Read_Bit(void);//¶Á³öÒ»¸öÎ»
uint8_t  DS18B20_Check(void);//¼ì²âÊÇ·ñ´æÔÚDS18B20
void DS18B20_Rst(void);//¸´Î»DS18B20    
#endif















