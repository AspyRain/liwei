#include "tds.h"
#include "systick.h"
#include <rtthread.h>  // 引入 RT-Thread 打印库

static void DS18B20_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    __HAL_RCC_GPIOA_CLK_ENABLE();  // 开启 GPIOA 时钟
    rt_kprintf("GPIOA clock enabled.\n");

    GPIO_InitStruct.Pin = DS18B20_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;  // 推挽输出
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(DS18B20_PORT, &GPIO_InitStruct);

    HAL_GPIO_WritePin(DS18B20_PORT, DS18B20_PIN, GPIO_PIN_SET);  // DS18B20 数据引脚初始化为高电平输出
    rt_kprintf("DS18B20 GPIO initialized and set to high.\n");
}

static void DS18B20_Mode_Input(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    GPIO_InitStruct.Pin = DS18B20_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(DS18B20_PORT, &GPIO_InitStruct);
    rt_kprintf("DS18B20 set to input mode.\n");
}

static void DS18B20_Mode_Output(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    GPIO_InitStruct.Pin = DS18B20_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;  // 推挽输出
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(DS18B20_PORT, &GPIO_InitStruct);
    rt_kprintf("DS18B20 set to output mode.\n");
}

static void DS18B20_Rst(void)
{
    DS18B20_Mode_Output();
    rt_kprintf("Sending reset signal to DS18B20.\n");
    HAL_GPIO_WritePin(DS18B20_PORT, DS18B20_PIN, GPIO_PIN_RESET);  // 主机产生复位脉冲
    delay_us(750);  // 至少 480us
    HAL_GPIO_WritePin(DS18B20_PORT, DS18B20_PIN, GPIO_PIN_SET);  // 将总线拉高
    rt_kprintf("Reset signal sent. Waiting for response...\n");
    delay_us(15);   // 等待从机响应
}

static uint8_t DS18B20_Presence(void)
{
    uint8_t pulse_time = 0;

    DS18B20_Mode_Input();  // 主机设置为上拉输入
    rt_kprintf("Waiting for DS18B20 presence pulse...\n");

    // 等待存在脉冲的到来，存在脉冲为一个 60~240us 的低电平信号
    while (HAL_GPIO_ReadPin(DS18B20_PORT, DS18B20_PIN) && pulse_time < 100)
    {
        pulse_time++;
        delay_us(1);
    }

    if (pulse_time >= 100)
    {
        rt_kprintf("No presence pulse detected.\n");
        return 1;  // 超时未检测到存在脉冲
    }

    pulse_time = 0;
    while (!HAL_GPIO_ReadPin(DS18B20_PORT, DS18B20_PIN) && pulse_time < 240)  // 存在脉冲的持续时间不能超过 240us
    {
        pulse_time++;
        delay_us(1);
    }

    if (pulse_time >= 240)
    {
        rt_kprintf("Presence pulse too long.\n");
        return 1;  // 存在脉冲时间过长
    }

    rt_kprintf("Presence pulse received.\n");
    return 0;
}

static uint8_t DS18B20_Read_Bit(void)
{
    uint8_t dat = 0;

    DS18B20_Mode_Output();
    HAL_GPIO_WritePin(DS18B20_PORT, DS18B20_PIN, GPIO_PIN_RESET);  // 产生 1us 的低电平信号
    rt_kprintf("Sending bit read signal...\n");
    delay_us(10);

    DS18B20_Mode_Input();
    delay_us(10);  // 等待数据稳定

    if (HAL_GPIO_ReadPin(DS18B20_PORT, DS18B20_PIN) == GPIO_PIN_SET)
        dat = 1;
    delay_us(50);

    rt_kprintf("Read bit: %d\n", dat);
    return dat;
}

static uint8_t DS18B20_Read_Byte(void)
{
    uint8_t i, dat = 0;

    rt_kprintf("Reading byte...\n");
    for (i = 0; i < 8; i++)
    {
        dat |= (DS18B20_Read_Bit() << i);  // 逐位读取数据
    }

    rt_kprintf("Read byte: 0x%02X\n", dat);
    return dat;
}

void DS18B20_Write_Byte(uint8_t dat)
{
    uint8_t i, testb;
    DS18B20_Mode_Output();

    rt_kprintf("Writing byte: 0x%02X\n", dat);
    for (i = 0; i < 8; i++)
    {
        testb = dat & 0x01;
        dat = dat >> 1;

        // 写 0 和写 1 的时间至少要大于 60us
        if (testb)
        {
            HAL_GPIO_WritePin(DS18B20_PORT, DS18B20_PIN, GPIO_PIN_RESET);
            delay_us(8);  // 1us < 延时 < 15us

            HAL_GPIO_WritePin(DS18B20_PORT, DS18B20_PIN, GPIO_PIN_SET);
            delay_us(58);  // 58us + 8us > 60us
        }
        else
        {
            HAL_GPIO_WritePin(DS18B20_PORT, DS18B20_PIN, GPIO_PIN_RESET);
            delay_us(70);  // 60us < 延时 < 120us

            HAL_GPIO_WritePin(DS18B20_PORT, DS18B20_PIN, GPIO_PIN_SET);
            delay_us(2);   // 1us < 恢复时间 < 无限大
        }
    }
}

void DS18B20_Start(void)
{
    DS18B20_Rst();
    DS18B20_Presence();
    DS18B20_Write_Byte(0xCC);  // 跳过 ROM
    DS18B20_Write_Byte(0x44);  // 开始转换
}

uint8_t DS18B20_Init(void)
{
    DS18B20_GPIO_Config();
    DS18B20_Rst();

    if (DS18B20_Presence() == 0)
    {
        rt_kprintf("DS18B20 initialized successfully.\n");
    }
    else
    {
        rt_kprintf("DS18B20 initialization failed.\n");
    }

    return DS18B20_Presence();
}

// 获取DS18B20的温度，返回float类型
float DS18B20_Get_Temp(void)
{
    uint8_t TL, TH;
    float temp;
    DS18B20_Start();           // 启动转换
    DS18B20_Rst();
    DS18B20_Check();
    DS18B20_Write_Byte(0xcc);  // Skip ROM
    DS18B20_Write_Byte(0xbe);  // 读取温度
    TL = DS18B20_Read_Byte();  // LSB
    TH = DS18B20_Read_Byte();  // MSB

    if (TH > 7)  // 负数处理
    {
        TH = ~TH;
        TL = ~TL;
        temp = 0;  // 温度为负数
    }
    else
    {
        temp = 1;  // 温度为正数
    }

    temp = TH;
    temp <<= 8;
    temp += TL;
    temp = (float)temp * 0.625;  // 转换为温度值
    return temp;  // 返回温度值（float）
}
