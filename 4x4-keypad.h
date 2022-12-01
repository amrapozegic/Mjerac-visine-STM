#ifndef __4X4_KEYPAD_H
#define __4X4_KEYPAD_H

#include "stm32f4xx.h"

void initGPIOkeypad();
uint8_t getKey();
#endif
