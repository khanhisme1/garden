#include "stm32f4xx.h"
#include "dht22.h"
#include "delay.h"

void DHT22_Start(void)
{   
    // Configure PA1 as output
    GPIOA->MODER &= ~(GPIO_MODER_MODER1);
    GPIOA->MODER |= GPIO_MODER_MODER1_0;
    
	//Pull up
	GPIOA->PUPDR &= ~(GPIO_PUPDR_PUPDR1);
    GPIOA->PUPDR |= GPIO_PUPDR_PUPDR1_0;

    // Set PA1 as high speed
    GPIOA->OSPEEDR &= ~(GPIO_OSPEEDER_OSPEEDR1);
    GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR1;
	
    // Reset PA1
    GPIOA->ODR &= ~GPIO_ODR_ODR_1;
    delay_us(1000);

    // Set PA1
    GPIOA->ODR |= GPIO_ODR_ODR_1;
    delay_us(20);
	GPIOA->ODR &= ~GPIO_ODR_ODR_1;
	
    // Set PA1 as input
    GPIOA->MODER &= ~GPIO_MODER_MODER1;
	GPIOA->PUPDR &= ~GPIO_PUPDR_PUPDR1;
    GPIOA->OSPEEDR &= ~GPIO_OSPEEDER_OSPEEDR1;
}

uint8_t DHT22_Check_Response (void) {
	dht22_state = NORMAL;
    uint8_t response = 0;
	uint32_t timeout = 100000;
    delay_us(40);
	
    if (!(GPIOA->IDR & GPIO_IDR_IDR_1))
    {
        delay_us(80);
        if (GPIOA->IDR & GPIO_IDR_IDR_1) response = 1;
        else response = 2;
    }
    while (GPIOA->IDR & GPIO_IDR_IDR_1 && timeout--) 
	{
		if (timeout == 0) 
		{
			dht22_state = ERROR;
			return response;
		}
	}

    return response;
}

uint8_t DHT22_Read()
{
	dht22_state = NORMAL;
	uint32_t timeout = 100000;
    uint8_t data, i;
    for(i = 0; i < 8; i++)
    {
        while (!(GPIOA->IDR & GPIO_IDR_IDR_1) && timeout--)
		{
			if (timeout == 0) 
			{
				dht22_state = ERROR;
				return 0;
			}
		}
        delay_us(40);
        if (!(GPIOA->IDR & GPIO_IDR_IDR_1)) {
			data &= ~(1<<(7-i));
		}
        else
        {
            data |= (1<<(7-i));
        }
		
		timeout = 100000;
		while ((GPIOA->IDR & GPIO_IDR_IDR_1) && timeout--)
		{
			if (timeout == 0) 
			{
				if (timeout == 0) 
				{
					dht22_state = ERROR;
					return 0;
				}
			}
		}
    }
	return data;
}
