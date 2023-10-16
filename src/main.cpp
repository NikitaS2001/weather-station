// #include <stm32f1xx.h>
#include <stm32f103x6.h>

#define DELAYSTAMP 600'000 // 10 minutes

volatile uint32_t timestamp = 600'000;

void SysTick_Handler(void);


int main(void)
{
    if (SysTick_Config(SystemCoreClock / 1000)) { return 1; }
    
    for(;;)
    {
        if (DELAYSTAMP < timestamp)
        {
            
            timestamp = 0;
        }
    }

    return 0;
}

void SysTick_Handler(void) 
{
  timestamp++;
}
