#include "gd32vf103.h"
#include "systick.h"
#include "dht22/dht22.h"
#include "lcd/lcd.h"
#include <string.h>
#include <stdio.h>

char data[40];

/*!
    \brief      main function
    \param[in]  none
    \param[out] none
    \retval     none
*/
int main(void)
{
    int cnt = 0;

    char temp[10];
    char humid[10];

    char temp_display[18];
    char humid_display[18];
    char time_display[18];

    DHT22_Init();
    LCD_Init();
    LCD_Clear(WHITE);
    BACK_COLOR = WHITE;

    LCD_ShowString(16, 16, (u8 *)("By: d4nk3n"), BLUE);
    LCD_ShowString(16, 32, (u8 *)("Temp: "), BRED);
    LCD_ShowString(16, 48, (u8 *)("Humid: "), BROWN);
    while (1)
    {
        delay_1ms(5000);
        if (DHT22_READ(data, GPIOB, DHT22_PIN) == 0)
        {
            strcpy(temp, "fail ");
            strcpy(humid, "fail ");
        }
        else
        {
            DHT22_READTEMP(data, temp);
            DHT22_READHUMID(data, humid);
        }
        cnt++;
        sprintf(temp_display, "Temp: %s", temp);
        LCD_ShowString(16, 32, (u8 *)(temp_display), BRED);
        sprintf(humid_display, "Humid: %s", humid);
        LCD_ShowString(16, 48, (u8 *)(humid_display), BROWN);
        sprintf(time_display, "Read: %d", cnt);
        LCD_ShowString(16, 64, (u8 *)(time_display), BLACK);
    }
}
