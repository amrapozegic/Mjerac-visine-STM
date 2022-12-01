#include "delay.h"
#include "4x4-keypad.h"

char key;
uint16_t col_state = 0x0800;

void initGPIOkeypad(){
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOEEN;
	GPIOE->MODER &= ~(0x003FC000);
	GPIOE->MODER |= 0x15400000;
	GPIOE->OSPEEDR |= 0x3FFFC000;
	
	GPIOE->ODR &= ~(0x7800); 
}

uint8_t getKey(){
	key = '\0';
	
	while(key == '\0'){
		GPIOE->ODR |= col_state;
		
		if(GPIOE->ODR == 0x0800){
			if(GPIOE->IDR & 0x0080){
				key = '1';
			}
			else if(GPIOE->IDR & 0x0100){
				key = '4';
			}
			else if(GPIOE->IDR & 0x0200){
				key = '7';
			}
			else if(GPIOE->IDR & 0x0400){
				key = '*';
			}
			else
				key = '\0';
		}
		else if(GPIOE->ODR == 0x1000){
			if(GPIOE->IDR & 0x0080){
				key = '2';
			}
			else if(GPIOE->IDR & 0x0100){
				key = '5';
			}
			else if(GPIOE->IDR & 0x0200){
				key = '8';
			}
			else if(GPIOE->IDR & 0x0400){
				key = '0';
			}
			else
				key = '\0';
		}
		else if(GPIOE->ODR == 0x2000){
			if(GPIOE->IDR & 0x0080){
				key = '3';
			}
			else if(GPIOE->IDR & 0x0100){
				key = '6';
			}
			else if(GPIOE->IDR & 0x0200){
				key = '9';
			}
			else if(GPIOE->IDR & 0x0400){
				key = '#';
			}
			else
				key = '\0';
		}
		else if(GPIOE->ODR == 0x4000){
			if(GPIOE->IDR & 0x0080){
				key = 'A';
			}
			else if(GPIOE->IDR & 0x0100){
				key = 'B';
			}
			else if(GPIOE->IDR & 0x0200){
				key = 'C';
			}
			else if(GPIOE->IDR & 0x0400){
				key = 'D';
			}
			else
				key = '\0';	
		}
		
		delay_ms(50);
		
		GPIOE->ODR &= ~(col_state);
			
		col_state <<= 1;
		if(col_state == 0x8000)
			col_state = 0x0800;
	}		
	delay_ms(350);
	return key;
}
