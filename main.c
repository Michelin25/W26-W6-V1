#include <stdint.h>
#include <stdio.h>

#include "accel.h"
#include "led.h"
#include "sysinit.h"
#include "systick.h"
#include "usart.h"

#include "stm32f042k6.h"
volatile bool flag = false;
volatile uint8_t rx_dat;

// Send a character to the terminal window
int __io_putchar(int data) {
  usart2_tx((uint8_t)data);
  return data;
}
//Test

// Callback function for systick exceptions registered in systick_init()
// Toggle the onboard green LED and start an ADC conversion
void systick_callback_function(void) {
    led_toggle(LED_USER);
    
    //rx_dat = accel_whoami();
    //flag = 1;
}
// Callback function called by External interupt at 10hz 
void accel_callback_function(int16_t x, int16_t y, int16_t z){
    printf("%6d,%6d,%6d\n", x, y, z);//X,Y,Z
}

// Do nothing when a key is pressed
void usart2_rx_callback_function(uint8_t rx_data) {
}

int main(void) {
    //int16_t x, y, z;
    // Initialize clocks/peripherals and configure I/O
    sys_init();

    // Start the systick timer (2 Hz) and call the
    // systick_callback_function on timer events:
    systick_init(systick_callback_function);

    // Configure usart2 for 115200 baud, 8 data, no parit, 1 stop
    // call the usart2_rx_callback_function on received data
    usart2_init(usart2_rx_callback_function);

    // Enable the 3-axis accelerometer
    //Passing in the callback func to call upon interupt
    accel_init(accel_callback_function);

    // Enable exception/interrupt handling in the processor core:
    __asm("cpsie i");

    // Banner
    printf("Lab 5: Accelerometers and Motion Classification:\n");


    while( 1 ) {
        ;;; // Do nothing;  all work is done in interrupt handlers
    }
    // while( 1 ) {
    //     // Add some delay between reads so we do not hammer the SPI bus
    //     // unnecessarily (can lock up the accelerometer as well)
    //     for( int i = 0; i < 50000; i++ );
    //     // Check if new accelerometer data is available
    //     flag = accel_data_ready();
    //     //printf("%d\r\n", flag);

    //     if(flag) {
    //         // If so, read it out and print it to the terminal
    //         accel_read(&x, &y, &z);
    //         printf("Accel X: %6d  Y: %6d  Z: %6d\r\n", x, y, z);
    //     }
    // }
}