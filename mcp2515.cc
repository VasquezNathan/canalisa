#include "mcp2515.h"

MCP2515::MCP2515() {

    // 8mhz crystal 1Mbps canbus.
    this-> reset();
    this->write(0x2a, MCP_8MHz_1000kBPS_CFG1);
    this->write(0x29, MCP_8MHz_1000kBPS_CFG2);
    this->write(0x28, MCP_8MHz_1000kBPS_CFG3);
}

void MCP2515::reset(void) {
    uint8_t * response;
    this->spi.send_instruction(INST_RESET, 0, response);
}

void MCP2515::read(uint8_t address, uint8_t * data) {
    this->spi.send_instruction(INST_READ, address, data);
}

void MCP2515::write(uint8_t address, uint8_t data) {
    this->spi.write_instruction(INST_WRITE, address, data);
}