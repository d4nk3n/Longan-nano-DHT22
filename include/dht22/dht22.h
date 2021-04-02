#include "systick.h"
#include "gd32vf103_gpio.h"
#include <string.h>	

/*
DATA:
|39     32|31     24|23    17|16     8|7     0|
| humid_h | humid_l | temp_h | temp_l | check |
*/
#define DHT22_PIN GPIO_PIN_9

#define TEMP_L (8)
#define TEMP_H (23)

#define HUMID_L (24)
#define HUMID_H (39)

void DHT22_Init();
void pin_pullup(uint32_t BASE, uint32_t PIN);
void pin_pulldown(uint32_t BASE, uint32_t PIN);
int getSignal(int usTimeout, FlagStatus state, uint32_t BASE, uint32_t PIN);

int DHT22_READ(char* data, uint32_t BASE, uint32_t PIN);
void DHT22_READTEMP(char* data, char* temp_s);
void DHT22_READHUMID(char* data, char* humid_s);
int DHT22_CHECKOK(char* data);
