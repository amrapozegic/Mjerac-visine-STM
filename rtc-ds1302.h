#ifndef __RTC_DS1302_H
#define __RTC_DS1302_H

#include "stm32f4xx.h"

#define WR_CREG_ADR		0x8E	// Control Register Write
#define RD_CREG_ADR		0x8F	// Control Register Read

#define WR_TCR_ADR    	0x90  	//Trickle Charge Register Write
#define RD_TCR_ADR    	0x91  	//Trickle Charge Register Read

#define WR_SEC_ADR 		0x80	// Seconds Write
#define WR_MIN_ADR		0x82	// Minutes Write
#define WR_HR_ADR		0x84	// Hours Write
#define WR_DATE_ADR		0x86	// Day of Month Write
#define WR_MON_ADR		0x88 	// Month Write
#define WR_DAY_ADR		0x8A	// Day of Week Write
#define WR_YR_ADR		0x8C	// Year Write

#define RD_SEC_ADR		0x81	// Seconds Read
#define RD_MIN_ADR		0x83	// Minutes Read
#define RD_HR_ADR		0x85	// Hours Read
#define RD_DATE_ADR		0x87	// Day of Month Read
#define RD_MON_ADR		0x89	// Month Read
#define RD_DAY_ADR		0x8B	// Day of Week Read
#define RD_YR_ADR		0x8D	// Year Read

#define WR_BURST_ADR 	0xBE	// Burst Write
#define RD_BURST_ADR 	0xBF	// Burst Read

void initGPIOrtc();

void RST_on();
void RST_off();
void CLK_pulse();

void writeByte(uint8_t adress);
uint8_t readByte(uint8_t adress);

uint8_t * getDateTime();
void setDateTime(uint8_t sec, uint8_t min, uint8_t hr, uint8_t date, uint8_t month, uint8_t day, uint8_t year);

uint8_t dec2bcd(uint8_t dec);
uint8_t bcd2dec(uint8_t bcd);

#endif
