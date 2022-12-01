#include "delay.h"

void initGPIO_sensor1(){
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOEEN;
	GPIOE->MODER |= 0x00000400;
	GPIOE->MODER &= ~(0x00003000);
	GPIOE->OSPEEDR |= 0x00003C00;
}

void TrigHIGH_sensor1(){
	GPIOE->ODR &= ~(0x0020);
	delay_us(2);
				
	GPIOE->ODR |= 0x0020;
	delay_us(10);
				
	GPIOE->ODR &= ~(0x0020);
}

void initGPIO_sensor2(){
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOEEN;
	GPIOE->MODER |= 0x00000040;
	GPIOE->MODER &= ~(0x00000300);
	GPIOE->OSPEEDR |= 0x000003C0;
}

void TrigHIGH_sensor2(){
	GPIOE->ODR &= ~(0x0008);
	delay_us(2);
				
	GPIOE->ODR |= 0x0008;
	delay_us(10);
				
	GPIOE->ODR &= ~(0x0008);
}

double Distance_sensor1(){
	TrigHIGH_sensor1();
			
	while(!(GPIOE->IDR & 0x0040));
	startSTOPWATCH();

	while(GPIOE->IDR & 0x0040);
	return stopSTOPWATCH() / 58.3090379; 								// s = v * t
}

double Distance_sensor2(){
	TrigHIGH_sensor2();
			
	while(!(GPIOE->IDR & 0x0010));
	startSTOPWATCH();
			
	while(GPIOE->IDR & 0x0010);
	return stopSTOPWATCH() / 58.3090379;	
}
