class SPI {
    private:
        volatile uint8_t *spiPort;
    public:

        /* initialize pins 3-7 (inclusive) on PORTD for SPI*/
        SPI();
        void send_instruction(uint8_t instruction, uint8_t data, uint8_t *response);
};