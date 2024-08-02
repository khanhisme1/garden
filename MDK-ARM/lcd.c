#include "stm32f4xx.h"
#include "i2c.h"
#include "lcd.h"
#include "delay.h"
#include <stdio.h>

void LCD_SendCmd(uint8_t cmd) {
    uint8_t data_u, data_l;
    uint8_t data_t[4];
    data_u = (cmd&0xf0);
    data_l = ((cmd<<4)&0xf0);
    data_t[0] = data_u|0x0C;
    data_t[1] = data_u|0x08;
    data_t[2] = data_l|0x0C;
    data_t[3] = data_l|0x08;
    for(int i=0; i<4; i++)
    {
        I2C1_SendData(LCD_ADDR, data_t[i]);
        delay_us(20);
    }
}

void LCD_SendData(uint8_t data) {
    uint8_t data_u, data_l;
    uint8_t data_t[4];
    data_u = (data&0xf0);
    data_l = ((data<<4)&0xf0);
    data_t[0] = data_u|0x0D;
    data_t[1] = data_u|0x09;
    data_t[2] = data_l|0x0D;
    data_t[3] = data_l|0x09;
    for(int i=0; i<4; i++)
    {
        I2C1_SendData(LCD_ADDR, data_t[i]);
        delay_us(20);
    }
}

void LCD_Init(void) {
    LCD_SendCmd(0x33);
	delay_us(10000);
	
    LCD_SendCmd(0x32);
	delay_us(2000);
	
    LCD_SendCmd(0x06);
	delay_us(2000);
	
    LCD_SendCmd(0x0C);
	delay_us(2000);
	
    LCD_SendCmd(0x28);
	delay_us(2000);
	
    LCD_SendCmd(0x01);
    delay_us(2000);
}

void LCD_PutString(char *string, int line) {
	
	if (line == 1) {
		LCD_SendCmd(0x80);
	} else {
		LCD_SendCmd(0xC0);
	}
    while (*string) {
        LCD_SendData((uint8_t)(*string));
        string++;
    }
}

void LCD_PutFloat(float value, int line) {
	char buffer[32];
	sprintf(buffer, "%.1f", value);
    LCD_PutString(buffer, line);
}

void LCD_PutFloatString(float value, char *string, int line) {
    char buffer[32];
    sprintf(buffer, "%.1f%s", value, string);
    LCD_PutString(buffer, line);
}
