#include "stm32f4xx.h" 
#include "delay.h"

void Timer2_Init(void) { 
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN; 
}

void delay_us(uint32_t us) {
	TIM2->PSC = 16-1;
	TIM2->ARR = us;
	TIM2->EGR = TIM_EGR_UG;
	TIM2->SR &= ~TIM_SR_UIF;
	TIM2->CR1 |= TIM_CR1_CEN; 
	while (!(TIM2->SR & TIM_SR_UIF));
	TIM2->SR &= ~TIM_SR_UIF;
	TIM2->CR1 &= ~TIM_CR1_CEN;
}
