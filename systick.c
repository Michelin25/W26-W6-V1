#include "systick.h"

static void(*systick_cbfn)(void) = 0;

// Implement a 2 Hz SysTick timer event
void systick_init(void(*cbfn)(void)) {
    // register the callback function
    systick_cbfn = cbfn;

    // Configure a 2 Hz SysTick event by dividing HCLK/8 = 6 MHz by 
    // 3 million.  Note we have to subtract 1 from the desired divisor
    // per the STM32F042K6 programming manual section 4.4.2 
    // register description
    SYSTICK->RVR = 3000000-1;
    // Switch to the "external clock source" (HCLK/8 = 6 MHz), 
    // enable the counter,
    // and enable an exception request when the counter reaches 0
    SYSTICK->CSR = (SYSTICK_CSR_ENABLE | SYSTICK_CSR_TICKINT);
}

void SysTick_Handler(void) {
    if( systick_cbfn) 
        systick_cbfn();
}