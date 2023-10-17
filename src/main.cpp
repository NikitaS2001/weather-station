#include <stm32f1xx.h>
#include "i2c.h"
#include "delay.h"

#define DELAYSTAMP 600'000 // 10 minutes

Delay_TypeDef delay;

int main(void)
{
    DWT_Init();
    I2C1_Init(I2C_FAST);


    for(;;)
    {
        if (DWT_nb_timeout(&delay))
        {
            
            DWT_nb_delay_ms(&delay, DELAYSTAMP);
        }
    }

    return 0;
}