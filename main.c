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
// // Callback function called by External interupt at 10hz 
// void accel_callback_function(int16_t x, int16_t y, int16_t z){
//     printf("%6d,%6d,%6d\n", x, y, z);
// }

#define Z_QUEUE_LEN 20
#define RUNNING_Z_THRESHOLD 5000

static int32_t z_queue[Z_QUEUE_LEN];
static uint8_t z_queue_count = 0;

static void process_z_sample(int16_t z) {
    // Step 1: Fill the queue until we have 20 samples.
    if (z_queue_count < Z_QUEUE_LEN) {
        z_queue[z_queue_count] = z*z;
        z_queue_count = z_queue_count + 1;
    }
    else {
        // Step 2: Queue is full, so shift left by one.
        // Oldest value is dropped and new value is added at the end.
        for (uint8_t i = 0; i < (Z_QUEUE_LEN - 1); i++) {
            z_queue[i] = z_queue[i + 1];
        }
        z_queue[Z_QUEUE_LEN - 1] = z*z;
    }

    // Step 3: Only calculate average when we have all 20 values.
    if (z_queue_count == Z_QUEUE_LEN) {
        int32_t sum = 0;

        // Add all queue values.
        for (uint8_t i = 0; i < Z_QUEUE_LEN; i++) {
            sum += z_queue[i];
        }

        // Calculate average z.
        int16_t avg_z = (int16_t)(sum / Z_QUEUE_LEN);

        // If average is above threshold, we classify as running.
        if (avg_z < RUNNING_Z_THRESHOLD) {
            printf("Person is running (avg z = %d)\n", avg_z);
        }
        else {
             printf("Person is walking or standing (avg z = %d)\n", avg_z);
        }
    }
}

// Callback function called by External interupt at 10hz 
void accel_callback_function(int16_t x, int16_t y, int16_t z){
    printf("%6d,%6d,%6d\n", x, y, z);
    process_z_sample(z);
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