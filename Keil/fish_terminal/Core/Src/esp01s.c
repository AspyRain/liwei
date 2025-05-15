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
void Esp01s_Init(char *ip, char *password, char *server,int port)
{
  char command[50];
  // 发送初始化指令到ESP01S
  sendData(&huart2, "AT+RST\r\n");
  rt_thread_mdelay(2000);
  
  sendData(&huart2, "AT+CWMODE=1\r\n");
  rt_thread_mdelay(2000);   
  sprintf(command, "AT+CWJAP=\"%s\",\"%s\"\r\n", ip, password);
  sendData(&huart2, command);
  rt_thread_mdelay(5000);
  sprintf(command, "AT+CIPSTART=\"TCP\",\"%s\",%d\r\n",server,port);
  sendData(&huart2, command);
  rt_thread_mdelay(2000);
  sendData(&huart2, "AT+CIPMODE=1\r\n");
  rt_thread_mdelay(1000);
  sendData(&huart2, "AT+CIPSEND\r\n");
  rt_thread_mdelay(1000);
  esp_flag = 1;
  // 清空接收缓冲区
  huart2.Instance->DR; // 读取数据寄存器，将接收缓冲区中的数据清空
  // 重新启用USART1接收中断
  rt_kprintf("初始化完成\n");
}

// 生成数据的函数
void generateDataString(char *buffer, float temperature, float humidity, float brightness, float co)
{
    // 格式化数据并生成 JSON 格式字符串
    sprintf(buffer, "{\"temperature\": %.2f, \"humidity\": %.2f, \"brightness\": %.2f, \"co\": %.2f}",
            temperature, humidity, brightness, co);
}

// 生成完整的 HTTP POST 请求字符串
void generateHttpPostRequest(char *buffer, const char *ip, float temperature, float humidity, float brightness, float co)
{
    char dataString[100];
    generateDataString(dataString, temperature, humidity, brightness, co);

    // 请求头部分
    sprintf(buffer, 
            "POST /soil/save HTTP/1.1\r\n"
            "Host: %s:8080\r\n"
            "Content-Type: application/json\r\n"
            "Content-Length: %lu\r\n"
            "\r\n"  // 空行分隔头部和数据部分
            "%s",   // JSON 数据
            ip, strlen(dataString), dataString); // 将生成的JSON数据填入
}

// 发送数据到 ESP-01S
void espSendWithRequest(char *ip, float temperature, float humidity, float brightness, float co)
{
    char requestBuffer[200];  // 用于保存完整的 HTTP 请求

    // 生成完整的 HTTP 请求
    generateHttpPostRequest(requestBuffer, ip, temperature, humidity, brightness, co);

    // 调用发送函数
    espSend(requestBuffer, 1);  // 1 表示添加换行符
}

void espSend(char *message, int enterFlag)
{
  char command[50];
  if (enterFlag == 1)
  {
    addNewlineIfNeeded(message);
  }
  rt_kprintf("发送数据\n");
  sendData(&huart2, message);
  rt_thread_mdelay(1000);
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
