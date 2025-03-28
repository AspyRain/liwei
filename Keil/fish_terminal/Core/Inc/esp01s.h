#ifndef ESP01S_H
#define ESP01S_H
#include <stdint.h>
void Esp01s_Init(char *ip, char *password, char *server,int port);
void espSend(char* message,int enterFlag);
void addNewlineIfNeeded(char* message);
#endif /* ESP01S_H */