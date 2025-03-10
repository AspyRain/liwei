#include "esp01s.h"
#include "usart.h"
#include "rtthread.h"
#include "stm32f1xx_hal.h"
#include "stdlib.h"
#include "main.h"
#include "global.h"
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#define WAIT_TIMEOUT_MS 5500
void Esp01s_Init(char *ip, char *password, char *port)
{
  char command[50];
  // 发送初始化指令到ESP01S
  // status = "AT+RST";
  // sendData(&huart1, "AT+RST\r\n");
  // while (AT_OK_Flag == 0)
  // {
  //   rt_thread_mdelay(50);
  // }
  // AT_OK_Flag = 0;
  // rt_thread_mdelay(1000);
  status = "AT+CWMODE=1";
  sendData(&huart1, "AT+CWMODE=1\r\n");
  waitCommand("OK",10000);
  status = "AT+CWJAP";
  sprintf(command, "AT+CWJAP=\"%s\",\"%s\"\r\n", ip, password);
  sendData(&huart1, command);
  waitCommand("OK",10000);
  waitCommand("Connect_Net",10000);
  AT_Connect_Net_Flag = 0;
  status = "AT+CIPMUX=1";
  sprintf(command, "AT+CIPMUX=1\r\n");
  sendData(&huart1, command);
  waitCommand("OK",10000);
  status = "AT+CIPSERVER=1";
  sprintf(command, "AT+CIPSERVER=1,%s\r\n", port);
  sendData(&huart1, command);
  waitCommand("OK",10000);
  esp_flag = 1;
  HAL_GPIO_WritePin(WIFI_EN_GPIO_Port,WIFI_EN_Pin, GPIO_PIN_SET);
  // 清空接收缓冲区
  huart1.Instance->DR; // 读取数据寄存器，将接收缓冲区中的数据清空
  // 重新启用USART1接收中断
  rt_kprintf("初始化完成\n");
}
void waitCommand(char *command, uint32_t timeout_ms)
{
  uint32_t start_time = rt_tick_get();
  
  if (strcmp(command, "OK") == 0)
  {
    while (AT_OK_Flag == 0)
    {
      rt_thread_mdelay(50);
      
      // 检查是否超过了超时时间
      if ((rt_tick_get() - start_time) > timeout_ms)
      {
        rt_kprintf("等待 OK 超时\n");
        break;
      }
    }
    AT_OK_Flag = 0;
  }
  else if (strcmp(command, "Connect_Net") == 0)
  {
    while (AT_Connect_Net_Flag == 0)
    {
      rt_thread_mdelay(50);
      
      // 检查是否超过了超时时间
      if ((rt_tick_get() - start_time) > timeout_ms)
      {
        rt_kprintf("等待 Connect_Net 超时\n");
        break;
      }
    }
    AT_Connect_Net_Flag = 0;
  }
  else if (strcmp(command, "Ready") == 0)
  {
    while (AT_Ready_Flag == 0)
    {
      rt_thread_mdelay(50);
      
      // 检查是否超过了超时时间
      if ((rt_tick_get() - start_time) > timeout_ms)
      {
        rt_kprintf("等待 Ready 超时\n");
        break;
      }
    }
    AT_Ready_Flag = 0;
  }
}

void espSend(char *message, int enterFlag)
{
  char command[50];
  if (enterFlag == 1)
  {
    addNewlineIfNeeded(message);
  }
  rt_kprintf("message:%s\n", message);
  sprintf(command, "AT+CIPSEND=0,%d\r\n", strlen(message));
  rt_kprintf("esp函数执行send\n");
  sendData(&huart1, command);
  waitCommand("OK",10000);
  AT_OK_Flag = 0;
  rt_kprintf("发送数据\n");
  sendData(&huart1, message);
  waitCommand("OK",10000);
  rt_kprintf("发送完成\n");
}

void addNewlineIfNeeded(char *message)
{
  // 获取字符串长度
  size_t len = strlen(message);

  // 判断末尾是否为 '\n'
  if (len > 0 && message[len - 1] != '\n')
  {
    // 在末尾添加 '\n'
    strcat(message, "\n");
  }
}
