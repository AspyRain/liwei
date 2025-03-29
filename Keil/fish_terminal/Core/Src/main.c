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
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <rtthread.h>
#include "esp01s.h"
#include <stdint.h>
#include "tds.h"
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
float compensationCoefficient, compensationVolatge, TDS_value;
float kValue = 1.0;  // 传感器校准系�??
char  TEMP_Buff[5];   //温度存放数组
char  TDS_Buff[6];   //TDS存放数组
float TEMP_Value=0.0;
float PH_Value=0.0;
float TDS_Value=0.0;
float SIZE_Value=0.0;
float SPEED_Value=0.0;
char *realCommand;

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
  /* USER CODE BEGIN 2 */
	
	HAL_UART_Receive_IT(&huart2, (uint8_t *)&usart2_c, 1);
  // rt_thread_t ph_task = rt_thread_begin("ph_task", 
  //                                            get_ph, RT_NULL,
  //                                            1024, 3, 10);

  rt_thread_t turbidity_task = rt_thread_begin("turbidity_task", 
                                           get_turbidity, RT_NULL,
                                           1024, 3, 10);
                    
  rt_thread_t oled_task = rt_thread_begin("oled_task", 
                                           oled_menu, RT_NULL,
                                           1024, 3, 10);
  
  rt_thread_t data_controller_task = rt_thread_begin("data_controller_task", 
                                            data_controller, RT_NULL,
                                            1024, 3, 10);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV2;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
rt_thread_t rt_thread_begin(const char *name,
                             void (*entry)(void *parameter),
                             void       *parameter,
                             rt_uint32_t stack_size,
                             rt_uint8_t  priority,
                             rt_uint32_t tick){
  rt_thread_t task = rt_thread_create(name,
                                              entry, RT_NULL,
                                              stack_size, priority, tick); //
  if (task != RT_NULL)
  {
    rt_thread_startup(task);
    rt_kprintf("%s thread is already started\n",name);
    return task;
  }
  else
  {
    rt_kprintf("%s thread is not started\n",name);
    return RT_NULL;
  }

}
//获取ph�?
void get_ph(void *prmt){
	while (1)
  {
    rt_kprintf("正在运行:getph\n");
    rt_thread_mdelay(1000);
  }
  
}

//获取浑浊�?/温度
void get_turbidity(void *prmt){
  DS18B20_Init();
	while (1)
  {
		TEMP_Value=DS18B20_Get_Temp();
	  TEMP_Buff[0]=(int)(TEMP_Value)%1000/100+'0';	
	  TEMP_Buff[1]=(int)(TEMP_Value)%100/10+'0';
	  TEMP_Buff[2]='.';
	  TEMP_Buff[3]=(int)(TEMP_Value)%10+'0';
    rt_kprintf("正在运行:getturbidity\n");
    rt_thread_mdelay(10000);
  }
}

//oled显示
void oled_menu(void *prmt){
  OLED_Init();
  OLED_Clear();
  OLED_ShowStart();

  rt_thread_mdelay(1000);
  OLED_Clear();
  OLED_ShowCHinese( index[0][0].x, index[0][0].y, 10);//�?
  OLED_ShowCHinese( index[0][1].x, index[0][1].y, 13);//�?
  OLED_ShowChar(    index[0][2].x, index[0][2].y, ':', 16);

  OLED_ShowCHinese(index[1][0].x, index[1][0].y, 11);//�?
  OLED_ShowCHinese(index[1][1].x, index[1][1].y, 12);//�?
  OLED_ShowCHinese(index[1][2].x, index[1][2].y, 13);//�?
  OLED_ShowChar(    index[1][3].x, index[1][3].y, ':', 16);

  OLED_ShowString(index[2][0].x,index[2][0].y,"PH",16);//PH
  OLED_ShowChar(    index[2][1].x, index[2][1].y, ':', 16);

  OLED_ShowCHinese( index[3][0].x, index[3][0].y, 14);//�?
  OLED_ShowCHinese( index[3][1].x, index[3][1].y, 15);//度寸


  OLED_ShowCHinese( index[3][4].x, index[3][4].y, 0);//�?
  OLED_ShowCHinese( index[3][5].x, index[3][5].y, 16);//�?


	while (1)
  {

    char tempStr[10]; // 用来存放格式化后的字符串
    // 格式化浮点数，保留一位小�?
    sprintf(tempStr, "%.1f", TEMP_Value);
    // 显示字符�?
    OLED_ShowString(index[0][3].x, index[0][3].y, tempStr, 16); // 假设显示�? (0, 0) 位置，使�?16号字�?

    char TDSStr[10]; // 用来存放格式化后的字符串
    // 格式化浮点数，保留一位小�?
    sprintf(TDSStr, "%.1f", TDS_Value);
    // 显示字符�?
    OLED_ShowString(index[1][4].x, index[1][4].y, TDSStr, 16); // 假设显示�? (0, 0) 位置，使�?16号字�?



    char phStr[10]; // 用来存放格式化后的字符串
    // 格式化浮点数，保留一位小�?
    sprintf(phStr, "%.1f", PH_Value);
    // 显示字符�?
    OLED_ShowString(index[2][2].x, index[2][2].y, phStr, 16); // 假设显示�? (0, 0) 位置，使�?16号字�?

    char SIZEStr[10]; // 用来存放格式化后的字符串
    // 格式化浮点数，保留一位小�?
    sprintf(SIZEStr, "%.1f", SIZE_Value);
    // 显示字符�?
    OLED_ShowString(index[3][2].x, index[3][2].y, SIZEStr, 15); // 假设显示�? (0, 0) 位置，使�?16号字�?

    char SPEEDStr[10]; // 用来存放格式化后的字符串
    // 格式化浮点数，保留一位小�?
    sprintf(SPEEDStr, "%.1f", SPEED_Value);
    // 显示字符�?
    OLED_ShowString(index[3][6].x, index[3][6].y, SPEEDStr, 15); // 假设显示�? (0, 0) 位置，使�?16号字�?

    rt_kprintf("正在运行:oled\n");
    rt_thread_mdelay(1000);
  }
}
//数据控制- wifi通信
void data_controller(void *prmt){
  // wifi初始�???
	Esp01s_Init("AspyRain", "[FrommetoU]", "192.168.70.191",8888);
	while (esp_flag)
  {
    char buffer[100];  // 预留足够的空间存放字符串

    // 格式化字符串，保留一位小�?
    sprintf(buffer, "data:%.1f,%.1f,%.1f,%.1f,%.1f\n", 
            TEMP_Value, PH_Value, TDS_Value, SIZE_Value, SPEED_Value);

    // 打印结果
    rt_kprintf("发�?�数�?%s\n", buffer);
    espSend(buffer,0);
		rt_kprintf("正在运行:wifi通信\n");
    rt_thread_mdelay(2000);
	}
    
}

void TDS_Value_Conversion()
{
    // 1️⃣ 读取 ADC 电压（HAL 方式�??
    HAL_ADC_Start(&hadc2);
    HAL_ADC_PollForConversion(&hadc2, 10);
    ADC_ConvertedValue[0] = HAL_ADC_GetValue(&hadc2);
    HAL_ADC_Stop(&hadc2);

    ADC_ConvertedValueLocal[0] = (float)ADC_ConvertedValue[0] / 4096 * 3.3; // AD 转换

    // 2️⃣ 读取温度（如果使用内部温度传感器，可�?? ADC 采集�??
    float TEMP_Value = 25.0; // 默认 25°C，如有温度传感器可替�??

    // 3️⃣ 计算温度补偿系数
    compensationCoefficient = 1.0 + 0.02 * (TEMP_Value - 25.0);
    compensationVolatge = ADC_ConvertedValueLocal[0] / compensationCoefficient;

    // 4️⃣ TDS 计算
    if (ADC_ConvertedValueLocal[0] >= 0 && ADC_ConvertedValueLocal[0] < 0.1)
    {
        compensationVolatge = 0;
    }

    TDS_value = (133.42 * compensationVolatge * compensationVolatge * compensationVolatge
                 - 255.86 * compensationVolatge * compensationVolatge
                 + 857.39 * compensationVolatge) * 0.5 * kValue;

    // 5️⃣ 限制 TDS 范围
    if (TDS_value <= 0) { TDS_value = 0; }
    if (TDS_value > 1400) { TDS_value = 1400; }

    // 6️⃣ 格式化输�??
    TDS_Buff[0] = (int)(TDS_value) / 1000 + '0';
    TDS_Buff[1] = (int)(TDS_value) % 1000 / 100 + '0';
    TDS_Buff[2] = (int)(TDS_value * 100) % 100 / 10 + '0';
    TDS_Buff[3] = (int)(TDS_value * 100) % 10 + '0';
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
    if (usart2_c == '\n'){
      rt_kprintf("%s",usart2_rx_buffer);
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
  if (htim->Instance == TIM1) {
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

#ifdef  USE_FULL_ASSERT
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
