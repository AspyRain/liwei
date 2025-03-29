#include "systick.h"
#include "stm32f1xx_hal.h"

static __IO uint32_t TimingDelay;

/**
  * @brief  初始化 `SysTick` 定时器
  * @note   HAL 库默认已经初始化 `SysTick`，不需要手动配置
  * @retval 无
  */
void SysTick_Init(void)
{
    HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK_DIV8);  // 选择时钟源 (HCLK/8)
}

/**
  * @brief  微秒级延时
  * @param  us: 延时时间 (微秒)
  * @retval 无
  */
 #define CPU_FREQUENCY_MHZ    72		// STM32时钟主频
 void delay_us(__IO uint32_t delay)
 {
     int last, curr, val;
     int temp;
 
     while (delay != 0)
     {
         temp = delay > 900 ? 900 : delay;
         last = SysTick->VAL;
         curr = last - CPU_FREQUENCY_MHZ * temp;
         if (curr >= 0)
         {
             do
             {
                 val = SysTick->VAL;
             }
             while ((val < last) && (val >= curr));
         }
         else
         {
             curr += CPU_FREQUENCY_MHZ * 1000;
             do
             {
                 val = SysTick->VAL;
             }
             while ((val <= last) || (val > curr));
         }
         delay -= temp;
     }
 }
 
 

/**
  * @brief  毫秒级延时
  * @param  ms: 延时时间 (毫秒)
  * @retval 无
  */
void Delay_ms(uint32_t ms)
{
    HAL_Delay(ms);  // 直接使用 HAL 的延时函数
}

/**
  * @brief  计时器减少（在 `SysTick_Handler` 中调用）
  * @retval 无
  */
void TimingDelay_Decrement(void)
{
    if (TimingDelay != 0)
    {
        TimingDelay--;
    }
}
