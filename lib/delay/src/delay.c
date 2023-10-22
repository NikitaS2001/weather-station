#include <stm32f1xx_hal.h>
#include "delay.h"

void Delay_nb_ms(Delay *dt, uint32_t delay)
{
    dt->delay = delay;
    dt->timestamp = HAL_GetTick();
}

int8_t Delay_nb_timeout(Delay *dt)
{
    return (int8_t)((HAL_GetTick() - dt->timestamp) > dt->delay);
}