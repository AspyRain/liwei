/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdint.h>
#include <rtthread.h>
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
rt_thread_t rt_thread_begin(const char *name,
                             void (*entry)(void *parameter),
                             void       *parameter,
                             rt_uint32_t stack_size,
                             rt_uint8_t  priority,
                             rt_uint32_t tick);

void get_ph(void *prmt);
void get_turbidity(void *prmt);
void oled_menu(void *prmt);
void data_controller(void *prmt);

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define PH_IN0_Pin GPIO_PIN_0
#define PH_IN0_GPIO_Port GPIOA
#define WIFI_TX_Pin GPIO_PIN_2
#define WIFI_TX_GPIO_Port GPIOA
#define WIFI_RX_Pin GPIO_PIN_3
#define WIFI_RX_GPIO_Port GPIOA
#define KEY_Pin GPIO_PIN_4
#define KEY_GPIO_Port GPIOA
#define WIFI_EN_Pin GPIO_PIN_5
#define WIFI_EN_GPIO_Port GPIOA
#define FISH_TX_Pin GPIO_PIN_10
#define FISH_TX_GPIO_Port GPIOB
#define FISH_RX_Pin GPIO_PIN_11
#define FISH_RX_GPIO_Port GPIOB
#define TDS_TX_Pin GPIO_PIN_9
#define TDS_TX_GPIO_Port GPIOA
#define TDS_RX_Pin GPIO_PIN_10
#define TDS_RX_GPIO_Port GPIOA
#define OLED_SCL_Pin GPIO_PIN_6
#define OLED_SCL_GPIO_Port GPIOB
#define OLED_SDA_Pin GPIO_PIN_7
#define OLED_SDA_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
