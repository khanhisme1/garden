#include <stm32f4xx.h>
#include "pwm.h"

void Timer3_Init(void) {
	//GPIOA Clock
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	
	//Timer 3 clock
    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
	
	//PA6 alternative function mode
	GPIOA->MODER &= ~GPIO_MODER_MODER6;
    GPIOA->MODER |= GPIO_MODER_MODER6_1;
	
	//PA6 PWM mode
    GPIOA->AFR[0] |= (2 << (24));
	
	//Prescaler value
    TIM3->PSC = 0;
	
	//Auto reload value
    TIM3->ARR = 100;
	
	//Set channel 1 for PWM
    TIM3->CCMR1 |= TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2;
	
    //Enable channel 1 compare output
	TIM3->CCER |= TIM_CCER_CC1E;
	
	//Enable timer 3
    TIM3->CR1 |= TIM_CR1_CEN;
	
	TIM3->CCR1 = 0;
}

void Timer4_Init(void) {
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
    RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;
	GPIOB->MODER &= ~GPIO_MODER_MODER6;
    GPIOB->MODER |= GPIO_MODER_MODER6_1;
    GPIOB->AFR[0] |= (2 << (24));
    TIM4->PSC = 0;
    TIM4->ARR = 100;
    TIM4->CCMR1 |= TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2;
	TIM4->CCER |= TIM_CCER_CC1E;
    TIM4->CR1 |= TIM_CR1_CEN;
	TIM4->CCR1 = 0;
}

void PWM3_SetDutyCycle(uint8_t value) {
    TIM3->CCR1 = value;
}

void PWM4_SetDutyCycle(uint8_t value) {
	TIM4->CCR1 = value;
}
