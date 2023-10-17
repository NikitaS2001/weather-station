#include "delay.h"

#define MS_COEFF (SystemCoreClock / 1000)
#define S_COEFF (SystemCoreClock)

// -----------------------------------------------
// DWT Init
// -----------------------------------------------
void DWT_Init(){
	CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;		// Enable TRC
	DWT->CYCCNT = 0;									// Reset counter
	DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;				// Enable counter

	SystemCoreClockUpdate();							// Update SystemCoreClock variable
}

// -----------------------------------------------
// DWT Get Ticks
// -----------------------------------------------
uint32_t DWT_tick() {
	return DWT->CYCCNT;
}

// -----------------------------------------------
// DWT Get Milliseconds
// -----------------------------------------------
uint32_t DWT_millis(void) {
	return DWT_tick() / MS_COEFF;						// tick to ms
}

// -----------------------------------------------
// DWT Blocking Delay (ms)
// -----------------------------------------------
void DWT_delay_ms(uint32_t delay){
	delay *= MS_COEFF;									// ms to tick
	uint32_t t0 = DWT_tick();
	while((DWT_tick() - t0) < delay);
}

// -----------------------------------------------
// DWT Blocking Delay (s)
// -----------------------------------------------
void DWT_delay_s(uint32_t delay){
	delay *= S_COEFF;									// s to tick
	uint32_t t0 = DWT_tick();
	while((DWT_tick() - t0) < delay);
}

// -----------------------------------------------
// DWT Non Blocking Timeout (ticks)
// -----------------------------------------------
uint8_t DWT_nb_timeout(Delay_TypeDef *dt) {
	return ((DWT_tick() - dt->timestamp) > dt->delay);
}

// -----------------------------------------------
// DWT Non Blocking Delay (ms)
// -----------------------------------------------
void DWT_nb_delay_ms(Delay_TypeDef *dt, uint32_t delay) {
	dt->delay = delay * MS_COEFF;						// ms to tick
	dt->timestamp = DWT_tick();
}
