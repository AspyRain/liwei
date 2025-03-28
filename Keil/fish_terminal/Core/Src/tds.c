#include "tds.h"
#include "systick.h"
static void DS18B20_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    DS18B20_CLK_ENABLE();

    GPIO_InitStruct.Pin = DS18B20_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(DS18B20_PORT, &GPIO_InitStruct);

    DS18B20_DATA_OUT(GPIO_PIN_SET);
}

static void DS18B20_Mode_Input(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = DS18B20_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(DS18B20_PORT, &GPIO_InitStruct);
}

static void DS18B20_Mode_Output(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = DS18B20_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(DS18B20_PORT, &GPIO_InitStruct);
}

static void DS18B20_Reset(void)
{
    DS18B20_Mode_Output();
    DS18B20_DATA_OUT(GPIO_PIN_RESET);
    Delay_us(750); // 至少 480us
    DS18B20_DATA_OUT(GPIO_PIN_SET);
    Delay_us(15); // 释放总线等待响应
}

static uint8_t DS18B20_Presence(void)
{
    uint8_t pulse_time = 0;
    DS18B20_Mode_Input();

    while (DS18B20_DATA_IN() && pulse_time < 100)
    {
        pulse_time++;
        Delay_us(1);
    }
    if (pulse_time >= 100) return 1;

    pulse_time = 0;
    while (!DS18B20_DATA_IN() && pulse_time < 240)
    {
        pulse_time++;
        Delay_us(1);
    }
    return (pulse_time >= 240) ? 1 : 0;
}

static uint8_t DS18B20_Read_Bit(void)
{
    uint8_t dat = 0;
    DS18B20_Mode_Output();
    DS18B20_DATA_OUT(GPIO_PIN_RESET);
    Delay_us(2);
    
    DS18B20_Mode_Input();
    Delay_us(10);

    if (DS18B20_DATA_IN())
        dat = 1;
    Delay_us(50);

    return dat;
}

static uint8_t DS18B20_Read_Byte(void)
{
    uint8_t i, dat = 0;
    for (i = 0; i < 8; i++)
    {
        dat |= (DS18B20_Read_Bit() << i);
    }
    return dat;
}

static void DS18B20_Write_Byte(uint8_t dat)
{
    uint8_t i;
    DS18B20_Mode_Output();
    for (i = 0; i < 8; i++)
    {
        DS18B20_DATA_OUT(GPIO_PIN_RESET);
        Delay_us(2);

        if (dat & 0x01)
            DS18B20_DATA_OUT(GPIO_PIN_SET);
        else
            DS18B20_DATA_OUT(GPIO_PIN_RESET);

        Delay_us(60);
        DS18B20_DATA_OUT(GPIO_PIN_SET);
        dat >>= 1;
    }
}

void DS18B20_Init(void)
{
    DS18B20_GPIO_Config();
    DS18B20_Reset();
    DS18B20_Presence();
}

float DS18B20_Get_Temp(void)
{
    uint8_t tpmsb, tplsb;
    short s_tem;
    float f_tem;

    DS18B20_Reset();
    DS18B20_Presence();
    DS18B20_Write_Byte(0xCC); // 跳过 ROM
    DS18B20_Write_Byte(0x44); // 开始转换

    rt_thread_mdelay(750);

    DS18B20_Reset();
    DS18B20_Presence();
    DS18B20_Write_Byte(0xCC); // 跳过 ROM
    DS18B20_Write_Byte(0xBE); // 读取温度值

    tplsb = DS18B20_Read_Byte();
    tpmsb = DS18B20_Read_Byte();

    s_tem = (tpmsb << 8) | tplsb;
    f_tem = s_tem * 0.0625f;

    return f_tem;
}
