#include "spi.h"

#include "stm32f042k6.h"
// Update spi.c's spi_init() function.
//  You will only be connecting a single device, the LIS3DH, to the STM32F042K6's SPI peripheral so 
//you can set all SPI communications parameters (SPI bus speed, clock phase, clock polarity, data width) in spi_init().  You do not need to (but you are welcome to) generalize SPI read/write transfers to support multiple SPI devices on the SPI bus in this design.

// *Grading Rubric [10 points total]*:
// * -2: for each missing configuration setting necessary for proper functionality
// * -2: inadequate commenting
// * -2: "magic numbers"
#define setting baud rate = 
void spi_init(void) {
    //Disable SPI
    SPI1 ->CR1.SPE = 0;
    //data sheet table 2.4.1 Max Mhz spi communcation rate
    // Baud rate sertting
    SPI1 -> CR1.BR = 0b011;
    //Setting clock polarity
    SPI1 -> CR1.CPOL = 0b1;
    //Setting clock phase
    SPI1 -> CR1.CPHA = 0b1;
    //Setting data length to 16 bits
    SPI1 -> CR2.DS = 0b1111;
    
    //Slave managment
    SPI1->CR1.SSM  = 1;      // software slave management
    SPI1->CR1.SSI  = 1;      // internal NSS high
    
    //Setting master
    SPI1 ->CR1.MSTR = 1; 

    
    //Enable SPI
    SPI1 ->CR1.SPE = 1;
    // SPI1 ->CR1 | = ; 
    // SPI1 ->CR1 | = ; 
}

void spi_txfr_16(uint16_t tx_data, uint16_t *rx_data) {
    //Checking if transmitt buffer is empty
    
    // Wait until we are allowed to write to DR
    while (!SPI1->SR.TXE) {}

    SPI1->DR = (uint16_t)tx_data;

    // Wait until Receive buffer not empty RXNE = 1: Rx buffer not empty
    while (!SPI1->SR.RXNE) {}

    // Read received 16 bits (clears RXNE)
    *rx_data = (uint16_t)SPI1->DR;

    // Wait until SPI is no longer busy (transfer fully complete on the wire)
    while (SPI1->SR.BSY) {}

    // if (SPI1->SPI_SR_s.TXE ==1){
    //     while (SPI1->SPI_SR_s.BSY ==1) {
    //         SPI1->SPI_SR_s.DR = (uint32_t)tx_data;
    //         while (SR.RXNE ==1 ){
    //             rx_data = (SPI1->SPI_SR_s.DR&(0b000000001111111111111111));
    //         }
    //     }
       
    // }
}
