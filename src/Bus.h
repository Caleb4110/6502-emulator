#ifndef INC_6502_BUS_H
#define INC_6502_BUS_H


#include "mp6502.h"

// Total size of memory
#define RAM_SIZE 0xFFFF

class Bus {
public:
    Bus();
    ~Bus();

    // Devices connected to the bus
    mp6502 cpu;

    // RAM
    uint8_t ram[RAM_SIZE] = {0x00}; // 64KB of RAM

    // Bus Read & Write
    void write(uint16_t addr, uint8_t data);
    uint8_t read(uint16_t addr, bool bReadOnly = false);
};


#endif //INC_6502_BUS_H
