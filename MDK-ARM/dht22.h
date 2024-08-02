#include "stm32f4xx.h"

extern volatile uint8_t humid_th;
extern volatile uint8_t temperature_th;

void DHT22_Start(void);
uint8_t DHT22_Read();
uint8_t DHT22_Check_Response(void);
