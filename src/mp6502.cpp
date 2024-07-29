#include "mp6502.h"

// Constructor
mp6502::mp6502() {
    // Credit: olcNES
    using a = mp6502;
    lookup = {
        { "BRK", &a::BRK, &a::IMM, 7 },{ "ORA", &a::ORA, &a::IZX, 6 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 3 },{ "ORA", &a::ORA, &a::ZP0, 3 },{ "ASL", &a::ASL, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "PHP", &a::PHP, &a::IMP, 3 },{ "ORA", &a::ORA, &a::IMM, 2 },{ "ASL", &a::ASL, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::NOP, &a::IMP, 4 },{ "ORA", &a::ORA, &a::ABS, 4 },{ "ASL", &a::ASL, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
        { "BPL", &a::BPL, &a::REL, 2 },{ "ORA", &a::ORA, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "ORA", &a::ORA, &a::ZPX, 4 },{ "ASL", &a::ASL, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "CLC", &a::CLC, &a::IMP, 2 },{ "ORA", &a::ORA, &a::ABY, 4 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "ORA", &a::ORA, &a::ABX, 4 },{ "ASL", &a::ASL, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
        { "JSR", &a::JSR, &a::ABS, 6 },{ "AND", &a::AND, &a::IZX, 6 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "BIT", &a::BIT, &a::ZP0, 3 },{ "AND", &a::AND, &a::ZP0, 3 },{ "ROL", &a::ROL, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "PLP", &a::PLP, &a::IMP, 4 },{ "AND", &a::AND, &a::IMM, 2 },{ "ROL", &a::ROL, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "BIT", &a::BIT, &a::ABS, 4 },{ "AND", &a::AND, &a::ABS, 4 },{ "ROL", &a::ROL, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
        { "BMI", &a::BMI, &a::REL, 2 },{ "AND", &a::AND, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "AND", &a::AND, &a::ZPX, 4 },{ "ROL", &a::ROL, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "SEC", &a::SEC, &a::IMP, 2 },{ "AND", &a::AND, &a::ABY, 4 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "AND", &a::AND, &a::ABX, 4 },{ "ROL", &a::ROL, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
        { "RTI", &a::RTI, &a::IMP, 6 },{ "EOR", &a::EOR, &a::IZX, 6 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 3 },{ "EOR", &a::EOR, &a::ZP0, 3 },{ "LSR", &a::LSR, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "PHA", &a::PHA, &a::IMP, 3 },{ "EOR", &a::EOR, &a::IMM, 2 },{ "LSR", &a::LSR, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "JMP", &a::JMP, &a::ABS, 3 },{ "EOR", &a::EOR, &a::ABS, 4 },{ "LSR", &a::LSR, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
        { "BVC", &a::BVC, &a::REL, 2 },{ "EOR", &a::EOR, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "EOR", &a::EOR, &a::ZPX, 4 },{ "LSR", &a::LSR, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "CLI", &a::CLI, &a::IMP, 2 },{ "EOR", &a::EOR, &a::ABY, 4 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "EOR", &a::EOR, &a::ABX, 4 },{ "LSR", &a::LSR, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
        { "RTS", &a::RTS, &a::IMP, 6 },{ "ADC", &a::ADC, &a::IZX, 6 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 3 },{ "ADC", &a::ADC, &a::ZP0, 3 },{ "ROR", &a::ROR, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "PLA", &a::PLA, &a::IMP, 4 },{ "ADC", &a::ADC, &a::IMM, 2 },{ "ROR", &a::ROR, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "JMP", &a::JMP, &a::IND, 5 },{ "ADC", &a::ADC, &a::ABS, 4 },{ "ROR", &a::ROR, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
        { "BVS", &a::BVS, &a::REL, 2 },{ "ADC", &a::ADC, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "ADC", &a::ADC, &a::ZPX, 4 },{ "ROR", &a::ROR, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "SEI", &a::SEI, &a::IMP, 2 },{ "ADC", &a::ADC, &a::ABY, 4 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "ADC", &a::ADC, &a::ABX, 4 },{ "ROR", &a::ROR, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
        { "???", &a::NOP, &a::IMP, 2 },{ "STA", &a::STA, &a::IZX, 6 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 6 },{ "STY", &a::STY, &a::ZP0, 3 },{ "STA", &a::STA, &a::ZP0, 3 },{ "STX", &a::STX, &a::ZP0, 3 },{ "???", &a::XXX, &a::IMP, 3 },{ "DEY", &a::DEY, &a::IMP, 2 },{ "???", &a::NOP, &a::IMP, 2 },{ "TXA", &a::TXA, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "STY", &a::STY, &a::ABS, 4 },{ "STA", &a::STA, &a::ABS, 4 },{ "STX", &a::STX, &a::ABS, 4 },{ "???", &a::XXX, &a::IMP, 4 },
        { "BCC", &a::BCC, &a::REL, 2 },{ "STA", &a::STA, &a::IZY, 6 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 6 },{ "STY", &a::STY, &a::ZPX, 4 },{ "STA", &a::STA, &a::ZPX, 4 },{ "STX", &a::STX, &a::ZPY, 4 },{ "???", &a::XXX, &a::IMP, 4 },{ "TYA", &a::TYA, &a::IMP, 2 },{ "STA", &a::STA, &a::ABY, 5 },{ "TXS", &a::TXS, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 5 },{ "???", &a::NOP, &a::IMP, 5 },{ "STA", &a::STA, &a::ABX, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "???", &a::XXX, &a::IMP, 5 },
        { "LDY", &a::LDY, &a::IMM, 2 },{ "LDA", &a::LDA, &a::IZX, 6 },{ "LDX", &a::LDX, &a::IMM, 2 },{ "???", &a::XXX, &a::IMP, 6 },{ "LDY", &a::LDY, &a::ZP0, 3 },{ "LDA", &a::LDA, &a::ZP0, 3 },{ "LDX", &a::LDX, &a::ZP0, 3 },{ "???", &a::XXX, &a::IMP, 3 },{ "TAY", &a::TAY, &a::IMP, 2 },{ "LDA", &a::LDA, &a::IMM, 2 },{ "TAX", &a::TAX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "LDY", &a::LDY, &a::ABS, 4 },{ "LDA", &a::LDA, &a::ABS, 4 },{ "LDX", &a::LDX, &a::ABS, 4 },{ "???", &a::XXX, &a::IMP, 4 },
        { "BCS", &a::BCS, &a::REL, 2 },{ "LDA", &a::LDA, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 5 },{ "LDY", &a::LDY, &a::ZPX, 4 },{ "LDA", &a::LDA, &a::ZPX, 4 },{ "LDX", &a::LDX, &a::ZPY, 4 },{ "???", &a::XXX, &a::IMP, 4 },{ "CLV", &a::CLV, &a::IMP, 2 },{ "LDA", &a::LDA, &a::ABY, 4 },{ "TSX", &a::TSX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 4 },{ "LDY", &a::LDY, &a::ABX, 4 },{ "LDA", &a::LDA, &a::ABX, 4 },{ "LDX", &a::LDX, &a::ABY, 4 },{ "???", &a::XXX, &a::IMP, 4 },
        { "CPY", &a::CPY, &a::IMM, 2 },{ "CMP", &a::CMP, &a::IZX, 6 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "CPY", &a::CPY, &a::ZP0, 3 },{ "CMP", &a::CMP, &a::ZP0, 3 },{ "DEC", &a::DEC, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "INY", &a::INY, &a::IMP, 2 },{ "CMP", &a::CMP, &a::IMM, 2 },{ "DEX", &a::DEX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "CPY", &a::CPY, &a::ABS, 4 },{ "CMP", &a::CMP, &a::ABS, 4 },{ "DEC", &a::DEC, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
        { "BNE", &a::BNE, &a::REL, 2 },{ "CMP", &a::CMP, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "CMP", &a::CMP, &a::ZPX, 4 },{ "DEC", &a::DEC, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "CLD", &a::CLD, &a::IMP, 2 },{ "CMP", &a::CMP, &a::ABY, 4 },{ "NOP", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "CMP", &a::CMP, &a::ABX, 4 },{ "DEC", &a::DEC, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
        { "CPX", &a::CPX, &a::IMM, 2 },{ "SBC", &a::SBC, &a::IZX, 6 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "CPX", &a::CPX, &a::ZP0, 3 },{ "SBC", &a::SBC, &a::ZP0, 3 },{ "INC", &a::INC, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "INX", &a::INX, &a::IMP, 2 },{ "SBC", &a::SBC, &a::IMM, 2 },{ "NOP", &a::NOP, &a::IMP, 2 },{ "???", &a::SBC, &a::IMP, 2 },{ "CPX", &a::CPX, &a::ABS, 4 },{ "SBC", &a::SBC, &a::ABS, 4 },{ "INC", &a::INC, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
        { "BEQ", &a::BEQ, &a::REL, 2 },{ "SBC", &a::SBC, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "SBC", &a::SBC, &a::ZPX, 4 },{ "INC", &a::INC, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "SED", &a::SED, &a::IMP, 2 },{ "SBC", &a::SBC, &a::ABY, 4 },{ "NOP", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "SBC", &a::SBC, &a::ABX, 4 },{ "INC", &a::INC, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
    };
}
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

void mp6502::write(uint16_t addr, uint8_t data_write) const {
    bus->write(addr, data_write);
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
    // Transfer accumulator to X register
    reg[X] = acc;

    // Set flags
    set_flag(Z, reg[X] == 0x00);
    set_flag(N, reg[X] & 0x80);
    return 0;
}

uint8_t mp6502::TAY() {
    // Transfer accumulator to Y register
    reg[Y] = acc;

    // Set flags
    set_flag(Z, reg[Y] == 0x00);
    set_flag(N, reg[Y] & 0x80);
    return 0;
}

uint8_t mp6502::TXA() {
    // Transfer X register to accumulator
    acc = reg[X];

    // Set flags
    set_flag(Z, acc == 0x00);
    set_flag(N, acc & 0x80);
    return 0;
}

uint8_t mp6502::TYA() {
    // Transfer Y register to accumulator
    acc = reg[Y];

    // Set flags
    set_flag(Z, acc == 0x00);
    set_flag(N, acc & 0x80);
    return 0;
}

uint8_t mp6502::TSX() {
    // Transfer stack pointer to X register
    reg[X] = sp;

    // Set flags
    set_flag(Z, reg[X] == 0x00);
    set_flag(N, reg[X] & 0x80);
    return 0;
}

uint8_t mp6502::TXS() {
    // Transfer X register to stack pointer
    sp = reg[X];
    return 0;
}

uint8_t mp6502::PHA() {
    // Push accumulator on stack
    write(STACK_OFFSET + sp, acc);
    sp++;
    return 0;
}

uint8_t mp6502::PHP() {
    // Push processor status on stack
    write(STACK_OFFSET + sp, flags);
    sp++;
    return 0;
}

uint8_t mp6502::PLA() {
    // Pop from stack to accumulator
    sp--;
    acc = read(STACK_OFFSET + sp);

    // Set flags
    set_flag(Z, acc == 0x00);
    set_flag(N, acc & 0x80);
    return 0;
}

uint8_t mp6502::PLP() {
    // Pop from stack to processor status
    sp--;
    flags = read(STACK_OFFSET + sp);
    return 0;
}

uint8_t mp6502::AND() {
    // Fetch data
    fetch();

    // Perform AND operation
    acc &= data;

    // Set flags
    set_flag(Z, acc == 0x0);
    set_flag(N, acc & 0x80);
    return 0;
}

uint8_t mp6502::EOR() {
    // Fetch data
    fetch();

    // Perform XOR operation
    acc ^= data;

    // Set flags
    set_flag(Z, acc == 0x0);
    set_flag(N, acc & 0x80);
    return 0;
}

uint8_t mp6502::ORA() {
    // Fetch data
    fetch();

    // Perform OR operation
    acc |= data;

    // Set flags
    set_flag(Z, acc == 0x0);
    set_flag(N, acc & 0x80);
    return 0;
}

uint8_t mp6502::BIT() {
    // Fetch data
    fetch();

    // Perform a bit test
    temp = acc & data;

    // Set flags
    set_flag(Z, (temp & 0x00FF) == 0x00);
    set_flag(V, data & (1 << 6));
    set_flag(N, data & (1 << 7));
    return 0;
}

uint8_t mp6502::ADC() {
    // Fetch data
    fetch();

    // Perform addition
    temp = acc + data + get_flag(C);

    // Set flags
    set_flag(C, temp > 255);
    set_flag(Z, (temp & 0x00FF) == 0);
    set_flag(V, ~(acc ^ data) & (acc ^ temp) & 0x80);
    set_flag(N, temp & 0x80);

    acc = temp & 0x00FF;

    return 1;
}

uint8_t mp6502::SBC() {
    // Fetch data
    fetch();

    // Perform subtraction
    temp = acc - data - (!get_flag(C));

    // Set flags
    set_flag(C, temp < 0x0100);
    set_flag(Z, (temp & 0x00FF) == 0);
    set_flag(V, ~(acc ^ data) & (acc ^ temp) & 0x80);
    set_flag(N, temp & 0x80);

    acc = temp & 0x00FF;
    return 1;
}

uint8_t mp6502::CMP() {
    // Fetch data
    fetch();

    // Set flags based on comparison
    set_flag(C, acc >= data);
    set_flag(Z, acc == data);
    set_flag(N, (acc - data) & 0x80);
    return 1;
}

uint8_t mp6502::CPX() {
    // Fetch data
    fetch();

    // Set flags based on comparison
    set_flag(C, reg[X] >= data);
    set_flag(Z, reg[X] == data);
    set_flag(N, (reg[X] - data) & 0x80);
    return 0;
}

uint8_t mp6502::CPY() {
    // Fetch data
    fetch();

    // Set flags based on comparison
    set_flag(C, reg[Y] >= data);
    set_flag(Z, reg[Y] == data);
    set_flag(N, (reg[Y] - data) & 0x80);
    return 0;
}

uint8_t mp6502::INC() {
    // Fetch data
    fetch();

    // Increment and write data
    data++;
    write(addr_abs, data);

    // Set flags
    set_flag(Z, data == 0x00);
    set_flag(N, data & 0x80);
    return 0;
}

uint8_t mp6502::INX() {
    // Increment X register
    reg[X]++;

    // Set flags
    set_flag(Z, reg[X] == 0x00);
    set_flag(N, reg[X] & 0x80);
    return 0;
}

uint8_t mp6502::INY() {
    // Increment Y register
    reg[Y]++;

    // Set flags
    set_flag(Z, reg[Y] == 0x00);
    set_flag(N, reg[Y] & 0x80);
    return 0;
}

uint8_t mp6502::DEC() {
    // Fetch data
    fetch();

    // Decrement and write data
    data--;
    write(addr_abs, data);

    // Set flags
    set_flag(Z, data == 0x00);
    set_flag(N, data & 0x80);
    return 0;
}

uint8_t mp6502::DEX() {
    // Decrement X register
    reg[X]--;

    // Set flags
    set_flag(Z, reg[X] == 0x00);
    set_flag(N, reg[X] & 0x80);
    return 0;
}

uint8_t mp6502::DEY() {
    // Decrement Y register
    reg[Y]--;

    // Set flags
    set_flag(Z, reg[Y] == 0x00);
    set_flag(N, reg[Y] & 0x80);
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
    // Fetch data
    fetch();

    // Set flags and shift right
    set_flag(C, data & 0x01);
    temp = data >> 1;
    set_flag(Z, (temp & 0x00FF) == 0x00);
    set_flag(N, temp & 0x80);

    if (lookup[opcode].addr_mode == &mp6502::IMP)
        acc = temp & 0x00FF;
    else
        write(addr_abs, temp & 0x00FF);
    return 0;
}

uint8_t mp6502::ROL() {
    // Fetch data
    fetch();

    // Shift left
    temp = (data << 1) | get_flag(C);

    // Set flags
    set_flag(C, data & 0x80);
    set_flag(Z, (temp & 0x00FF) == 0x00);
    set_flag(N, temp & 0x80);

    // Write back to memory
    if (lookup[opcode].addr_mode == &mp6502::IMP)
        acc = temp & 0x00FF;
    else
        write(addr_abs, temp & 0x00FF);
    return 0;
}

uint8_t mp6502::ROR() {
    // Fetch data
    fetch();

    // Shift right
    temp = (data >> 1) | (get_flag(C) << 7);

    // Set flags
    set_flag(C, data & 0x01);
    set_flag(Z, (temp & 0x00FF) == 0x00);
    set_flag(N, temp & 0x80);

    // Write back to memory
    if (lookup[opcode].addr_mode == &mp6502::IMP)
        acc = temp & 0x00FF;
    else
        write(addr_abs, temp & 0x00FF);
    return 0;
}

uint8_t mp6502::JMP() {
    // Set program counter
    pc = addr_abs;
    return 0;
}

uint8_t mp6502::JSR() {
    // Decrement pc
    pc--;

    // Push pc to stack
    write(STACK_OFFSET + sp, (pc >> 8) & 0x00FF);
    sp++;
    write(STACK_OFFSET + sp, pc & 0x00FF);
    sp++;

    // Set program counter
    pc = addr_abs;
    return 0;
}

uint8_t mp6502::RTS() {
    // Pull pc from stack
    sp--;
    pc = read(STACK_OFFSET + sp);
    sp--;
    pc |= read(STACK_OFFSET + sp) << 8;

    // Increment pc
    pc++;
    return 0;
}

uint8_t mp6502::BCC() {
    if (!get_flag(C)) {
        cycles++;
        addr_abs = pc + addr_rel;

        if ((addr_abs & 0xFF00) != (pc & 0xFF00))
            cycles++;

        pc = addr_abs;
    }
    return 0;
}

uint8_t mp6502::BCS() {
    if (get_flag(C)) {
        cycles++;
        addr_abs = pc + addr_rel;

        if ((addr_abs & 0xFF00) != (pc & 0xFF00))
            cycles++;

        pc = addr_abs;
    }
    return 0;
}

uint8_t mp6502::BEQ() {
    if (get_flag(Z)) {
        cycles++;
        addr_abs = pc + addr_rel;

        if ((addr_abs & 0xFF00) != (pc & 0xFF00))
            cycles++;

        pc = addr_abs;
    }
    return 0;
}

uint8_t mp6502::BMI() {
    if (get_flag(N)) {
        cycles++;
        addr_abs = pc + addr_rel;

        if ((addr_abs & 0xFF00) != (pc & 0xFF00))
            cycles++;

        pc = addr_abs;
    }
    return 0;
}

uint8_t mp6502::BNE() {
    if (!get_flag(Z)) {
        cycles++;
        addr_abs = pc + addr_rel;

        if ((addr_abs & 0xFF00) != (pc & 0xFF00))
            cycles++;

        pc = addr_abs;
    }
    return 0;
}

uint8_t mp6502::BPL() {
    if (!get_flag(N)) {
        cycles++;
        addr_abs = pc + addr_rel;

        if ((addr_abs & 0xFF00) != (pc & 0xFF00))
            cycles++;

        pc = addr_abs;
    }
    return 0;
}

uint8_t mp6502::BVC() {
    if (!get_flag(V)) {
        cycles++;
        addr_abs = pc + addr_rel;

        if ((addr_abs & 0xFF00) != (pc & 0xFF00))
            cycles++;

        pc = addr_abs;
    }
    return 0;
}

uint8_t mp6502::BVS() {
    if (get_flag(V)) {
        cycles++;
        addr_abs = pc + addr_rel;

        if ((addr_abs & 0xFF00) != (pc & 0xFF00))
            cycles++;

        pc = addr_abs;
    }
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
    pc++;

    set_flag(I, true);
    write(STACK_OFFSET + sp, (pc >> 8) & 0x00FF);
    sp++;
    write(STACK_OFFSET + sp, pc & 0x00FF);
    sp++;

    set_flag(B, true);
    write(STACK_OFFSET + sp, flags);
    sp++;
    set_flag(B, false);

    pc = (uint16_t)read(BRK_OFFSET) | ((uint16_t)read(BRK_OFFSET + 1) << 8);
    return 0;
}

uint8_t mp6502::NOP() const {
    switch (opcode) {
        case 0x1C:
        case 0x3C:
        case 0x5C:
        case 0x7C:
        case 0xDC:
        case 0xFC:
            return 1;
            break;
    }
    return 0;
    return 0;
}

uint8_t mp6502::RTI() {
    // Pull from stack
    sp--;
    flags = read(STACK_OFFSET + sp);
    sp--;
    pc = read(STACK_OFFSET + sp);
    return 0;
}