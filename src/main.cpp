#include <stm32f1xx.h>
#include <stdio.h>
#include "i2c.h"
#include "delay.h"
#include "BMP280.h"
#include "PCF8574.h"

#define DELAYSTAMP 600'000 // 10 minutes

Delay_TypeDef delay;

float tem = 0;
float press = 0;
float hum = 0;

char buffer[50] = {0};

char start[] = "start";

int main(void)
{
    DWT_Init();
    I2C1_Init(I2C_STANDARD);
    BMP280_Init(BMP280_Normal);
    LCD_Init();

    LCD_Clear();
    // LCD_SendData('H');
    // LCD_SendString(start);

    for(;;)
    {

        if (DWT_nb_timeout(&delay))
        {
            // BMP280_GetTemperatureAndPressure(&tem, &press);
            sprintf(buffer, "T: %f", tem);
            // LCD_SendString(buffer);

            DWT_nb_delay_ms(&delay, DELAYSTAMP);
        }
    }

    return 0;
}