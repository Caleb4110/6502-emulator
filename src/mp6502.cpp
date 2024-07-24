#include "mp6502.h"

// Constructor
mp6502::mp6502() = default;
// Destructor
mp6502::~mp6502() = default;

// Helper functions
uint8_t mp6502::get_flag(FLAGS flag) {
    return flags & flag;
}

void mp6502::set_flag(FLAGS flag, bool value) {
    if (value) {
        flags |= flag;
    } else {
        flags &= ~flag;
    }
}

uint8_t mp6502::fetch() {
    if (lookup[opcode].addr_mode != &mp6502::IMP)
        data = read(addr_abs);
    return data;
}

void mp6502::connect_bus(Bus *b) {
    bus = b;
}

uint8_t mp6502::read(uint16_t addr) {
    return bus->read(addr, false);
}

void mp6502::write(uint16_t addr, uint8_t data) {
    bus->write(addr, data);
}

//++++++++++++++++++++++++++++ADDRESSING MODES+++++++++++++++++++++++++++++++++++++//

uint8_t mp6502::IMP() {
    data = acc;
    return 0;
}

uint8_t mp6502::IMM() {
    addr_abs = pc++;
    return 0;
}

uint8_t mp6502::ZP0() {
    addr_abs = read(pc) & 0x00FF;
    pc++;
    return 0;
}

uint8_t mp6502::ZPX() {
    addr_abs = (read(pc) + reg[X]) & 0x00FF;
    pc++;
    return 0;
}

uint8_t mp6502::ZPY() {
    addr_abs = (read(pc) + reg[Y]) & 0x00FF;
    pc++;
    return 0;
}

uint8_t mp6502::REL() {
    addr_rel = read(pc);
    pc++;
    if (addr_rel & 0x80)
        addr_rel |= 0xFF00;
    return 0;
}

uint8_t mp6502::ABS() {
    uint8_t lo = read(pc);
    pc++;
    uint16_t hi = read(pc);
    pc++;

    addr_abs = (hi << 8) | lo;
    return 0;
}

uint8_t mp6502::ABX() {
    uint8_t lo = read(pc);
    pc++;
    uint16_t hi = read(pc);
    pc++;

    addr_abs = (hi << 8) | lo;
    addr_abs += reg[X];

    if ((addr_abs & 0xFF00) != (hi << 8))
        return 1;
    return 0;
}

uint8_t mp6502::ABY() {
    uint8_t lo = read(pc);
    pc++;
    uint16_t hi = read(pc);
    pc++;

    addr_abs = (hi << 8) | lo;
    addr_abs += reg[Y];

    if ((addr_abs & 0xFF00) != (hi << 8))
        return 1;
    return 0;
}

uint8_t mp6502::IND() {
    uint8_t addr_lo = read(pc);
    pc++;
    uint16_t addr_hi = read(pc);
    pc++;

    uint16_t addr_ptr = (addr_hi << 8) | addr_lo;
    if (addr_lo == 0x00FF) // Page boundary bug
        addr_abs = (read(addr_ptr & 0xFF00) << 8) | read(addr_ptr);
    else // Normal operation
        addr_abs = (read(addr_ptr + 1) << 8) | read(addr_ptr);
    return 0;
}

uint8_t mp6502::IZX() {
    uint16_t t = read(pc);
    pc++;

    uint8_t lo = read(t + reg[X] & 0x00FF);
    uint16_t hi = read((t + reg[X] + 1) & 0x00FF);

    addr_abs = (hi << 8) | lo;
    addr_abs += reg[X];
    return 0;
}

uint8_t mp6502::IZY() {
    uint16_t t = read(pc);
    pc++;

    uint8_t lo = read(t & 0x00FF);
    uint16_t hi = read((t + 1) & 0x00FF);

    addr_abs = (hi << 8) | lo;
    addr_abs += reg[Y];

    if ((addr_abs & 0xFF00) != (hi << 8))
        return 1;
    return 0;
}

//+++++++++++++++++++++++++++++++++OPCODES++++++++++++++++++++++++++++++++++++++++//

uint8_t mp6502::LDA() {
    // Fetch data
    fetch();

    // Load data into accumulator
    acc = data;

    // Set flags
    set_flag(Z, acc == 0x00);
    set_flag(N, acc & 0x80);

    return 1;
}

uint8_t mp6502::LDX() {
    // Fetch data
    fetch();

    // Load data into X register
    reg[X] = data;

    // Set flags
    set_flag(Z, reg[X] == 0x00);
    set_flag(N, reg[X] & 0x80);

    return 1;
}

uint8_t mp6502::LDY() {
    // Fetch data
    fetch();

    // Load data into Y register
    reg[Y] = data;

    // Set flags
    set_flag(Z, reg[Y] == 0x00);
    set_flag(N, reg[Y] & 0x80);

    return 0;
}

uint8_t mp6502::STA() {
    write(addr_abs, acc);
    return 0;
}

uint8_t mp6502::STX() {
    write(addr_abs, reg[X]);
    return 0;
}

uint8_t mp6502::STY() {
    write(addr_abs, reg[Y]);
    return 0;
}

uint8_t mp6502::TAX() {
    reg[X] = acc;


    return 0;
}

uint8_t mp6502::TAY() {
    // Implementation goes here
    return 0;
}

uint8_t mp6502::TXA() {
    // Implementation goes here
    return 0;
}

uint8_t mp6502::TYA() {
    // Implementation goes here
    return 0;
}

uint8_t mp6502::TSX() {
    // Implementation goes here
    return 0;
}

uint8_t mp6502::TXS() {
    // Implementation goes here
    return 0;
}

uint8_t mp6502::PHA() {
    // Implementation goes here
    return 0;
}

uint8_t mp6502::PHP() {
    // Implementation goes here
    return 0;
}

uint8_t mp6502::PLA() {
    // Implementation goes here
    return 0;
}

uint8_t mp6502::PLP() {
    // Implementation goes here
    return 0;
}

uint8_t mp6502::AND() {

    // Fetch data
    fetch();

    // Perform AND operation
    temp = acc & data;

    // Set flags
    set_flag(Z, temp == 0x0);
    set_flag(N, temp & 0x80);

    acc = temp;

    return 0;
}

uint8_t mp6502::EOR() {
    // Implementation goes here
    return 0;
}

uint8_t mp6502::ORA() {
    // Implementation goes here
    return 0;
}

uint8_t mp6502::BIT() {
    // Implementation goes here
    return 0;
}

uint8_t mp6502::ADC() {
    // Fetch data
    fetch();

    // Perform addition
    temp = acc + data + get_flag(C);

    // Set flags
    set_flag(C, temp > 255);
    set_flag(Z, temp == 0);
    set_flag(V, ~(acc ^ data) & (acc ^ temp) & 0x80);
    set_flag(N, temp & 0x80);

    acc = temp & 0x00FF;

    return 1;
}

uint8_t mp6502::SBC() {
    // Implementation goes here
    return 0;
}

uint8_t mp6502::CMP() {
    // Implementation goes here
    return 0;
}

uint8_t mp6502::CPX() {
    // Implementation goes here
    return 0;
}

uint8_t mp6502::CPY() {
    // Implementation goes here
    return 0;
}

uint8_t mp6502::INC() {
    // Implementation goes here
    return 0;
}

uint8_t mp6502::INX() {
    // Implementation goes here
    return 0;
}

uint8_t mp6502::INY() {
    // Implementation goes here
    return 0;
}

uint8_t mp6502::DEC() {
    // Implementation goes here
    return 0;
}

uint8_t mp6502::DEX() {
    // Implementation goes here
    return 0;
}

uint8_t mp6502::DEY() {
    // Implementation goes here
    return 0;
}

uint8_t mp6502::ASL() {

    // Fetch data
    fetch();

    // Shift left
    temp = data << 1;

    // Set flags
    set_flag(C, (temp & 0xFF00) > 0);
    set_flag(Z, (temp & 0x00FF) == 0x00);
    set_flag(N, temp & 0x80);

    // Write back to memory
    if (lookup[opcode].addr_mode == &mp6502::IMP)
        acc = temp & 0x00FF;
    else
        write(addr_abs, temp & 0x00FF);

    return 0;
}

uint8_t mp6502::LSR() {
    // Implementation goes here
    return 0;
}

uint8_t mp6502::ROL() {
    // Implementation goes here
    return 0;
}

uint8_t mp6502::ROR() {
    // Implementation goes here
    return 0;
}

uint8_t mp6502::JMP() {
    // Implementation goes here
    return 0;
}

uint8_t mp6502::JSR() {
    // Implementation goes here
    return 0;
}

uint8_t mp6502::RTS() {
    // Implementation goes here
    return 0;
}

uint8_t mp6502::BCC() {
    // Implementation goes here
    return 0;
}

uint8_t mp6502::BCS() {
    // Implementation goes here
    return 0;
}

uint8_t mp6502::BEQ() {
    // Implementation goes here
    return 0;
}

uint8_t mp6502::BMI() {
    // Implementation goes here
    return 0;
}

uint8_t mp6502::BNE() {
    // Implementation goes here
    return 0;
}

uint8_t mp6502::BPL() {
    // Implementation goes here
    return 0;
}

uint8_t mp6502::BVC() {
    // Implementation goes here
    return 0;
}

uint8_t mp6502::BVS() {
    // Implementation goes here
    return 0;
}

uint8_t mp6502::CLC() {
    set_flag(C, false);
    return 0;
}

uint8_t mp6502::CLD() {
    set_flag(D, false);
    return 0;
}

uint8_t mp6502::CLI() {
    set_flag(I, false);
    return 0;
}

uint8_t mp6502::CLV() {
    set_flag(V, false);
    return 0;
}

uint8_t mp6502::SEC() {
    set_flag(C, false);
    return 0;
}

uint8_t mp6502::SED() {
    set_flag(D, true);
    return 0;
}

uint8_t mp6502::SEI() {
    set_flag(I, true);
    return 0;
}

uint8_t mp6502::BRK() {
    // Implementation goes here
    return 0;
}

uint8_t mp6502::NOP() {
    // Implementation goes here
    return 0;
}

uint8_t mp6502::RTI() {
    // Implementation goes here
    return 0;
}