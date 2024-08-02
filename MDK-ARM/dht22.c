#include "stm32f4xx.h"
#include "dht22.h"
#include "delay.h"
#include "lcd.h"

void DHT22_Start(void)
{   
    // Configure PA4 as output
    GPIOA->MODER &= ~(GPIO_MODER_MODER4);
    GPIOA->MODER |= GPIO_MODER_MODER4_0;
    
	//Pull up
	GPIOA->PUPDR &= ~(GPIO_PUPDR_PUPDR4);
    GPIOA->PUPDR |= GPIO_PUPDR_PUPDR4_0;

    // Set PA4 as high speed
    GPIOA->OSPEEDR &= ~(GPIO_OSPEEDER_OSPEEDR4);
    GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR4;
	
    // Reset PA4
    GPIOA->ODR &= ~GPIO_ODR_ODR_4;
    delay_us(1000);

    // Set PA4
    GPIOA->ODR |= GPIO_ODR_ODR_4;
    delay_us(20);
	GPIOA->ODR &= ~GPIO_ODR_ODR_4;
	
    // Set PA4 as input
    GPIOA->MODER &= ~GPIO_MODER_MODER4;
	
	// Pull up
	GPIOA->PUPDR &= ~GPIO_PUPDR_PUPDR4;

	// Out speed disabled
    GPIOA->OSPEEDR &= ~GPIO_OSPEEDER_OSPEEDR4;
}

uint8_t DHT22_Check_Response (void) {
    uint8_t response = 0;
    delay_us(40);
	
	//Check if the data is ready to be sent
    if (!(GPIOA->IDR & GPIO_IDR_IDR_4))
    {
        delay_us(80);
        if (GPIOA->IDR & GPIO_IDR_IDR_4) response = 1;
        else response = 2;
    }
    while (GPIOA->IDR & GPIO_IDR_IDR_4); // wait till input is 0

    return response;
}

uint8_t DHT22_Read()
{
    uint8_t data, i;
	//Read 8 bit
    for(i = 0; i < 8; i++)
    {
        while (!(GPIOA->IDR & GPIO_IDR_IDR_4));
        delay_us(40);
        if (!(GPIOA->IDR & GPIO_IDR_IDR_4)) {
			data &= ~(1<<(7-i));
		}
        else
        {
            data |= (1<<(7-i));
        }
		while (GPIOA->IDR & GPIO_IDR_IDR_4);
    }
	return data;
}
