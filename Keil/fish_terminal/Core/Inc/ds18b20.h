#ifndef __DS18B20_H
#define __DS18B20_H

#include "main.h"

// ??DS18B20????
#define DS18B20_CMD_SKIP_ROM        0xCC
#define DS18B20_CMD_CONVERT_T       0x44
#define DS18B20_CMD_READ_SCRATCHPAD 0xBE

void DS18B20_Init(void);
uint8_t DS18B20_Reset(void);
void DS18B20_StartConv(void);
float DS18B20_GetTemp(void);

#endif
