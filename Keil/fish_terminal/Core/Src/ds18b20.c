#include "ds18b20.h"
#include "rtthread.h"
void delay_us_self(uint32_t time)
{
  time *= 10;
	while(time)
		time--;
}
// 单总线写低
void ds18b20_write_low(void)
{
    HAL_GPIO_WritePin(DS18B20_PORT, DS18B20_PIN, GPIO_PIN_RESET);
    delay_us_self(480); // 持续低电平 480us
    rt_kprintf("Writing LOW to the bus\n");
}

// 单总线写高
void ds18b20_write_high(void)
{
    HAL_GPIO_WritePin(DS18B20_PORT, DS18B20_PIN, GPIO_PIN_SET);
    delay_us_self(60);  // 写高电平 60us
    rt_kprintf("Writing HIGH to the bus\n");
}

// 读取单总线的值
uint8_t ds18b20_read_bit(void)
{
    uint8_t bit_val;
    
    // 写低电平，并延时
    ds18b20_write_low();
    delay_us_self(1);
    
    // 释放总线并延时，等待 DS18B20 响应
    ds18b20_write_high();
    delay_us_self(14);
    
    // 读取引脚值（读取是否有响应）
    bit_val = HAL_GPIO_ReadPin(DS18B20_PORT, DS18B20_PIN);
    delay_us_self(45); // 等待数据稳定
    
    rt_kprintf("Read bit: %d\n", bit_val); // 输出读取的位值
    return bit_val;
}

// 发送一个字节（8位）到单总线
void ds18b20_write_byte(uint8_t byte)
{
    rt_kprintf("Writing byte: 0x%02X\n", byte); // 输出要写入的字节
    for (int i = 0; i < 8; i++)
    {
        if (byte & 0x01)
        {
            ds18b20_write_high(); // 写 1
        }
        else
        {
            ds18b20_write_low(); // 写 0
        }
        byte >>= 1;
    }
}

// 从 DS18B20 读取一个字节
uint8_t ds18b20_read_byte(void)
{
    uint8_t byte = 0;
    rt_kprintf("Reading byte...\n");
    for (int i = 0; i < 8; i++)
    {
        byte >>= 1;
        if (ds18b20_read_bit())
        {
            byte |= 0x80; // 如果读取到 1，则设置高位为 1
        }
    }
    rt_kprintf("Read byte: 0x%02X\n", byte); // 输出读取的字节
    return byte;
}

// 初始化 DS18B20 传感器
void ds18b20_init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    
    // 配置引脚为推挽输出模式
    GPIO_InitStruct.Pin = DS18B20_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(DS18B20_PORT, &GPIO_InitStruct);
    
    // 延时，确保传感器稳定
    rt_thread_mdelay(1000);
    rt_kprintf("DS18B20 sensor initialized.\n");
}

// 读取温度数据
float ds18b20_read_temperature(void)
{
    uint8_t temp_lsb, temp_msb;
    int16_t temp_raw;
    float temperature;
    
    rt_kprintf("Starting temperature read sequence...\n");

    // 发送 RESET 命令
    ds18b20_write_low();
    delay_us_self(480); // 480us
    ds18b20_write_high();
    delay_us_self(80);  // 80us
    rt_kprintf("Sent RESET pulse.\n");

    // 检查是否有设备响应
    if (!ds18b20_read_bit())
    {
        // 设备无响应
        rt_kprintf("No response from DS18B20.\n");
        return -9999.0;
    }
    
    rt_kprintf("Device response received.\n");

    // 发送 CONVERT T 命令（开始转换温度）
    ds18b20_write_byte(0x44);
    delay_us_self(750);  // 转换时间
    rt_kprintf("Sent CONVERT T command, waiting for conversion...\n");

    // 发送 RESET 命令
    ds18b20_write_low();
    delay_us_self(480); // 480us
    ds18b20_write_high();
    delay_us_self(80);  // 80us
    
    // 发送 READ SCRATCHPAD 命令（读取温度数据）
    ds18b20_write_byte(0xBE);
    rt_kprintf("Sent READ SCRATCHPAD command.\n");
    
    // 读取温度的低字节和高字节
    temp_lsb = ds18b20_read_byte();
    temp_msb = ds18b20_read_byte();
    
    rt_kprintf("Received temperature LSB: 0x%02X, MSB: 0x%02X\n", temp_lsb, temp_msb);
    
    // 合并温度值
    temp_raw = (temp_msb << 8) | temp_lsb;
    
    // 转换成温度值（单位：摄氏度）
    temperature = (float)temp_raw / 16.0;
    rt_kprintf("Raw temperature: %d, Converted temperature: %.2f°C\n", temp_raw, temperature);
    
    return temperature;
}
