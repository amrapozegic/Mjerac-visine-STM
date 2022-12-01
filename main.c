#include <stdio.h>
#include "string.h"
#include "stm32f4xx.h"
#include "delay.h"
#include "16x2-lcd.h"
#include "4x4-keypad.h"
#include  "hc-sr04.h"
#include "rtc-ds1302.h"
#include "usart.h"

#define SPEED_OF_SOUND 		0.0343                                      // cm/us (on 20°C)

void getJMBG();
void measure();
void time();
void end_text();

char key = 0;
char JMBG[13] = "";
double height = 0;
uint8_t * clk;

int main(void){
	initGPIOlcd();
	initLCD_8bits();
	initGPIOkeypad();
	initGPIO_sensor1();
	initGPIO_sensor2();
	initSTOPWATCH();
	initGPIOrtc();
	initUSART3(USART3_BAUDRATE_921600);
	
	//setDateTime(0, 25, 18, 3, 4, 7, 22); 								// Format: [Sec (0 - 59), Min (0 - 59), Hr (0 - 24), Day of month (1 - 31),
																		// Month (1 - 12), Day of week (1 - 7), Year (0 - 22)]
	
	while(1){	
		clearLCD();
		setCursor(0, 2);
		printLCD("Pritisnite #");
		setCursor(1, 3);
		printLCD("za pocetak!");
		
		while(getKey() != '#');
		
		clearLCD();

		setCursor(0, 4);
		printLCD("Zdravo!");
		delay_ms(1000);
		
		getJMBG();
		measure();
		time();
		
		printUSART3("--------------------------------\n");
		printUSART3("JMBG: %s\n", JMBG);
		printUSART3("Visina: %f cm\n", height*10);
		printUSART3("Datum: %d/%d/%d/%d\n", clk[3], clk[4], clk[6], clk[5]);
		printUSART3("Vrijeme: %d:%d:%d\n", clk[2], clk[1], clk[0]);
		printUSART3("--------------------------------\n");
		
		setCursor(0, 1);
		printLCD("# - PONOVI");
		setCursor(1, 1);
		printLCD("* - KRAJ");
		
		do{
			key = getKey();
		}
		while(key != '#' && key != '*');
		
		if(key == '*')
			break;
		else
			JMBG[0] = '\0';
	}
	clearLCD();
	end_text();
}

void getJMBG(){
	char no_char = ' '; 
	
	while(1){
		clearLCD();	
		setCursor(0, 1);
		printLCD("Unesite JMBG:");	
		setCursor(1, 1);
		cursorON();
		
		while(strlen(JMBG) < 13){
			key = getKey();
		
			if(key != '*' && key != '#' && key != 'D' && key != 'B' && key != 'A'){
				if(key != 'C'){	
					printChar(key);		
					strncat(JMBG, &key, 1);
				}
				else{
					if(strlen(JMBG) != 0){
						setCursor(1, strlen(JMBG));
						printChar(no_char);
						JMBG[strlen(JMBG) - 1] = '\0';
					}
					setCursor(1, strlen(JMBG) + 1);
				}
			}
		}
		
		cursorOFF();
		delay_ms(1000);
		clearLCD();
		
		setCursor(0, 1);
		printLCD("# - POTVRDI");
		setCursor(1, 1);
		printLCD("* - ODBACI");
		
		do{
			key = getKey();
		}
		while(key != '#' && key != '*');
		
		if(key == '#')
			break;
		else
			JMBG[0] = '\0';
	}
	
	delay_us(50);
	clearLCD();
}

void measure(){
	setCursor(0, 1);
	printLCD("Pritisnite #");
	setCursor(1, 1);
	printLCD("za mjerenje!");
	
	while(getKey() != '#');
	
	double distance1 , distance2;
		
	distance1 = Distance_sensor1();
	delay_ms(50);		                        
	distance2 = Distance_sensor2();
		
	height = distance2 - distance1;

	char height_str[10];

	sprintf(height_str, "%.1f", height);
	
	clearLCD();
	setCursor(0, 4);
	printLCD("Visina:");
	setCursor(1, 4);
	printLCD(height_str);
	
	delay_ms(5000);
	clearLCD();
}

void time(){
	clk = getDateTime();
}

void end_text(){
	while(1){
		for(int i = 0; i <= 12; ++i){
			setCursor(0, i);
			printLCD("KRAJ");
			delay_ms(500);
			clearLCD();
		}
		for(int i = 12; i >= 0; --i){
			setCursor(0, i);
			printLCD("KRAJ");
			delay_ms(500);
			clearLCD();
		}
	}
}
