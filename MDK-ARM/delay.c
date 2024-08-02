#include "stm32f4xx.h" 
#include "delay.h"

void Timer2_Init(void) { 
	//Clock enable 
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN; 
}

void delay_us(uint32_t us) {
	//Prescaler 
	TIM2->PSC = 16-1;
	
	//Auto-reload value
	TIM2->ARR = us;
	
	//Forced update
	TIM2->EGR = TIM_EGR_UG;
	
	//Clear update flag
	TIM2->SR &= ~TIM_SR_UIF;
	
	//Timer enable 
	TIM2->CR1 |= TIM_CR1_CEN; 
	while (!(TIM2->SR & TIM_SR_UIF)); // wait till update flag is set 
	
	//Clear update flag
	TIM2->SR &= ~TIM_SR_UIF;
	
	//Stop timer
	TIM2->CR1 &= ~TIM_CR1_CEN;
}
