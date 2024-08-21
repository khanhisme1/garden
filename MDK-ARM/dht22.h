#include "stm32f4xx.h"

#define NORMAL 0
#define ERROR 1

extern volatile uint8_t humid_th;
extern volatile uint8_t temperature_th;
extern volatile uint8_t dht22_state;

void DHT22_Start(void);
uint8_t DHT22_Read();
uint8_t DHT22_Check_Response(void);
