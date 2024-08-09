#include "stm32f4xx.h"



void UART2_Init(void);
void UART2_Transmit(uint8_t c);
void UART2_Transmit_String(char* str);
void UART2_Transmit_Uint16(uint16_t value);
void UART2_Transmit_Float(float humidity);

void UART1_Init(void);
void UART1_Transmit(char c);
void UART1_Transmit_String(char* str);
void UART1_Transmit_Uint16(uint16_t value);
void UART1_Transmit_Float(float humidity, float temperature);
