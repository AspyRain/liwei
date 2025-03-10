// globals.c
#include "global.h"

char usart1_rx_buffer[BUFFER_SIZE];
int usart1_rx_index = 0;
int esp_flag = 0;
int timeout_flag = 0;
int esp_command_flag=0;
int AT_OK_Flag = 0;
int AT_Ready_Flag = 0;
int AT_Connect_Net_Flag = 0;
char *status = "";