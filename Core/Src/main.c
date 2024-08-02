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
volatile uint8_t temperature_th = 25;
volatile uint8_t light_th = 50;
volatile uint8_t dirt_th = 50;
volatile uint8_t state = DIRT;

int main(void)
{
	GPIOC0_Init();
	GPIOC1_Init();
	GPIOC2_Init();
	UART1_Init();
	UART2_Init();
	ADC_Init();
	Interrupt_PB_Init();
	Timer2_Init();
	Timer3_Init();
	I2C3_Init();
	I2C1_Init();
	LCD_Init();
	I2C3_SendData(POWER_ON);
	delay_us(1000);
	I2C3_SendData(RESET);
	delay_us(1000);
	I2C3_SendData(CONTINUOUS_HIGH_RES_MODE);
	delay_us(120000);


	uint32_t dirt_value;
	float dirt;
	
	uint8_t response;
	uint8_t humidity_first_byte;
	uint8_t humidity_second_byte;
	uint8_t temperature_first_byte;
	uint8_t temperature_second_byte;
	uint8_t sum;
	
	uint16_t raw_humidity;
	uint16_t raw_temperature;
	
	float humidity;
	float temperature;

	uint8_t temperature1;
	uint8_t humidity1;

	uint8_t last_state = state;
	
	while (1)
	{
		//Light intensity sensor
		float lumen = (float) BH1750_Read() / 1.2;
		
		if (lumen > 12000) {
			lumen = 12000;
		}
		
		lumen = (lumen / 12000) * 100;
		
		if (lumen  > (light_th + 10) || lumen < (light_th - 10)) {
			PWM3_SetDutyCycle(light_th);
		}
		
		// Dirt sensor
		dirt_value = ADC_Read_Channel_1();
		dirt = (float) dirt_value;
		dirt = (dirt / 4095) * 100; 
		if (dirt < dirt_th) {
			GPIOC1_On();
		} else {
			GPIOC1_Off();
		}

		// Humidity and temperature sensor
		DHT22_Start();
		response = DHT22_Check_Response();
		if (response == 2) {
			UART2_Transmit_String("Error");
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
				GPIOC2_Off();
				GPIOC0_On();
			} else {
				GPIOC2_On();
				GPIOC0_Off();
			}
			
			if (state == HUMID) {
				LCD_PutString("HUMIDITY:       ", 1);
				LCD_PutFloatString(humidity, "%     ", 2);
			} else if (state == TEMPERATURE) {
				LCD_PutString("TEMPERATURE:    ", 1);
				LCD_PutFloatString(temperature, " do C" , 2);
			} else if (state == LIGHT) {
				LCD_PutString("LIGHT INTENSITY:", 1);
				LCD_PutFloatString(lumen, "%     ",  2);
			} else if (state == DIRT){
				LCD_PutString("SOIL HUMIDITY:  ", 1);
				LCD_PutFloatString(dirt, "%     ", 2);
			}
		}
		
		// Check every 400ms
		delay_us(700000);
	}
}
