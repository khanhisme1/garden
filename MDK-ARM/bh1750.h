#include "stm32f4xx.h"

extern volatile uint8_t light_th;
extern volatile uint8_t dirt_th;

#define I2C_TIMEOUT 100000
#define BH1750_ADDRESS 0x46
#define POWER_ON 0x01
#define RESET 0x07
#define CONTINUOUS_HIGH_RES_MODE 0x10

void BH1750_Init(void);
void I2C3_Init(void);
void I2C3_SendData(uint8_t data);
uint16_t BH1750_Read(void);