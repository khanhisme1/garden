#include "stm32f4xx.h"
#include "i2c.h"



void I2C1_Init(void)
{
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
    RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;
	
    GPIOB->MODER |= GPIO_MODER_MODER8_1 | GPIO_MODER_MODER9_1;
	GPIOB->OTYPER |= GPIO_OTYPER_OT_8 | GPIO_OTYPER_OT_9;
	GPIOB->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR8 | GPIO_OSPEEDER_OSPEEDR9;
	GPIOB->PUPDR |= GPIO_PUPDR_PUPDR8_0 | GPIO_PUPDR_PUPDR9_0;
	GPIOB->AFR[1] |= (4 << 0) | (4 << 4);

	I2C1->CR1 |= I2C_CR1_SWRST;
	I2C1->CR1 &= ~I2C_CR1_SWRST;
	
    I2C1->CR2 |= 42;
    I2C1->CCR = 210;
    I2C1->TRISE = 43;
	
	I2C1->CR1 |= I2C_CR1_PE;
}

void I2C3_SendData(uint8_t data) {
    uint32_t timeout = I2C_TIMEOUT;

    // Acknowledge enable
    I2C3->CR1 |= I2C_CR1_ACK;

    // Send start signal
    I2C3->CR1 |= I2C_CR1_START;
    while(!(I2C3->SR1 & I2C_SR1_SB) && timeout--);
    if (timeout == 0) {
        return;
    }

    // Send slave address
    timeout = I2C_TIMEOUT;
    while(!(I2C3->SR1 & I2C_SR1_ADDR) && timeout--);
    if (timeout == 0) {
        return;
    }

    (void)I2C3->SR1;
    (void)I2C3->SR2;

    // Send data
    timeout = I2C_TIMEOUT;
    while (!(I2C3->SR1 & I2C_SR1_TXE) && timeout--);
    if (timeout == 0) {
        return;
    }

    I2C3->DR = data;
    timeout = I2C_TIMEOUT;
    while(!(I2C3->SR1 & I2C_SR1_BTF) && timeout--);
    if (timeout == 0) {
        return;
    }

    // Send stop signal
    I2C3->CR1 |= I2C_CR1_STOP;
}
