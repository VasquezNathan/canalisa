#include <avr/io.h>
#include "spi.h"

// Pin definitions for PORTD.
#define SCK PORTD4
#define SI  PORTD5
#define SO  PORTD6
#define CS  PORTD7

// Macros for settings pins high or low.
#define SCK_HIGH()  (PORTD |= (1 << SCK))
#define SCK_LOW()   (PORTD &= ~(1 << SCK))
#define SI_HIGH()   (PORTD |= (1 << SI))
#define SI_LOW()    (PORTD &= ~(1 << SI))
#define SO_STATUS() (PIND & (1 << SO))
#define CS_HIGH()   (PORTD |= (1 << CS))
#define CS_LOW()    (PORTD &= ~(1 << CS))

SPI::SPI() {
    // Set data out for PORTD pins 7,6, and 4
    // SCK, SI, CS respectively. PORTD 5 will be SO.

    // SCK, SI, CS out
    DDRD |= (1 << SCK) | (1 << SI) | (1 << CS);

    // SO in
    DDRD &= ~(1 << SO);

    // Internal pull up resistor.
    PORTD |= (1 << SO);
}

void SPI::send_instruction(uint8_t instruction, uint8_t data, uint8_t *response ) {
    // init response.
    *response = 0b00000000;
    
    // select chip.
    CS_HIGH();
    CS_LOW();
    
    // For every bit in the instruction.
    for (int8_t i = 7; i >= 0; i--) {
        // Toggle MOSI line.
        (instruction & (1 << i)) ? SI_HIGH() : SI_LOW();

        // MOSI read on SCK rising edge.
        SCK_LOW();
        SCK_HIGH();

    }

    // For every bit in the data.
    for (int8_t i = 7; i >= 0; i--) {
        // Toggle MOSI line.
        (data & (1 << i)) ? SI_HIGH() : SI_LOW();

        // MOSI read on rising edge.
        SCK_LOW();
        SCK_HIGH();
    }

    // For every bit of the response.
    for (int8_t i = 7; i >= 0; i--) {
        // MISO read on falling edge.
        SCK_HIGH();
        SCK_LOW();

        // Read MISO.
        if (SO_STATUS()) *response |= (1 << i);
    }

    

    // raise chip select to complete instruction.
    CS_HIGH();

    return;
}

