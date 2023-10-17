// #include <stm32f1xx.h>
#include <stm32f103x6.h>
#include "i2c.h"
#include "delay.h"

#define DELAYSTAMP 600'000 // 10 minutes

Delay_TypeDef bme280;

int main(void)
{
    if (SysTick_Config(SystemCoreClock / 1000)) { return 1; }
    
    I2C1_Init(I2C_FAST);

    for(;;)
    {
        if (DWT_nb_timeout(&bme280))
        {
            
            DWT_nb_delay_ms(&bme280, DELAYSTAMP);
        }
    }

    return 0;
}