#ifndef ESP01S_H
#define ESP01S_H
#include <stdint.h>
void Esp01s_Init(char* ip, char* password, char* port);
void espSend(char* message,int enterFlag);
void addNewlineIfNeeded(char* message);
void waitCommand(char *command, uint32_t timeout_ms);
#endif /* ESP01S_H */