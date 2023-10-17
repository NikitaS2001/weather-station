#include <stm32f1xx.h>
#include "i2c.h"
#include "delay.h"
#include "BMP280.h"

#define DELAYSTAMP 600'000 // 10 minutes

Delay_TypeDef delay;

float tem = 0;
float press = 0;
float hum = 0;

int main(void)
{
    DWT_Init();
    I2C1_Init(I2C_FAST);
    BMP280_Init(BMP280_Mode_Type::BMP280_Normal);

    for(;;)
    {
        if (DWT_nb_timeout(&delay))
        {
            BMP280_GetTemperatureAndPressure(&tem, &press);
            DWT_nb_delay_ms(&delay, DELAYSTAMP);
        }
    }

    return 0;
}