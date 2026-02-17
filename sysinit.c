#include "gpio.h"
#include "rcc.h"

#include <stdio.h>

#include "stm32f042k6.h"

// Switch to the 48MHz internal oscillator
static void sys_clock_init(void) {
    // Enable the oscillator (reference manual section 6.4.14)
    RCC->CR2 |= RCC_CR2_HSI48ON;
    // Wait for it to stabilize (reference manual section 6.4.14)
    while( !(RCC->CR2 & RCC_CR2_HSI48RDY) );
    // Switch to using the HSI48 clock source for SYSCLK/HCLK 
    // (reference manual section 6.4.2)
    RCC->CFG.SW = RCC_CFG_SW_HSI48;
    
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGCOMP_EN;
        
}

// Configure General Purpose IO (e.g. the LED on Port B pin 3)
// NOTE that only GPIO clocks for general purpose IO are enabled
// here.  "GPIO peripheral" clocks and port configurations for
// other periperals (e.g. USART, Timer, ADC) are managed in 
// their respective *_init() functions
static void sys_gpio_init(void) {
    // Enable the GPIOA and GPIOB Peripheral Clocks
    RCC->AHBENR |= RCC_AHBENR_IOPA_EN;
    RCC->AHBENR |= RCC_AHBENR_IOPB_EN;

    // Set Port A pins 3 and 4 and Port B pin 3 to output mode 
    GPIOA->MODER.moder3 = gpio_mode_output;

  
    GPIOA->MODER.moder4 = gpio_mode_output;

    gpio_pin_set(GPIOA, gpio_pin_4);

    
    //Setting as input
    GPIOA->MODER.moder0 = gpio_mode_input; 
    

    // Set Port B pin 3 to output mode (LED)
    GPIOB->MODER.moder3 = gpio_mode_output;
}

// Enable USART2 and configure to use Port A pins 2 (TX) and 15 (RX)
static void sys_usart2_init(void) {
    // Enable the USART2 Communications Interface Peripheral's Clock 
    RCC->APB1ENR |= RCC_APB1ENR_USART2_EN;
    // Reset the USART2 peripheral to out-of-reset defaults
    RCC->APB1RSTR |= RCC_APB1RSTR_USART2_RST;
    RCC->APB1RSTR &= ~RCC_APB1RSTR_USART2_RST;

    // Configure Port A pins 2 and 15 for USART2 TX and RX functionality
    RCC->AHBENR |= RCC_AHBENR_IOPA_EN;
    GPIOA->MODER.moder2 = gpio_mode_alternate_function;  // USART TX
    GPIOA->AFRL.afsel2 = 1; // USART2 alternate function TX
    GPIOA->MODER.moder15 = gpio_mode_alternate_function; // USART RX
    GPIOA->AFRH.afsel15 = 1; // USART2 alternate function RX
}

void sys_spi_init(void){
    // Enable the SP1 Communications Interface Peripheral's Clock 
    RCC->APB2ENR |= RCC_APB2ENR_SPI1_EN;

    // Reset the SPI1 peripheral to out-of-reset defaults
    RCC->APB2RSTR |= RCC_APB2RSTR_SPI1_RST;
    RCC->APB2RSTR &= ~RCC_APB2RSTR_SPI1_RST;

    // Configure Port A pins 4 to 7 for SPI communcication
    RCC->AHBENR |= RCC_AHBENR_IOPA_EN;

    //CS pin 4 used for CS is already set as output in GPIO section
    // GPIOA->MODER.moder4 = gpio_mode_alternate_function;
    // GPIOA->AFRL.afsel4 = 0;
    //SCK
    GPIOA->MODER.moder5 = gpio_mode_alternate_function;
    GPIOA->AFRL.afsel5 = 0; 
    //MISO
    GPIOA->MODER.moder6 = gpio_mode_alternate_function;
    GPIOA->AFRL.afsel6 = 0; 
    //MOSI
    GPIOA->MODER.moder7 = gpio_mode_alternate_function; 
    GPIOA->AFRL.afsel7 = 0; 

}


// Initialize system out of reset
void sys_init(void) {
    sys_clock_init();   
    sys_gpio_init();
    sys_usart2_init();
    sys_spi_init();
}


