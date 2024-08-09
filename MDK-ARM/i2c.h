#include "stm32f4xx.h"

#define I2C_TIMEOUT 100000

void I2C1_Init(void);
void I2C1_SendData(uint8_t address, uint8_t data);
