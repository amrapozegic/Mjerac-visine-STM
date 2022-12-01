#include "delay.h"
#include "rtc-ds1302.h"
#include "usart.h"

uint8_t clock_data[8];

void initGPIOrtc(){
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
	GPIOD->MODER |= 0x00000015;
	GPIOD->OSPEEDR |= 0x0000003F;
	
	GPIOD->ODR &= ~(0x0005); 
}

void RST_on(){
	GPIOD->ODR |= 0x0004;
}

void RST_off(){
	GPIOD->ODR &= ~(0x0004);
}

void CLK_pulse(){
	GPIOD->ODR |= 0x0001;
	delay_us(1);
	
	GPIOD->ODR &= ~(0x0001);
	delay_us(1);
}

void writeByte(uint8_t data){
	for(int i = 0; i < 8; ++i){
		if(data & 0x01)
			GPIOD->ODR |= 0x0002;
		else
			GPIOD->ODR &= ~(0x0002);

		CLK_pulse();
		
		data >>= 1;
	}
}

uint8_t readByte(uint8_t adress){
	uint8_t byte = 0x00;
	
	RST_on();
	writeByte(adress);
	
	GPIOD->MODER &= ~(0x0000000C);
		
		for(int j = 0; j < 8; ++j){
			byte = (byte >> 1);
			
			if(GPIOD->IDR & 0x0002)
				byte |= 0b10000000;
			else
				byte &= 0b01111111;
				
			CLK_pulse();
		}

	RST_off();
	
	GPIOD->MODER |= 0x00000004;
	
	return bcd2dec(byte);
}

uint8_t * getDateTime(){
	// Read DAT
	uint8_t temp = 0x00;
	
	RST_on();
	writeByte(RD_BURST_ADR);
	
	GPIOD->MODER &= ~(0x0000000C);
	
	for(int i = 0; i < 8; ++i){
		temp = 0x00;
		
		for(int j = 0; j < 8; ++j){
			temp = (temp >> 1);
			
			if(GPIOD->IDR & 0x0002)
				temp |= 0b10000000;
			else
				temp &= 0b01111111;
				
			CLK_pulse();
		}
	
	clock_data[i] = bcd2dec(temp);
	}
	
	RST_off();
	
	GPIOD->MODER |= 0x00000004;
	
	return clock_data;
}

uint8_t dec2bcd(uint8_t dec)
{
    return ((dec / 10 * 16) + (dec % 10));
}

uint8_t bcd2dec(uint8_t bcd){
	return ((bcd / 16 * 10) + (bcd % 16));
}

void setDateTime(uint8_t sec, uint8_t min, uint8_t hr, uint8_t date, uint8_t month, uint8_t day, uint8_t year){
	// Write DAT
	
	if(sec < 60 && sec >= 0 && min < 60 && min >= 0 && hr < 24 && hr >= 0 && date < 32 && date > 0 && month < 13 && month > 0 && day < 8 && day > 0 && year < 100 && year >= 0){
		uint8_t byte;
		uint8_t data[8] = {sec, min, hr, date, month, day, year, 0x00000000};
		
		RST_on();
		writeByte(WR_BURST_ADR);

		for(int i = 0; i < 8; ++i){
			byte = dec2bcd(data[i]);
			
			for(int j = 0; j < 8; ++j){
				if(byte & 0x01)
					GPIOD->ODR |= 0x0002;
				else
					GPIOD->ODR &= ~(0x0002);

				CLK_pulse();
				
				byte >>= 1;
			}
		}
		
		RST_off();
	}
}
