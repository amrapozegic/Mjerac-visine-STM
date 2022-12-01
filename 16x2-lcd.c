#include "delay.h"
#include "16x2-lcd.h"

void initGPIOlcd(){
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
	GPIOC->MODER |= 0x00005555;
	GPIOC->OSPEEDR |= 0x0000FFFF;
	GPIOC->ODR &= ~(0x00FF);
	
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	GPIOA->MODER |= 0x00000044;
	GPIOA->OSPEEDR |= 0x000000CC;
	GPIOA->ODR &= ~(0x000A);
}

void initLCD_8bits(){	
	 //Function set Enable 2 lines, Data length to 8 bits
	 sendCommand((0x20 | 0x08 | 0x10));
	 
	 //Display control (Display ON, Cursor OFF)
	 sendCommand((0x08 | 0x04));
	
	clearLCD();
	setCursor(0, 0);
	
	delay_ms(20);
}

void sendBits(uint8_t data){
	uint16_t odr = 0x0001;
	for(int i = 0 ; i < 8 ; ++i){
		if(data & 0x01)
			GPIOC->ODR |= odr;
		else
			GPIOC->ODR &= ~(odr);
		
		odr <<= 1;	
		data >>= 1;
	}
}

void sendCommand(uint8_t com){
	GPIOA->ODR &= ~(0x0002); 											// RS = 0, RW = 0 (Instruction code, write)
	sendBits(com);
	EN_pulse();
}


void printChar(uint8_t c){
	GPIOA->ODR |= 0x0002; 												// RS = 1, RW = 0 (Data, write)
	sendBits(c);
	EN_pulse();
}
void EN_pulse(){
	GPIOA->ODR |= 0x0008;
	delay_us(20);
	GPIOA->ODR &= ~(0x0008);
	delay_us(60);
}

void clearLCD(){
	sendCommand(0x01);
	delay_ms(5);
}

void printLCD(char * str){
	for(int i = 0; str[i] != '\0' && i < 16; ++i){
		printChar(str[i]);
		delay_ms(1);
	}
}
void setCursor(uint8_t row, uint8_t col){
	uint8_t cursor = 0x00;
	if(row == 0)
		cursor = 0x80;
	else
		cursor = 0xC0;
		
	if(col >= 1 && col <= 15)
		cursor |= col;
	
	sendCommand(cursor);		
	
}

void cursorON(){
	sendCommand((0x08 | 0x04 | 0x01 ));
}

void cursorOFF(){
	sendCommand((0x08 | 0x04));
}

