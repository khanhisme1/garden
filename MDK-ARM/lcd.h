#include "stm32f4xx.h"

#define LCD_ADDR (0x27 << 1)
#define LCD_EN 0x04
#define LCD_RS 0x01

void LCD_Init(void);
void LCD_SendCmd(uint8_t cmd);
void LCD_SendData(uint8_t data);
void LCD_PutString(char *string, int line);
void LCD_PutFloat(float value, int line);
void LCD_PutFloatString(float value, char *string, int line);
