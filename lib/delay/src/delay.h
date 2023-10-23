#ifndef __DELAY_H__
#define __DELAY_H__

#include <stm32f1xx_hal.h>

#define MINUTES_10 600000
typedef struct
{
    uint32_t delay;
    uint32_t timestamp;
} Delay;

void Delay_nb_ms(Delay *dt, uint32_t delay);
int8_t Delay_nb_timeout(Delay *dt);

#endif /* __DELAY_H__ */