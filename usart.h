#ifndef __USART3_H_
#define __USART3_H_

#include <stdio.h>
#include <stdarg.h>
#include "stm32f4xx.h"
#include "misc.h"

#define USART_ECHO

#define USART3_BUFFER_SIZE	512

#define USART3_BAUDRATE_921600 0x0000002D
#define USART3_BAUDRATE_460800 0x0000005B
#define USART3_BAUDRATE_115200 0x0000016C
#define USART3_BAUDRATE_9600 0x00001117

void initUSART3(uint32_t baudrate);
void putcharUSART3(uint8_t data);
void printUSART3(char * str, ...);
void sprintUSART3(uint8_t * str);

#endif
