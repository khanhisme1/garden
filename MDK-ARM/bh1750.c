#include "stm32f4xx.h"
#include "bh1750.h"
#include "delay.h"

void I2C3_Init(void) {
    // Enable clock for I2C3 and GPIOA, GPIOB
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN | RCC_AHB1ENR_GPIOBEN;
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
	//Acknowlegde enable
	I2C3->CR1 |= I2C_CR1_ACK;
	
	//Send start signal
    I2C3->CR1 |= I2C_CR1_START;
    while(!(I2C3->SR1 & I2C_SR1_SB));
	
	//Send slave address
    I2C3->DR = BH1750_ADDRESS;
    while(!(I2C3->SR1 & I2C_SR1_ADDR));
	
	(void)I2C3->SR1;
    (void)I2C3->SR2;
	
	//Send data
	while (!(I2C3->SR1 & I2C_SR1_TXE));
    I2C3->DR = data;
    while(!(I2C3->SR1 & I2C_SR1_BTF));
	
	//Send stop signal
    I2C3->CR1 |= I2C_CR1_STOP;
}

uint16_t BH1750_Read(void) {
    uint16_t lumen = 0;

    I2C3->CR1 |= I2C_CR1_START;
    while (!(I2C3->SR1 & I2C_SR1_SB));

    I2C3->DR = (BH1750_ADDRESS) | 1;
    while (!(I2C3->SR1 & I2C_SR1_ADDR));

    (void)I2C3->SR1;
    (void)I2C3->SR2;

//    I2C3->CR1 |= I2C_CR1_ACK;

    while (!(I2C3->SR1 & I2C_SR1_RXNE));
    lumen = I2C3->DR << 8;

 //   I2C3->CR1 &= ~I2C_CR1_ACK;

    while (!(I2C3->SR1 & I2C_SR1_RXNE));
    lumen |= I2C3->DR;
	
	I2C3->CR1 |= I2C_CR1_STOP;

    return lumen;
}
