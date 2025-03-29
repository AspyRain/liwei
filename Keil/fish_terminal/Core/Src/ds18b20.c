#include "ds18b20.h"
#include "tim.h"
#include <rtthread.h>  // 引入 RT-Thread 打印库

#define DS18B20_Set(x) 		HAL_GPIO_WritePin(DS18B20_GPIO_Port, DS18B20_Pin, (GPIO_PinState)(x))
#define DS18B20_Get()	 		HAL_GPIO_ReadPin(DS18B20_GPIO_Port, DS18B20_Pin)

void DS18B20_WriteByte(uint8_t byte);
uint8_t DS18B20_ReadByte(void);


/*****************************************************************************
* 说明：DS18B20的微秒延时函数
* 输入：_usTime时间
* 输出：无
*****************************************************************************/
static void DS18B20_DelayUs(uint16_t _usTime)
{
	__HAL_TIM_SetCounter(&htim3,0);
	__HAL_TIM_ENABLE(&htim3);
	while(__HAL_TIM_GetCounter(&htim3) < _usTime);
	__HAL_TIM_DISABLE(&htim3);
}

/*****************************************************************************
* 说明：复位并检测DS18B20的存在
* 主机和DS18B20做任何通讯前都需要对其初始化。
* 初始化期间，总线控制器拉低总线并保持480us以上挂在总线上的器件将被复位，
* 然后释放总线，等到15-60us，此时18B20将返回一个60-240us之间的低电平存在信号。
* 输入：无
* 输出：1=存在，0=不存在
*****************************************************************************/
uint8_t DS18B20_Reset(void)
{
    uint8_t presence = 0;
    // 拉低总线
    DS18B20_Set(0);
    rt_kprintf("Sending reset signal to DS18B20...\n");
    DS18B20_DelayUs(600);
    
    // 释放总线
    DS18B20_Set(1);
    rt_kprintf("Releasing bus...\n");
    DS18B20_DelayUs(60);
    
    // 检测DS18B20响应
    presence = DS18B20_Get();
    DS18B20_DelayUs(240);
    
    if (presence == GPIO_PIN_SET) {
        rt_kprintf("Presence pulse detected.\n");
    } else {
        rt_kprintf("No presence pulse detected.\n");
    }

    return !presence;
}

/*****************************************************************************
* 说明：复位DS18B20
* 输入：无
* 输出：无
*****************************************************************************/
void DS18B20_Init(void)
{
    rt_kprintf("Initializing DS18B20...\n");
    // 复位DS18B20
    DS18B20_Reset();
    
    // 发送跳过ROM命令
    DS18B20_WriteByte(DS18B20_CMD_SKIP_ROM);
    rt_kprintf("Sent SKIP ROM command.\n");
}



/*****************************************************************************
* 说明：发送一个字节给DS18B20
* 1.总线控制器要产生一个写时序，必须将总线拉低最少1us，
* 2.产生写0时序时总线必须保持低电平60~120us之间，然后释放总线，
* 3.产生写1时序时在总线产生写时序后的15us内允许把总线拉高。
* 4.注意：2次写周期之间至少间隔1us
* 输入：无
* 输出：无
*****************************************************************************/
void DS18B20_WriteByte(uint8_t byte)
{
    rt_kprintf("Writing byte: 0x%02X...\n", byte);
    for (uint8_t i = 0; i < 8; i++)
    {
        // 发送低位
        DS18B20_Set(0);
        DS18B20_DelayUs(2);
        
        // 发送高位，根据byte的第i位来决定
        if (byte & (1 << i))
        {
            DS18B20_Set(1);
        }
        
        DS18B20_DelayUs(60);
        
        // 释放总线
        DS18B20_Set(1);
    }
    rt_kprintf("Byte written: 0x%02X\n", byte);
}

/*****************************************************************************
* 说明：从DS18B20读取一个字节
* 1.总线控制器要产生一个读时序，必须将总线拉低至少1us，
* 2.然后释放总线，在读信号开始后15us内总线控制器采样总线数据，读一位数据至少保持在60us以上。
* 3.注意：2次读周期之间至少间隔1us
* 输入：无
* 输出：无
*****************************************************************************/
uint8_t DS18B20_ReadByte(void)
{
    uint8_t byte = 0;
    
    rt_kprintf("Reading byte...\n");

    for (uint8_t i = 0; i < 8; i++)
    {
        // 发送低位
        DS18B20_Set(0);
        DS18B20_DelayUs(2);
        
        // 释放总线
        DS18B20_Set(1);
        DS18B20_DelayUs(8);
        
        // 读取高位数据
        if (DS18B20_Get())
        {
            byte |= (1 << i);
        }
        
        DS18B20_DelayUs(50);
    }

    rt_kprintf("Byte read: 0x%02X\n", byte);
    return byte;
}

/*****************************************************************************
* 说明：启动DS18B20转换
* 输入：无
* 输出：无
*****************************************************************************/
void DS18B20_StartConv(void)
{
    rt_kprintf("Starting DS18B20 conversion...\n");
	DS18B20_Init();
    // 发送温度转换命令
    DS18B20_WriteByte(DS18B20_CMD_CONVERT_T);
    rt_thread_mdelay(800);  // 延时 800ms，适用于12位精度	
}

/*****************************************************************************
* 说明：读取DS18B20温度
* 1.温度精度及转换时间表
--------------------------------------
R1	R0	精度	最大转换时间	温度LSB
--------------------------------------
0	0	9bit	93.75ms			0.5℃
0	1	10bit	187.5ms			0.25℃
1	0	11bit	375ms			0.125℃
1	1	12bit	750ms			0.0625℃
--------------------------------------
* 输入：无
* 输出：浮点型温度数据
*****************************************************************************/
float DS18B20_GetTemp(void)
{
    rt_kprintf("Getting temperature...\n");
    
    // 复位DS18B20并跳过ROM
    DS18B20_Init();
    
    // 发送读取寄存器命令
    DS18B20_WriteByte(DS18B20_CMD_READ_SCRATCHPAD);
    
    // 读取温度数据
    uint8_t tempLow = DS18B20_ReadByte();
    uint8_t tempHigh = DS18B20_ReadByte();
    
    // 计算温度值
    int16_t temp = (tempHigh << 8) | tempLow;
    float temperature = (float)temp / 16.0f; /* 0.0625℃ */
    
    rt_kprintf("Temperature read: %.2f°C\n", temperature);
    
    return temperature;
}
