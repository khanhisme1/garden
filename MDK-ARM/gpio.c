#include "stm32f4xx.h"
#include "gpio.h"

void GPIOC0_Init(void) {
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;

	GPIOC->MODER |= GPIO_MODER_MODER0_0;
}

void GPIOC1_Init(void) {
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;

	GPIOC->MODER |= GPIO_MODER_MODER1_0;
}

void GPIOC2_Init(void) {
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;

	GPIOC->MODER |= GPIO_MODER_MODER2_0;
}

void GPIOC0_Off(void) {
	GPIOC->ODR &= ~GPIO_ODR_ODR_0;
}

void GPIOC0_On(void) {
	GPIOC->ODR |= GPIO_ODR_ODR_0;
}

void GPIOC1_On(void) {
	GPIOC->ODR |= GPIO_ODR_ODR_1;
}

void GPIOC1_Off(void) {
	GPIOC->ODR &= ~GPIO_ODR_ODR_1;
}

void GPIOC2_On(void) {
	GPIOC->ODR |= GPIO_ODR_ODR_2;
}

void GPIOC2_Off(void) {
	GPIOC->ODR &= ~GPIO_ODR_ODR_2;
}
