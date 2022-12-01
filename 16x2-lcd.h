#ifndef __16X2_LCD_H
#define __16X2_LCD_H

#include "stm32f4xx.h"

void initGPIOlcd();
void initLCD_8bits();
void sendBits(uint8_t data);
void printChar(uint8_t c);
void sendCommand(uint8_t com); 
void printLCD(char * str);
void setCursor(uint8_t row, uint8_t col);
void clearLCD();
void EN_pulse();
void cursorON();
void cursorOFF();

#endif
