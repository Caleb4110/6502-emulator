#include "Bus.h"

Bus::Bus() {
    // Connect CPU to communication bus
    cpu.connect_bus(this);
}

Bus::~Bus() = default;

void Bus::write(uint16_t addr, uint8_t data) {
    if (addr >= 0x0000 && addr <= RAM_SIZE)
        ram[addr] = data;
}

uint8_t Bus::read(uint16_t addr, bool bReadOnly) {
    if (addr >= 0x0000 && addr <= RAM_SIZE)
        return ram[addr];

    return 0x00;
}