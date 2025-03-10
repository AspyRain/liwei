#ifndef __GLOBAL_H
#define __GLOBAL_H
#define BUFFER_SIZE 250
#define dataMaxLen 4
extern char usart1_rx_buffer[BUFFER_SIZE];
extern int usart1_rx_index;
extern int esp_flag;
extern int timeout_flag;
extern int esp_command_flag;
extern int AT_OK_Flag;
extern int AT_Ready_Flag;
extern int AT_Connect_Net_Flag;
extern char* status;
#endif // __GLOBAL_H