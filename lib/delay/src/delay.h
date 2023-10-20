#pragma once

#include <stm32f1xx_hal.h>

typedef struct
{
    uint32_t delay = 0;
    uint32_t timestamp = 0;
} Delay;

void Delay_nb_ms(Delay *dt, uint32_t delay);
bool Delay_nb_timeout(Delay *dt);
