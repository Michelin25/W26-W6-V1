#pragma once

#include <stdint.h>

typedef volatile struct SPI_CR1_s {
    uint32_t CPHA       : 1;
    uint32_t CPOL       : 1;
    uint32_t MSTR       : 1;
    uint32_t BR         : 3;
    uint32_t SPE        : 1;  // Enable SPI
    uint32_t LSBFIRST   : 1;
    uint32_t SSI        : 1;
    uint32_t SSM        : 1;
    uint32_t RXONLY     : 1;
    uint32_t CRCL       : 1;
    uint32_t CRCNEXT    : 1;
    uint32_t CRCEN      : 1;
    uint32_t BIDIOE     : 1;
    uint32_t BIDIMODE   : 1;
    uint32_t unused     : 16;
} spi_cr1_t;

typedef volatile struct SPI_CR2_s {
    uint32_t RXDMAEN    : 1;
    uint32_t TXDMAEN    : 1;
    uint32_t SSOE       : 1;
    uint32_t NSSP       : 1;
    uint32_t FRF        : 1;
    uint32_t ERRIE      : 1;
    uint32_t RXNEIE     : 1;
    uint32_t TXEIE      : 1;
    uint32_t DS         : 4;
    uint32_t FRXTH      : 1;
    uint32_t LDMA_RX    : 1;
    uint32_t LDMA_TX    : 1;
    uint32_t RES        : 1;
    uint32_t unused     : 16;
} spi_cr2_t;

typedef volatile struct SPI_SR_s {
    uint32_t RXNE       : 1;
    uint32_t TXE        : 1;
    uint32_t CHSIDE     : 1;
    uint32_t UDR        : 1;
    uint32_t CRCERR     : 1;
    uint32_t MODF       : 1;
    uint32_t OVR        : 1;
    uint32_t BSY        : 1;
    uint32_t FRE        : 1;
    uint32_t FRLVL      : 2;
    uint32_t FTLVL      : 2; 
    uint32_t RES        : 3;
    uint32_t unused     : 16;
} spi_sr_t;

typedef volatile struct SPI_s {
    spi_cr1_t CR1;           // 0x00 - Config 1
    spi_cr2_t CR2;           // 0x04 - Config 2
    spi_sr_t SR;             // 0x08 - Status Register
    uint32_t DR;             // 0x0C - Data Register
    uint32_t CRCPR;          // 0x10 - CRC config
    uint32_t RXCRC;          // 0x14 - Received CRC
    uint32_t TXCRC;          // 0x18 - Transmitted CRC
    uint32_t I2SCFGR;        // 0x1C - I2S Config
    uint32_t I2SPR; 
} SPI_t;

void spi_init(void);

// Blocking 16-bit single transfer
void spi_txfr_16(uint16_t tx_data, uint16_t *rx_data);