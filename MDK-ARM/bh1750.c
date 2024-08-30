#include "stm32f4xx.h"
#include "bh1750.h"
#include "delay.h"

void BH1750_Init(void) {
	I2C3_SendData(POWER_ON);
	delay_us(1000);
	I2C3_SendData(RESET);
	delay_us(1000);
	I2C3_SendData(CONTINUOUS_HIGH_RES_MODE);
	delay_us(120000);
}


void I2C3_Init(void) {
    // Enable clock for I2C3 and GPIOA, GPIOC
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN | RCC_AHB1ENR_GPIOAEN;
    RCC->APB1ENR |= RCC_APB1ENR_I2C3EN;

    //Aternate function
    GPIOA->MODER |= GPIO_MODER_MODER8_1;
	
    //Open drain
	GPIOA->OTYPER |= GPIO_OTYPER_OT_8;
	
    //High speed output
	GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR8;
	
    //Pull up resistor
	GPIOA->PUPDR |= GPIO_PUPDR_PUPDR8_0;
	
    //Alternative function 4 for I2C
	GPIOA->AFR[1] |= (4 << 0);

    // Configure PBC9 as I2C3 SDA
    GPIOC->MODER |= GPIO_MODER_MODER9_1;
    GPIOC->OTYPER |= GPIO_OTYPER_OT_9;
    GPIOC->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR9;
    GPIOC->PUPDR |= GPIO_PUPDR_PUPDR9_0;
    GPIOC->AFR[1] |= (4 << 4);

	//Reset
    I2C3->CR1 |= I2C_CR1_SWRST;
	I2C3->CR1 &= ~I2C_CR1_SWRST;
	
	//I2C clock bus
    I2C3->CR2 |= 42;
    I2C3->CCR = 210;
    I2C3->TRISE = 43;
	
    //I2C enable
	I2C3->CR1 |= I2C_CR1_PE;
}


void I2C3_SendData(uint8_t data) {
	bh1750_state = NORMAL;
    uint32_t timeout = I2C_TIMEOUT;

    // Acknowledge enable
    I2C3->CR1 |= I2C_CR1_ACK;

    // Send start signal
    I2C3->CR1 |= I2C_CR1_START;
    while(!(I2C3->SR1 & I2C_SR1_SB) && timeout--)
    {
		if (timeout == 0) {
			bh1750_state = ERROR;
		}
	}

    // Send slave address
    I2C3->DR = BH1750_ADDRESS;
    timeout = I2C_TIMEOUT;
    while(!(I2C3->SR1 & I2C_SR1_ADDR) && timeout--)
    {
		if (timeout == 0) {
			bh1750_state = ERROR;
		}
	}

    (void)I2C3->SR1;
    (void)I2C3->SR2;

    // Send data
    timeout = I2C_TIMEOUT;
    while (!(I2C3->SR1 & I2C_SR1_TXE) && timeout--)
    {
		if (timeout == 0) {
			bh1750_state = ERROR;
		}
	}

    I2C3->DR = data;
    timeout = I2C_TIMEOUT;
    while(!(I2C3->SR1 & I2C_SR1_BTF) && timeout--)
    {
		if (timeout == 0) {
			bh1750_state = ERROR;
		}
	}

    // Send stop signal
    I2C3->CR1 |= I2C_CR1_STOP;
}

uint16_t BH1750_Read(void) {
	bh1750_state = NORMAL;
    uint16_t lumen = 0;
    uint32_t timeout = I2C_TIMEOUT;

    I2C3->CR1 |= I2C_CR1_START;
    while (!(I2C3->SR1 & I2C_SR1_SB) && timeout--)
    {
		if (timeout == 0) {
			bh1750_state = ERROR;
		}
	}

    I2C3->DR = (BH1750_ADDRESS) | 1;
    timeout = I2C_TIMEOUT;
    while (!(I2C3->SR1 & I2C_SR1_ADDR) && timeout--)
    {
		if (timeout == 0) {
			bh1750_state = ERROR;
		}
	}

    (void)I2C3->SR1;
    (void)I2C3->SR2;

    timeout = I2C_TIMEOUT;
    while (!(I2C3->SR1 & I2C_SR1_RXNE) && timeout--)
    {
		if (timeout == 0) {
			bh1750_state = ERROR;
		}
	}

    lumen = I2C3->DR << 8;

    timeout = I2C_TIMEOUT;
    while (!(I2C3->SR1 & I2C_SR1_RXNE) && timeout--)
    {
		if (timeout == 0) {
			bh1750_state = ERROR;
		}
	}

    lumen |= I2C3->DR;

    I2C3->CR1 |= I2C_CR1_STOP;

    return lumen;
}
