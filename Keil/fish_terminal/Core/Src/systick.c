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
void Delay_us(uint32_t us)
{
    uint32_t start = DWT->CYCCNT;  // 获取当前 CPU 周期计数
    uint32_t ticks = (SystemCoreClock / 1000000) * us; // 计算微秒对应的时钟周期数

    while ((DWT->CYCCNT - start) < ticks); // 轮询等待
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
