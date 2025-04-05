/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2025 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <rtthread.h>
#include "esp01s.h"
#include <stdint.h>
#include "ds18b20.h"
#include "global.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "oled.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
extern ADC_HandleTypeDef hadc2; // 确保 ADC 句柄全局可用
char usart2_c;
int dataLen;
char dataLenStr[dataMaxLen];
float ADC_ConvertedValueLocal[3] = {0};
uint16_t ADC_ConvertedValue[3] = {0};
float compensationCoefficient, compensationVolatge;
float kValue = 1.0; // 传感器校准系�????
char TEMP_Buff[5];  // 温度存放数组
char TDS_Buff[6];   // TDS存放数组
float TEMP_Value = 0.0;
float PH_Value = 0.0;
float TDS_Value = 0.0;
char *realCommand;
int pub_tick = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_ADC1_Init();
  MX_I2C1_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  MX_USART3_UART_Init();
  MX_ADC2_Init();
  MX_TIM3_Init();
  /* USER CODE BEGIN 2 */

  HAL_UART_Receive_IT(&huart2, (uint8_t *)&usart2_c, 1);
  HAL_TIM_Base_Start(&htim3);

  rt_thread_t Water_quality_testing = rt_thread_begin("Water_quality_testing",
                                                      testing_water, RT_NULL,
                                                      1024, 3, 10);

  rt_thread_t oled_task = rt_thread_begin("oled_task",
                                          oled_menu, RT_NULL,
                                          1024, 3, 10);

  rt_thread_t data_controller_task = rt_thread_begin("data_controller_task",
                                            data_controller, RT_NULL,
                                            1024, 3, 10);
 
  while (esp_flag == 0)
  {
    rt_thread_mdelay(20);
  }

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    rt_thread_mdelay(50);
    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
   * in the RCC_OscInitTypeDef structure.
   */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL6;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
   */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV4;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
rt_thread_t rt_thread_begin(const char *name,
                            void (*entry)(void *parameter),
                            void *parameter,
                            rt_uint32_t stack_size,
                            rt_uint8_t priority,
                            rt_uint32_t tick)
{
  rt_thread_t task = rt_thread_create(name,
                                      entry, RT_NULL,
                                      stack_size, priority, tick); //
  if (task != RT_NULL)
  {
    rt_thread_startup(task);
    rt_kprintf("%s thread is already started\n", name);
    return task;
  }
  else
  {
    rt_kprintf("%s thread is not started\n", name);
    return RT_NULL;
  }
}

// 获取浑浊�???/温度
void testing_water(void *prmt)
{
  DS18B20_Init();
  while (esp_flag == 0)
  {
    rt_thread_mdelay(50);
  }
  while (1)
  {
    rt_kprintf("正在运行:getturbidity\n");
    TEMP_Value = (float)DS18B20_Get_Temp() / 10;
    TEMP_Buff[0] = (int)(TEMP_Value) % 1000 / 100 + '0';
    TEMP_Buff[1] = (int)(TEMP_Value) % 100 / 10 + '0';
    TEMP_Buff[2] = '.';
    TEMP_Buff[3] = (int)(TEMP_Value) % 10 + '0';

    TDS_Value_Conversion(TEMP_Value);
    PH_Value = Get_pH_Value();
    rt_kprintf("tds: %d\ntemp:%d", (int)TDS_Value, (int)TEMP_Value);
    rt_thread_mdelay(1000);
  }
}
// 获取 pH 值的函数
float Get_pH_Value(void)
{
  // 1. 读取 ADC 数据
  HAL_ADC_Start(&hadc1);                            // 开始 ADC 转换
  HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY); // 等待转换完成
  uint32_t adc_value = HAL_ADC_GetValue(&hadc1);    // 获取 ADC 转换结果
  HAL_ADC_Stop(&hadc1);
  // 2. 计算电压 (假设 Vref = 3.3V, 12-bit ADC)
  float voltage = (adc_value / 4095.0) * 3.3;
  rt_kprintf("电压大小: %d\n", (int)(voltage * 1000));
  // 3. 计算 pH 值 (pH 7 对应 2.5V, 斜率 59.16 mV/pH)
  float pH = (voltage - 2.5) / 0.05916 + 7.0;
  // 5. 进行温度补偿 (温度漂移系数 -0.03 pH/°C)
  float pH_T = pH + (TEMP_Value - 25.0) * (-0.03);
  return pH_T;
}
// oled显示
void oled_menu(void *prmt)
{
  OLED_Init();
  OLED_Clear();
  OLED_ShowStart();

  while (esp_flag == 0)
  {
    rt_thread_mdelay(50);
  }

  OLED_Clear();
  OLED_ShowCHinese(index[0][0].x, index[0][0].y, 10); // 温
  OLED_ShowCHinese(index[0][1].x, index[0][1].y, 13); // 度
  OLED_ShowChar(index[0][2].x, index[0][2].y, ':', 16);

  OLED_ShowCHinese(index[1][0].x, index[1][0].y, 11); // 浑
  OLED_ShowCHinese(index[1][1].x, index[1][1].y, 12); // 浊
  OLED_ShowCHinese(index[1][2].x, index[1][2].y, 13); // 度
  OLED_ShowChar(index[1][3].x, index[1][3].y, ':', 16);

  OLED_ShowString(index[2][0].x, index[2][0].y, "PH", 16); // PH
  OLED_ShowChar(index[2][1].x, index[2][1].y, ':', 16);


  while (1)
  {
      {
          char tempStr[5]; // 用来存放格式化后的字符串
          format_string(tempStr, TEMP_Value,5);
          OLED_ShowString(index[0][3].x, index[0][3].y, tempStr, 16); // 假设显示 (0, 0) 位置，使 16 号字
      }
  
      {
          char TDSStr[6]; // 用来存放格式化后的字符串
          format_string(TDSStr, TDS_Value,6);
          OLED_ShowString(index[1][4].x, index[1][4].y, TDSStr, 16); // 假设显示 (0, 0) 位置，使 16 号字
      }
  
      {
          char phStr[5]; // 用来存放格式化后的字符串
          format_string(phStr, PH_Value,5);
          OLED_ShowString(index[2][2].x, index[2][2].y, phStr, 16); // 假设显示 (0, 0) 位置，使 16 号字
      }
  
  
      rt_thread_mdelay(1000);
  }
}
// 数据控制- wifi通信
void data_controller(void *prmt)
{
  // wifi初始化
  Esp01s_Init("504", "abcd761124", "119.29.243.196", 8887);
  while (esp_flag)
  {
    char buffer[100]; // 预留足够的空间存放字符串

    // 格式化字符串，保留一位小数
    sprintf(buffer, "data:%.1f,%.1f,%.1f\n",
            TEMP_Value, PH_Value, TDS_Value);

    // 打印结果
    rt_kprintf("发送数据:%s\n", buffer);
    espSend(buffer, 0);
    rt_kprintf("正在运行:wifi通信\n");
    rt_thread_mdelay(30000);
  }
}

void TDS_Value_Conversion()
{
  // 1️⃣ 读取 ADC 电压（HAL 方式�????
  HAL_ADC_Start(&hadc2);
  HAL_ADC_PollForConversion(&hadc2, 10);
  ADC_ConvertedValue[0] = HAL_ADC_GetValue(&hadc2);
  HAL_ADC_Stop(&hadc2);

  ADC_ConvertedValueLocal[0] = (float)ADC_ConvertedValue[0] / 4096 * 3.3; // AD 转换

  // 3️⃣ 计算温度补偿系数
  compensationCoefficient = 1.0 + 0.02 * (TEMP_Value - 25.0);
  compensationVolatge = ADC_ConvertedValueLocal[0] / compensationCoefficient;

  // 4️⃣ TDS 计算
  if (ADC_ConvertedValueLocal[0] >= 0 && ADC_ConvertedValueLocal[0] < 0.1)
  {
    compensationVolatge = 0;
  }

  TDS_Value = (133.42 * compensationVolatge * compensationVolatge * compensationVolatge - 255.86 * compensationVolatge * compensationVolatge + 857.39 * compensationVolatge) * 0.5 * kValue;

  // 5️⃣ 限制 TDS 范围
  if (TDS_Value <= 0)
  {
    TDS_Value = 0;
  }
  if (TDS_Value > 1400)
  {
    TDS_Value = 1400;
  }
}

void clearUsart()
{
  memset(usart2_rx_buffer, 0, sizeof(usart2_rx_buffer));
  usart2_rx_index = 0;
}

void reciveData()
{
  if (usart2_rx_index == -1)
    usart2_rx_index++;
  else
  {
    switch (esp_command_flag)
    {
    case 1:

      if (usart2_rx_index < dataMaxLen)
      {
        dataLenStr[usart2_rx_index++] = usart2_c;
      }
      else
      {
        rt_kprintf("data too long!\n");
      }
      break;
    case 2:

      if (usart2_rx_index < dataLen)
      {
        realCommand[usart2_rx_index++] = usart2_c;
      }
      else
      {
        rt_kprintf("data error!\n");
      }
      break;
    default:
      usart2_rx_buffer[usart2_rx_index++] = usart2_c;
      break;
    }
  }
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  if (huart == &huart2)
  {
    if (esp_command_flag == 3)
      esp_command_flag = 0;
    if (usart2_rx_index >= BUFFER_SIZE - 1)
    {
      clearUsart();
    }
    if (usart2_c == '\n')
    {
      rt_kprintf("%s", usart2_rx_buffer);
    }
    // if (esp_flag == 1)
    // {
    //   if (usart2_c == '+')
    //   {
    //     clearUsart();
    //   }
    //   else if (strstr((const char *)usart2_rx_buffer, "+IPD,0,") != NULL) // ???????,????????
    //   {
    //     // ???????
    //     esp_command_flag = 1;
    //     // ???????,?????????
    //     clearUsart();
    //   }
    //   else if (esp_command_flag == 1 && usart2_c == ':') // ??????
    //   {
    //     esp_command_flag = 2;
    //     dataLen = atoi(dataLenStr);
    //     usart2_rx_index = -1;
    //     memset(dataLenStr, 0, sizeof(dataLenStr));
    //     realCommand = (char *)malloc((dataLen * sizeof(char)) + 1);
    //     realCommand[dataLen] = '\0';
    //   }
    //}
    reciveData();
    // ?????????
    HAL_UART_Receive_IT(&huart2, (uint8_t *)&usart2_c, 1);
  }
}
/* USER CODE END 4 */

/**
 * @brief  Period elapsed callback in non blocking mode
 * @note   This function is called  when TIM1 interrupt took place, inside
 * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
 * a global variable "uwTick" used as application time base.
 * @param  htim : TIM handle
 * @retval None
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM1)
  {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
