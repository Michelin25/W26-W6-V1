#include "accel.h"

#include "exti.h"
#include "nvic.h"
#include "spi.h"
#include "stm32f042k6.h"

#include <stdio.h>

#include <stdbool.h>
#include <stdint.h>


//#define read_req;
uint16_t rx_data = 0;
uint16_t tx_data; 

#define RX8(w) ((uint8_t)((w) & 0xFF))
 //I used Chat GPT for figuring out this line the code was getting messy and 
//so I asked it hoe to make it cleaner and it suggested a idefine the manipulation 
//in this way. Which is super cool especially given that I am learning about C synatx...

//Registering the callback function handling interupts
static void (*accel_cf)(int16_t x, int16_t y, int16_t z) = 0;
// should I add this to the header? If not why not

bool accel_data_ready(void) {
    
    //read request
    tx_data =0xA700;

    gpio_pin_reset(GPIOA, gpio_pin_4);

    // Perform SPI transfer
    spi_txfr_16(tx_data, &rx_data);

    // Set chip select high
    gpio_pin_set(GPIOA, gpio_pin_4);
    //rx_data = (rx_data & 0xFF)
    
    //printf("%d,%d",(rx_data >> 8) & 0xFF,rx_data & 0xFF);
    //printf(accel_whoami());
    
    if ((RX8(rx_data) & 0x08) != 0){
        return true;
    } 
    return false;
}

void accel_read(int16_t *x, int16_t *y, int16_t *z) {
    uint16_t byt1;
    uint16_t byt2;

    //read request
    gpio_pin_reset(GPIOA, gpio_pin_4);

    // reading x 
    spi_txfr_16(0xA800, &byt1);

    // // Set chip select high
    gpio_pin_set(GPIOA, gpio_pin_4);

    gpio_pin_reset(GPIOA, gpio_pin_4);
    // reading x high
    spi_txfr_16(0xA900, &byt2);
    
    //extracting the bytes and sticking them toghether, and than shifting them by 4
    uint8_t xl = RX8(byt1);
    uint8_t xh = RX8(byt2);
    *x = (int16_t)((((uint16_t)xh << 8) | xl)) >> 4;


    // // Set chip select high
    gpio_pin_set(GPIOA, gpio_pin_4);

    gpio_pin_reset(GPIOA, gpio_pin_4);

    // reading y 
    spi_txfr_16(0xAA00, &byt1);//2A
    
    // Set chip select high
    gpio_pin_set(GPIOA, gpio_pin_4);

    gpio_pin_reset(GPIOA, gpio_pin_4);

    // reading y high 
    spi_txfr_16(0xAB00, &byt2);

    //extracting the bytes and sticking them toghether, and than shifting them by 4
    uint8_t yl = RX8(byt1);
    uint8_t yh = RX8(byt2);
    *y = (int16_t)((((uint16_t)yh << 8) | yl)) >> 4;

    // // Set chip select high
    gpio_pin_set(GPIOA, gpio_pin_4);
    
    gpio_pin_reset(GPIOA, gpio_pin_4);

    // reading z
    spi_txfr_16(0xAC00, &byt1);
        // // Set chip select high
    gpio_pin_set(GPIOA, gpio_pin_4);

    gpio_pin_reset(GPIOA, gpio_pin_4);

    // reading z high
    spi_txfr_16(0xAD00, &byt2);

    //extracting the bytes and sticking them toghether, and than shifting them by 4
    uint8_t zl = RX8(byt1);
    uint8_t zh = RX8(byt2);
    *z = (int16_t)((((uint16_t)zh << 8) | zl)) >> 4;

    // Set chip select high
    gpio_pin_set(GPIOA, gpio_pin_4);
}

//Setting up the accelrometer, this incudes setting up the interupts on LIS3DH and on the STM, configuring EXTI peripheral to generate interrupts
// on a rising edge on PORT A Pin 0. Configures the NVIC to pass EXTI0
// Accepts (a "callback") function pointer argument for when read is ready
void accel_init(void (*accel_cf_imp)(int16_t x, int16_t y, int16_t z)) {
    
    //Registering the callback function locally
    accel_cf = accel_cf_imp;

    // I need to change the internal registers to get the 10Hz
    spi_init();
    uint16_t tx_data = 0x2027;//0b1000111100000000; 

    //To test the communictation
    // 0. Set CS pin low
    // 1. Send a read request
    // 2. return data recieved
    //CS pin infrastructure
    // Set chip select low

    gpio_pin_reset(GPIOA, gpio_pin_4);
    //read request

    // Perform SPI transfer
    spi_txfr_16(tx_data, &rx_data);
    
    // Set chip select high
    gpio_pin_set(GPIOA, gpio_pin_4);
    
    //Enabling high resolution
    gpio_pin_reset(GPIOA, gpio_pin_4);
    //read request

    // Perform SPI transfer
    spi_txfr_16(0x2308, &rx_data);
    
    // Set chip select high
    gpio_pin_set(GPIOA, gpio_pin_4);

    //Testing
    gpio_pin_reset(GPIOA, gpio_pin_4);
    //read request
    tx_data =0xA000; //0xA027
    // Perform SPI transfer
    spi_txfr_16(tx_data, &rx_data);

    // Set chip select high
    gpio_pin_set(GPIOA, gpio_pin_4);


    //Checking if the rgister update was succesful
    if (RX8(rx_data) != 0x27) {
        while(true){}
    }

    //Enabling the interupt:
    tx_data = 0x2210;  

    gpio_pin_reset(GPIOA, gpio_pin_4);
    //read request

    // Perform SPI transfer
    spi_txfr_16(tx_data, &rx_data);
    
    // Set chip select high
    gpio_pin_set(GPIOA, gpio_pin_4);

    //Setting up External interupts

    //Enabling the ADC interupt to reach the M0 core
    NVIC_ISER |= NVIC_ISER_SETENA_EXTIO_1;
    //For internal refference and debugging
    //#define EXTI_IMR_IM0    (0x00000001)
    //#define EXTI_EMR_EM0    (0x00000001)
    //#define EXTI_RTSR_RT0   (0x00000001)
    EXTI->IMR  |= EXTI_IMR_IM0;    // enable EXTI0 interupts being apssed on
    EXTI->RTSR |= EXTI_RTSR_RT0;   // rising edge
   

    int16_t x, y, z;
    // If the interrupt line is already high we 
    // won't catch it because we are configured to
    // detect rising edges, not "high state", 
    // so we need to check for a high interrupt line 
    // here and, if high, read data to clear it
    if( GPIOA->IDR & gpio_pin_0 ) {
        
        //Clearing the pending interupt bit
        EXTI->PR = EXTI_PR_PR0;
        //Reading it to set the interupt pin low
        accel_read(&x, &y, &z);
    }
    
}

// Verify the accelerometer's WHO_AM_I register
// uint8_t accel_whoami(void) {
//     uint16_t tx_data = 0x8f00;//0b1000111100000000; 
   
//     gpio_pin_reset(GPIOA, gpio_pin_4);
//     //read request

//     // Perform SPI transfer
//     spi_txfr_16(tx_data, &rx_data);

//     // Set chip select high
//     gpio_pin_set(GPIOA, gpio_pin_4);
//     return ((uint8_t)rx_data);
// }

void EXTI0_1_IRQHandler(void){
    int16_t x,y,z;
    //Check that the callback function pointer is non-null before calling
    if (accel_cf == NULL) {
        printf("Undefined callback");
        while(true){}
    }
    

    //Clears the EXTI interrupt source, since we aren't using any
    EXTI->PR = EXTI_PR_PR0;
    //Read the latest X,Y,Z acceleration values
    accel_read(&x,&y,&z);
    //Passes the latest X,Y,Z acceleration values to the callback function pointer registered in accel_init()
    accel_cf(x,y,z);

}

