#include "dht22/dht22.h"
#include "lcd/lcd.h"
#include <string.h>
#include <stdio.h>

void DHT22_Init()
{
    rcu_periph_clock_enable(RCU_GPIOB);
    gpio_init(GPIOB, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, DHT22_PIN);
    GPIO_BOP(GPIOB) = DHT22_PIN;
}

void pin_pullup(uint32_t BASE, uint32_t PIN)
{
    GPIO_BOP(BASE) = PIN;
}

void pin_pulldown(uint32_t BASE, uint32_t PIN)
{
    GPIO_BC(BASE) = PIN;
}

/*!
    \brief          get raw data 
    \param[data]    data, 40 bytes
    \retval         1 if success, else return 0
*/
int DHT22_READ(char* data, uint32_t BASE, uint32_t PIN)
{
    for (int i = 0; i < 40; i++)
    {
        data[i] = 0;
    }
    pin_pulldown(BASE, PIN);
    delay_1ms(1);
    pin_pullup(BASE, PIN);
    delay_1us(30);
    gpio_init(GPIOB, GPIO_MODE_IPU, GPIO_OSPEED_50MHZ, PIN);
    int uSec = 0;
    uSec = getSignal(86, RESET, BASE, PIN);
    if (uSec < 0)
    {
        return 0;
    }
    uSec = getSignal(86, SET, BASE, PIN);
    if (uSec < 0)
    {
        return 0;
    }
    for (int k = 39; k > -1; k--) {

        uSec = getSignal(56, RESET, BASE, PIN);
        if (uSec < 0) return 0;

        uSec = getSignal(75, SET, BASE, PIN);
        if (uSec < 0) return 0;

        if (uSec > 30) {
            data[k] = 1;
        }
    }
    delay_1ms(1);
    gpio_init(GPIOB, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, PIN);
    pin_pullup(BASE, PIN);
    return DHT22_CHECKOK(data);
}


int getSignal(int usTimeout, FlagStatus state, uint32_t BASE, uint32_t PIN)
{
    int uSec = 0;
    while (gpio_input_bit_get(BASE, PIN) == state)
    {
        if (uSec > usTimeout)
        {
            return -1;
        }
        ++uSec;
        delay_1us(1);
    }
    return uSec;
}


/*!
    \brief          get temperature 
    \param[data]    data, 40 bytes
    \param[temp_s]  temp string, 10 bytes
    \retval         none
*/
void DHT22_READTEMP(char* data, char* temp_s)
{
    if (data[TEMP_H] == 1)
        strcpy(temp_s, "-");
    else
        strcpy(temp_s, "");

    int temp = 0;
    int cnt = 1;
    for (int i = TEMP_L; i < TEMP_H; i++)
    {
        temp += (data[i] == 1) ? cnt : 0;
        cnt <<= 1;
    }

    char dights[5] = "00.0";
    dights[0] = temp / 100 + 48;
    dights[1] = ((temp / 10) % 10) + 48;
    dights[3] = (temp % 10) + 48;
    strcat(temp_s, dights);
}

/*!
    \brief          get humid 
    \param[data]    data, 40 bytes
    \param[humid_s] temp string, 10 bytes
    \retval         none
*/
void DHT22_READHUMID(char* data, char* humid_s)
{
    strcpy(humid_s, "");
    int humid = 0;
    int cnt = 1;
    for (int i = HUMID_L; i < HUMID_H; i++)
    {
        humid += (data[i] == 1) ? cnt : 0;
        cnt <<= 1;
    }

    char dights[6] = "00.0\%";
    dights[0] = humid / 100 + 48;
    dights[1] = ((humid / 10) % 10) + 48;
    dights[3] = (humid % 10) + 48;
    strcat(humid_s, dights);
}


/*!
    \brief          check whether input data is valid
    \param[data]    data, 40 bytes
    \retval         1 if valid, else return 0
*/
int DHT22_CHECKOK(char* data)
{
    char out[20];
    int bitdata[5] = {0, 0, 0, 0, 0};
    int tmp = 0;
    int cnt = 1;
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            tmp += (data[32 - (8 * i) + j] == 1) ? cnt : 0;
            cnt <<= 1;
        }
        bitdata[i] = tmp;
        cnt = 1;
        tmp = 0;
    }

    sprintf(out, "Data: %02x%02x%02x%02x%02x", bitdata[0], bitdata[1], bitdata[2], bitdata[3], bitdata[4]);
    LCD_ShowString(16, 0, (u8 *)(out), BLACK);

    return (bitdata[4] == ((bitdata[0] + bitdata[1] + bitdata[2] + bitdata[3]) & 0xFF));
}
