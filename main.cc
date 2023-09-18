#include <avr/io.h>
#include <avr/interrupt.h>
// #include "spi.h"
#include "mcp2515.h"

#define F_CPU 16000000UL // ATmega328P clock frequency
#define BAUD 9600       // Desired baud rate

// Function prototypes
void USART_TX_HEX(uint8_t data);
void initUSART();
void USART_transmit(uint8_t data);
uint8_t USART_receive();

int main(void) {

    // Make data out for pin b5
    DDRB |= (1 << DDB5);

    // Initialize USART
    initUSART();
    
    // MCP2515 controller object.
    MCP2515 mcp2515 = MCP2515();

    // Serial out hex data from mcp2515.
    uint8_t response;
    mcp2515.read(0x29, &response);
    USART_TX_HEX(response);

    // Enable global interrupts
    sei();

    while (1) {
        // Your main program logic here
        for (int i=0; i < 10; i++) {}
    }

    return 0;
}

void initUSART() {
    // Set baud rate registers based on F_CPU and BAUD
    uint16_t ubrr_value = F_CPU / (16UL * BAUD) - 1;
    UBRR0H = (uint8_t)(ubrr_value >> 8);
    UBRR0L = (uint8_t)ubrr_value;

    // Enable receiver and transmitter, and enable RX interrupt
    UCSR0B = (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0);

    // Set frame format: 8 data bits, 1 stop bit, no parity
    // UCSR0C = (3 << UCSZ00);

    // Set frame format: 8 data, 2 stop bit.
    UCSR0C |= (1 << USBS0) | (3 << UCSZ00);
}

void USART_TX_HEX(uint8_t data) {
    const char * hexTable = "0123456789abcdef";
    uint8_t dataLow = hexTable[(data & 0x0f)];
    uint8_t dataHigh = hexTable[(data & 0xf0) >> 4];

    USART_transmit('0');
    USART_transmit('x');
    USART_transmit(dataHigh);
    USART_transmit(dataLow);
    USART_transmit('\n');
    USART_transmit('\r');
}

void USART_transmit(uint8_t data) {
    // Wait until the transmit buffer is empty
    /* Check the USART Data Register Empty bit of the 
    USART Control and Status Register A. 
    I'm curious if nothing else puts data in UDR0 I/O
    if it is necessary to have this while loop. */
    while (!(UCSR0A & (1 << UDRE0)));

    // Put the data into the transmit buffer
    UDR0 = data;
}

uint8_t USART_receive() {
    // Wait until data is received
    /* If this function is only invoked by USART
    receive complete ISR then we know RXC0 of UCSR0A
    ~should~ be set skip check */
    // while (!(UCSR0A & (1 << RXC0)));
    
    // Return the received data
    return UDR0;
}

// USART receive complete interrupt handler
ISR(USART_RX_vect) {
    // Receive data from USART
    uint8_t receivedData = USART_receive();

    // Your code to process receivedData here

    // Echo the received data back to the sender
    USART_transmit(receivedData);
}