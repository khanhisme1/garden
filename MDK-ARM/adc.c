#include "stm32f4xx.h"
#include "adc.h"

void ADC_Init() {
	//GPIOA Clock
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	
	//PA1 analog mode
	GPIOA->MODER |= GPIO_MODER_MODE1_0 | GPIO_MODER_MODE1_1;
	
	//PA0 analog mode
	GPIOA->MODER |= GPIO_MODER_MODE0_0 | GPIO_MODER_MODE0_1;
	
	//ADC clock
	RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
	
	//Sampling time
	ADC1->SMPR2 = ADC_SMPR2_SMP0_0 | ADC_SMPR2_SMP1_0;
	
	//2 channel adc
	ADC1->SQR1 = ADC_SQR1_L_0;
	
	//ADC enable
	ADC1->CR2 |= ADC_CR2_ADON;
}

uint32_t ADC_Read_Channel_0() {
	//Set channel 0
	ADC1->SQR3 = 0;
	
	//Start conversion
	ADC1->CR2 |= ADC_CR2_SWSTART;
	while(!(ADC1->SR & ADC_SR_EOC));
	
	//Return converted result
	return ADC1->DR;
}

uint32_t ADC_Read_Channel_1() {
	//Set channel 1
	ADC1->SQR3 = 1;
	
	//Start conversion
	ADC1->CR2 |= ADC_CR2_SWSTART;
	while(!(ADC1->SR & ADC_SR_EOC));
	
	//Return converted result
	return ADC1->DR;
}
