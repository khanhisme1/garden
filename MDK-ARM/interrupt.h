#include "stm32f4xx.h"

extern volatile uint8_t state;

#define HUMID 0
#define TEMPERATURE 1
#define LIGHT 2
#define DIRT 3
#define ERROR_DHT22 4
#define ERROR_BH1750 5
#define ERROR_DIRT_SENSOR 6

void Interrupt_PB_Init(void);