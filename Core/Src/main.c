#include "stm32f4xx.h"
#include "pwm.h"
#include "dht22.h"
#include "delay.h"
#include "uart.h"
#include "adc.h"
#include "gpio.h"
#include "i2c.h"
#include "lcd.h"
#include <stdio.h>
#include "bh1750.h"
#include "interrupt.h"

volatile uint8_t humid_th = 50;
volatile uint8_t temperature_th = 50;
volatile uint8_t light_th = 0;
volatile uint8_t dirt_th = 50;
volatile uint8_t dht22_state = NORMAL;
volatile uint8_t bh1750_state = NORMAL;
volatile uint8_t state = HUMID;

int main(void)
{
	Timer2_Init();
	Timer3_Init();
	Timer2_Init();
	GPIOC0_Init();
	GPIOC1_Init();
	GPIOC2_Init();
	GPIOC0_Off();
	GPIOC1_Off();
	GPIOC2_Off();
	UART1_Init();
	UART2_Init();
	ADC_Init();
	Interrupt_PB_Init();
	I2C3_Init();
	I2C1_Init();
	LCD_Init();
	BH1750_Init();

	uint8_t response;
	uint8_t humidity_first_byte;
	uint8_t humidity_second_byte;
	uint8_t temperature_first_byte;
	uint8_t temperature_second_byte;
	uint8_t sum;
	
	uint16_t raw_humidity;
	uint16_t raw_temperature;
	
	uint32_t dirt_value;
	uint8_t last_state = state;
	
	float humidity;
	float temperature;
	float lumen;
	float dirt;
	
	while (1)
	{
		//Display
		if (state == HUMID) {
			if (dht22_state == ERROR) {
				LCD_PutString("DHT22 ERROR     ", 1);
				LCD_PutString("DHT22 ERROR     ", 2);
				continue;
			}
			LCD_PutString("HUMIDITY:       ", 1);
			LCD_PutFloatString(humidity, "%     ", 2);
		} else if (state == TEMPERATURE) {
			if (dht22_state == ERROR) {
				LCD_PutString("DHT22 ERROR     ", 1);
				LCD_PutString("DHT22 ERROR     ", 2);
				continue;
			}
			LCD_PutString("TEMPERATURE:    ", 1);
			LCD_PutFloatString(temperature, "\'C   " , 2);
		} else if (state == LIGHT) {
			if (bh1750_state == ERROR) {
				LCD_PutString("BH1750 ERROR    ", 1);
				LCD_PutString("BH1750 ERROR    ", 2);
				continue;
			}
			LCD_PutString("LIGHT INTENSITY:", 1);
			LCD_PutFloatString(lumen, " lux  ",  2);
		} else if (state == DIRT){
			LCD_PutString("SOIL HUMIDITY:  ", 1);
			LCD_PutFloatString(dirt, "%     ", 2);
		}
		
		//Light intensity sensor
		lumen = BH1750_Read();
		if (lumen > 12000) {
			lumen = 12000;
		}
		

		PWM3_SetDutyCycle(light_th);
		
		// Dirt sensor
		dirt_value = ADC_Read_Channel_0();
		dirt = (float) dirt_value;
//		dirt = (dirt / 4095) * 100;
		dirt = 50;
		if (dirt < dirt_th) {
			GPIOC2_On();
		} else {
			GPIOC2_Off();
		}
		

		// Humidity and temperature sensor
		DHT22_Start();
		response = DHT22_Check_Response();
		if (response == 2) {
			dht22_state = ERROR;
		} else {
			//Read 40 bit data from dht22
			humidity_first_byte = DHT22_Read();
			humidity_second_byte = DHT22_Read();
			temperature_first_byte = DHT22_Read();
			temperature_second_byte = DHT22_Read();
			sum = DHT22_Read();
			
			//Concat the 8 bit datas
			raw_humidity = (humidity_first_byte << 8) | humidity_second_byte;
			raw_temperature = (temperature_first_byte << 8) | temperature_second_byte;

			//Caculate the real data 
			humidity = raw_humidity / 10.0f;
			temperature = raw_temperature / 10.0f;
			
			//Threshold for fan and mist spray
			if (humidity < humid_th || temperature > temperature_th) {
				GPIOC0_On();
				GPIOC1_On();
			} else {
				GPIOC0_Off();
				GPIOC1_Off();
			}
		}
		
		
		// Check every 700ms
		delay_us(700000);
	}
}