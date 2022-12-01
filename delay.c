#include "delay.h"

#define SYSTIMER_TIMEOUT 0
#define SYSTIMER_KEEP_ALIVE	1

void delay_ms(uint32_t ms){
	RCC->APB1ENR |= RCC_APB1ENR_TIM12EN;								// Enable APB1@42MHz BUS for TIM12
	TIM12->PSC = 0x0054 - 0x0001;										// Set prescaler to 84 to get 1MHz timer => T = 1us
	TIM12->ARR = 0x03E8;												// Reset timer on 1000 => 1ms
	TIM12->CR1 = 0X0084;												// ARPE ON, CMS disable, Up counting
	
	TIM12->EGR = TIM_EGR_UG;											// SET UG (Update Generation) to 1 to re-initialize counter and update the registers
	TIM12->CR1 |= TIM_CR1_CEN;											// SET CEN (Counter enable) to 1 - Starts the counter
	
	while(ms > 0){
		while((TIM12->SR & TIM_SR_UIF) == 0x0000);						// Wait for update event (UIF bit is set by hardware on update event)
		
		TIM12->SR &= ~TIM_SR_UIF;										// Clear the update flag (It is set by hardware, and has to be cleared by software)
		--ms;
	}
	
	TIM12->CR1 &= ~TIM_CR1_CEN;											// Disable counter - Stops the counter
	RCC->APB1ENR &= ~RCC_APB1ENR_TIM12EN;								// Disable the timer (TIM12)
}

void delay_us(uint32_t us){
	RCC->APB1ENR |= RCC_APB1ENR_TIM12EN;								// Enable APB1@42MHz BUS
	TIM12->PSC = 0x0001 - 0x0001;										// Set prescaler to 1 to get 84MHz timer => T = (1/84)us
	TIM12->ARR = 0x0054;												// Reset timer on 84 => 1us
	TIM12->CR1 = 0X0084;												
	
	TIM12->EGR = TIM_EGR_UG;											
	TIM12->CR1 |= TIM_CR1_CEN;											
	
	while(us > 0){
		while((TIM12->SR & TIM_SR_UIF) == 0x0000);						
		
		TIM12->SR &= ~TIM_SR_UIF;										
		--us;
	}
	
	TIM12->CR1 &= ~TIM_CR1_CEN;											
	RCC->APB1ENR &= ~RCC_APB1ENR_TIM12EN;
}

void initSYSTIMER(void)
{
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN; 								
	TIM2->PSC = 42000-0x0001;											 
																		
	TIM2->ARR = 0xFFFFFFFF;												
	TIM2->CR1 = 0x0084;													
																		
	TIM2->CR2 = 0x0000;
	TIM2->CNT = 0x00000000;												
	TIM2->EGR |= TIM_EGR_UG;											
	TIM2->CR1 |= TIM_CR1_CEN;												
}

uint32_t getSYSTIMER(void)
{
	uint32_t time = TIM2->CNT / 2;
	return time;
}

uint8_t chk4TimeoutSYSTIMER(uint32_t btime, uint32_t period)
{
	uint32_t time = TIM2->CNT / 2;
	if(time >= btime)
	{
		if(time >= (btime + period))
			return (SYSTIMER_TIMEOUT);
		else
			return (SYSTIMER_KEEP_ALIVE);
	}
	else
	{
		uint32_t utmp32 = 0xFFFFFFFF - btime;
		if((time + utmp32) >= period)
			return (SYSTIMER_TIMEOUT);
		else
			return (SYSTIMER_KEEP_ALIVE);
	}
}

void initSTOPWATCH(void)
{/// us resolution	
	RCC->APB1ENR |= RCC_APB1ENR_TIM5EN; 								// enable TIM5 
	TIM5->PSC = 0x0054-0x0001;											// set TIM5 counting prescaler to 84 (p406)
																		// 84MHz/84 = 1MHz -> count each us
	TIM5->ARR = 0xFFFFFFFF;												// reload value set to 1.19304647083h!
	TIM5->CR1 = 0x0084;													// ARPE On, CMS disable, Up counting
																		// UEV disable, TIM5 enable(p392)
	TIM5->CR2 = 0x0000;
	TIM5->CNT = 0x00000000;												// clear the start value
	
}

void startSTOPWATCH(void)
{
	TIM5->EGR |= TIM_EGR_UG;											// update event, reload all config p363
	TIM5->CR1 |= TIM_CR1_CEN;											// start counting!
}

uint32_t stopSTOPWATCH(void)
{
	uint32_t time;

	time = TIM5->CNT;
	TIM5->CR1 &= ~TIM_CR1_CEN;											// stop counting
	return time;
}
