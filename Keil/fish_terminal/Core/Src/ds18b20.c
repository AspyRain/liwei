#include "ds18b20.h"
#include "stm32f1xx_hal.h"
#include "systick.h"

// 配置 DS18B20 数据引脚为输出模式
void DS18B20_IO_OUT(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = DS18B20_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(DS18B20_GPIO_Port, &GPIO_InitStruct);
}

// 配置 DS18B20 数据引脚为输入模式
void DS18B20_IO_IN(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = DS18B20_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(DS18B20_GPIO_Port, &GPIO_InitStruct);
}

// 复位 DS18B20
void DS18B20_Rst(void)
{
    DS18B20_IO_OUT();  
    HAL_GPIO_WritePin(DS18B20_GPIO_Port, DS18B20_Pin, GPIO_PIN_RESET); 
    delay_us(750);
    HAL_GPIO_WritePin(DS18B20_GPIO_Port, DS18B20_Pin, GPIO_PIN_SET);   
    delay_us(15); // 15us
}

// 等待 DS18B20 的响应
uint8_t DS18B20_Check(void)
{
    uint8_t retry = 0;
    DS18B20_IO_IN(); 

    while (HAL_GPIO_ReadPin(DS18B20_GPIO_Port, DS18B20_Pin) && retry < 200)
    {
        retry++;
        delay_us(1);
    }

    if (retry >= 200) return 1;
    else retry = 0;

    while (!HAL_GPIO_ReadPin(DS18B20_GPIO_Port, DS18B20_Pin) && retry < 240)
    {
        retry++;
        delay_us(1);
    }

    if (retry >= 240) return 1;
    return 0;
}

// 读取 1 位数据
uint8_t DS18B20_Read_Bit(void)
{
    uint8_t data;
    DS18B20_IO_OUT();  
    HAL_GPIO_WritePin(DS18B20_GPIO_Port, DS18B20_Pin, GPIO_PIN_RESET);
    delay_us(2);
    HAL_GPIO_WritePin(DS18B20_GPIO_Port, DS18B20_Pin, GPIO_PIN_SET);
    DS18B20_IO_IN();  
    delay_us(12);

    if (HAL_GPIO_ReadPin(DS18B20_GPIO_Port, DS18B20_Pin)) data = 1;
    else data = 0;

    delay_us(50);
    return data;
}

// 读取 1 字节数据
uint8_t DS18B20_Read_Byte(void)
{
    uint8_t i, data = 0;
    for (i = 0; i < 8; i++)
    {
        data >>= 1;
        if (DS18B20_Read_Bit()) data |= 0x80;
    }
    return data;
}

// 写入 1 字节数据
void DS18B20_Write_Byte(uint8_t data)
{
    uint8_t i;
    DS18B20_IO_OUT();
    for (i = 0; i < 8; i++)
    {
        if (data & 0x01) 
        {
            HAL_GPIO_WritePin(DS18B20_GPIO_Port, DS18B20_Pin, GPIO_PIN_RESET);
            delay_us(2);
            HAL_GPIO_WritePin(DS18B20_GPIO_Port, DS18B20_Pin, GPIO_PIN_SET);
            delay_us(60);
        }
        else 
        {
            HAL_GPIO_WritePin(DS18B20_GPIO_Port, DS18B20_Pin, GPIO_PIN_RESET);
            delay_us(60);
            HAL_GPIO_WritePin(DS18B20_GPIO_Port, DS18B20_Pin, GPIO_PIN_SET);
            delay_us(2);
        }
        data >>= 1;
    }
}

// 开始温度转换
void DS18B20_Start(void)
{
    DS18B20_Rst();
    DS18B20_Check();
    DS18B20_Write_Byte(0xCC);
    DS18B20_Write_Byte(0x44);
}

// 初始化 DS18B20
uint8_t DS18B20_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    __HAL_RCC_GPIOA_CLK_ENABLE();

    GPIO_InitStruct.Pin = DS18B20_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(DS18B20_GPIO_Port, &GPIO_InitStruct);

    HAL_GPIO_WritePin(DS18B20_GPIO_Port, DS18B20_Pin, GPIO_PIN_SET);
    DS18B20_Rst();

    return DS18B20_Check();
}

// 获取温度值
short DS18B20_Get_Temp(void)
{
    uint8_t TL, TH;
    short temp;
    DS18B20_Start();
    DS18B20_Rst();
    DS18B20_Check();
    DS18B20_Write_Byte(0xCC);
    DS18B20_Write_Byte(0xBE);
    TL = DS18B20_Read_Byte();
    TH = DS18B20_Read_Byte();

    temp = ((TH << 8) | TL) * 0.625;

    return temp;
}
