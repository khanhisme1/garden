#include "stm32f4xx.h"
#include <stdio.h>
#include "uart.h"
#include "dht22.h"
#include "bh1750.h"

volatile uint8_t sensor;
volatile uint8_t flag;

void UART2_Init(void) {
	flag = 0;
	// Clock
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
	
	// Alternate function mode
    GPIOA->MODER |= GPIO_MODER_MODER2_1 | GPIO_MODER_MODER3_1;

	// Alternative function to 7
    GPIOA->AFR[0] |= 0x00007700;

	//Baud rate 9600 with 16Mhz clock bus
    USART2->BRR = 0x683;

	// Enable uart
    USART2->CR1 |= USART_CR1_UE;

	// Enable TX and RX
    USART2->CR1 |= USART_CR1_TE | USART_CR1_RE;
	
	// Enable RX interrupt
    USART2->CR1 |= USART_CR1_RXNEIE;
	
	// Set USART2 interrupt priority and enable it
	NVIC_SetPriority(USART2_IRQn, 7);
    NVIC_EnableIRQ(USART2_IRQn);
}

void UART2_Transmit(uint8_t c) {
    while (!(USART2->SR & USART_SR_TXE));
    USART2->DR = c;
}

void UART2_Transmit_String(char* str) {
    while (*str) {
        UART2_Transmit(*str++);
    }
}

void UART2_Transmit_Uint16(uint16_t value) {
    char buffer[6];
    snprintf(buffer, sizeof(buffer), "%u", value);
    UART1_Transmit_String(buffer);
}

void UART2_Transmit_Float(float humidity, float temperature) {
    char buffer[32];
    snprintf(buffer, sizeof(buffer), "%g %g\n", humidity, temperature);
    UART2_Transmit_String(buffer);
}

void USART2_IRQHandler(void) {
    if (USART2->SR & USART_SR_RXNE) {
        char received_char = USART2->DR;
        UART2_Transmit(received_char);
    }
}

void UART1_Init(void) {
	// Clock
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
	
	// Alternate function mode
    GPIOA->MODER |= GPIO_MODER_MODER9_1 | GPIO_MODER_MODER10_1;

	// Alternative function to 7
    GPIOA->AFR[1] |= 0x00000770;

	//Baud rate 9600 with 16Mhz clock bus
    USART1->BRR = 0x683;

	// Enable uart
    USART1->CR1 |= USART_CR1_UE;

	// Enable TX and RX
    USART1->CR1 |= USART_CR1_TE | USART_CR1_RE;
	
	// Enable RX interrupt
    USART1->CR1 |= USART_CR1_RXNEIE;
	
	// Set USART2 interrupt priority and enable it
	NVIC_SetPriority(USART1_IRQn, 8);
    NVIC_EnableIRQ(USART1_IRQn);
}

void UART1_Transmit(char c) {
    while (!(USART1->SR & USART_SR_TXE));
    USART1->DR = c;
}

void UART1_Transmit_String(char* str) {
    while (*str) {
        UART1_Transmit(*str++);
    }
}

void UART1_Transmit_Uint16(uint16_t value) {
    char buffer[6];  // Enough to hold maximum uint16_t value (65535) plus null terminator
    snprintf(buffer, sizeof(buffer), "%u", value);
    UART1_Transmit_String(buffer);
}

void UART1_Transmit_Float(float humidity, float temperature) {
    char buffer[32];
    snprintf(buffer, sizeof(buffer), "%g %g\n", humidity, temperature);
    UART1_Transmit_String(buffer);
}

void USART1_IRQHandler(void) {
    if (USART1->SR & USART_SR_RXNE) {
		if ( flag == 0) {
			flag = 1;
			sensor = USART1->DR;
		} else {
			flag = 0;
			switch (sensor) {
				case 1:
					humid_th = USART1->DR;
					UART2_Transmit_String("1");
					UART2_Transmit(humid_th);
					break;
				case 2:
					temperature_th = USART1->DR;
					UART2_Transmit_String("2");
					UART2_Transmit(temperature_th);
					break;
				case 3:
					light_th = USART1->DR;
					UART2_Transmit_String("3");
					UART2_Transmit(light_th);
					break;
				case 4:
					dirt_th = USART1->DR;
					UART2_Transmit_String("4");
					UART2_Transmit(dirt_th);
					break;
			}
		}
    }
}

