#ifndef INC_6502_H
#define INC_6502_H

#include <cstdint>
#include <string>
#include <vector>
#include "Bus.h"

/*
 * Data bus: 16bit
 * Where data is read and written to and from.
 *
 * Program counter: 16bit
 * points to the next instruction in memory.
 *
 * Stack pointer: 8bit
 * Stack pointer holds the low 8 bits of the next FREE location
 * on the stack. The stack is located between 0x0100 and 0x01FF (256 bytes)
 * Pushing to stack causes stack pointer to be decremented. Opposite for
 * popping off stack. CPU does not detect stack overflow (excessive pushing
 * and popping) and will crash program.
 *
 * Accumulator: 8bit
 * Used for all arithmetic and logical operations.
 * Can be stored and retrieved from memory or stack.
 *
 * Index Registers X and Y: 8bit
 * Commonly used to hold counters or offsets for accessing memory.
 * Special function for X register: can copy stack pointer or
 * change its value.
 *
 * Status registers 8bit (only 0 or 1)
 * Carry flag:
 * Is set if the last operation resulted in an overflow or an underflow.
 *
 * Zero flag:
 * Is set if the last operations result was zero.
 *
 * Interrupt disable:
 * While set, the processor will ignore device interrupts.
 *
 * Decimal mode:
 * While set, processor obeys rules of binary coded decimal (BCD)
 * arithmetic during addition and subtraction.
 *
 * Break command:
 * Set when a BRK instruction has been executed and an interrupt
 * has been generated to process it.
 *
 * Overflow flag:
 * Set during arithmetic operations if the result yields an invalid
 * 2's complement result. (adding +ve numbers and ending up with a
 * negative result). Determined by looking at the carry between bits 6
 * & 7 and between bit 7 and the carry flag.
 *
 * Negative flag:
 * Set if the result of the last operation had bit 7 set to 1.
 */

// Zero-page takes (0x0000-0x00FF)
#define ZERO_PAGE_OFFSET 0x0000

// Stack takes (0x0100-0x01FF)
#define STACK_OFFSET 0x0100

// Reserved locations (0xFFFA-0xFFFF)
// * Non-maskable interrupt handler (0xFFFA/B)
// * Power on reset location (0xFFFC/D)
// * BRK/Interrupt request handler (0xFFFE/F)
#define INTERRUPT_OFFSET 0xFFFA
#define PWR_RESET_OFFSET 0xFFFC
#define BRK_OFFSET 0xFFFE



//=========================================================//

class mp6502 {

    // enum for accessing index registers
    typedef enum {
        X,
        Y,
        INDEX_SIZE
    } INDEX;

    // enum for accessing processor status registers
    typedef enum {
        C = (1 << 0), // Carry flag
        Z = (1 << 1), // Zero flag
        I = (1 << 2), // Interrupt disable
        D = (1 << 3), // Decimal mode
        B = (1 << 4), // Break command
        U = (1 << 5), // Unused
        V = (1 << 6), // Overflow flag
        N = (1 << 7) // Negative flag
    } FLAGS;

public:
    mp6502();
    ~mp6502();

    Bus* bus{}; // Data bus
    uint16_t pc = 0x0000; // Program counter
    uint8_t sp = 0x00; // Stack pointer
    uint8_t acc = 0x00; // Accumulator
    uint8_t reg[INDEX_SIZE] = {0x00}; // Index registers X and Y
    uint8_t flags = {0x00}; // Processor status flags

private:

    struct INSTRUCTION {
        std::string name;
        uint8_t (mp6502::*opcode)() = nullptr;
        uint8_t (mp6502::*addr_mode)() = nullptr;
        uint8_t cycles = 0;
    };
    std::vector<INSTRUCTION> lookup; // Function lookup table

    //===============Helper functions===============//
    uint8_t get_flag(FLAGS flag);
    void set_flag(FLAGS flag, bool value);
    uint8_t fetch(); // Fetches data from memory OR immediate value

    //=========Bus communication functions==========//
    void connect_bus(Bus *b);
    uint8_t read(uint16_t addr);
    void write(uint16_t addr, uint8_t data) const;

    //==============Emulation variables=============//
    uint8_t data = 0x00; // Represents data that is read or written
    uint16_t temp = 0x0000; // A convenience variable used everywhere
    uint16_t addr_abs = 0x0000; // All used memory addresses end up in here
    uint16_t addr_rel = 0x00; // Represents absolute address following a branch
    uint8_t opcode = 0x00; // Is the instruction byte
    uint8_t cycles = 0;	// Counts how many cycles the instruction has remaining
    uint32_t clock_count = 0; // A global accumulation of the number of clocks

    //++++++++++++++++++++++++++++ADDRESSING MODES+++++++++++++++++++++++++++++++++++++//
    uint8_t IMP(); // Implicit
    uint8_t IMM(); // Immediate
    uint8_t ZP0(); // Zero Page
    uint8_t ZPX(); // Zero Page with X offset
    uint8_t ZPY(); // Zero Page with Y offset
    uint8_t REL(); // Relative
    uint8_t ABS(); // Absolute
    uint8_t ABX(); // Absolute with X offset
    uint8_t ABY(); // Absolute with Y offset
    uint8_t IND(); // Indirect
    uint8_t IZX(); // Indexed indirect X
    uint8_t IZY(); // Indexed indirect Y


    //+++++++++++++++++++++++++++++++++OPCODES++++++++++++++++++++++++++++++++++++++++//

    //============Load/store operations=============//
    uint8_t LDA(); // Load accumulator
    uint8_t LDX(); // Load X register
    uint8_t LDY(); // Load Y register
    uint8_t STA(); // Store accumulator
    uint8_t STX(); // Store X register
    uint8_t STY(); // Store Y register

    //==============Register transfers==============//
    uint8_t TAX(); // Transfer accumulator to X register
    uint8_t TAY(); // Transfer accumulator to Y register
    uint8_t TXA(); // Transfer X register to accumulator
    uint8_t TYA(); // Transfer Y register to accumulator

    //===============Stack Operations===============//
    uint8_t TSX(); // Transfer stack pointer to X register
    uint8_t TXS(); // Transfer X register to stack pointer
    uint8_t PHA(); // Push accumulator on stack
    uint8_t PHP(); // Push processor status on stack
    uint8_t PLA(); // Pull accumulator from stack
    uint8_t PLP(); // Pull processor status from stack

    //==============Logical operations==============//
    uint8_t AND(); // Logical AND
    uint8_t EOR(); // Exclusive OR
    uint8_t ORA(); // Logical OR
    uint8_t BIT(); // Bit test

    //=============Arithmetic operations============//
    uint8_t ADC(); // Add with carry
    uint8_t SBC(); // Subtract with carry
    uint8_t CMP(); // Compare with accumulator
    uint8_t CPX(); // Compare with X register
    uint8_t CPY(); // Compare with Y register

    //========Increment/decrement operations========//
    uint8_t INC(); // Increment a memory location
    uint8_t INX(); // Increment X register
    uint8_t INY(); // Increment Y register
    uint8_t DEC(); // Decrement a memory location
    uint8_t DEX(); // Decrement X register
    uint8_t DEY(); // Decrement Y register

    //===============Shift operations===============//
    uint8_t ASL(); // Arithmetic shift left
    uint8_t LSR(); // Logical shift right
    uint8_t ROL(); // Rotate left
    uint8_t ROR(); // Rotate right

    //===============Jump operations================//
    uint8_t JMP(); // Jump
    uint8_t JSR(); // Jump to subroutine
    uint8_t RTS(); // Return from subroutine

    //==============Branch operations===============//
    uint8_t BCC(); // Branch if carry flag clear
    uint8_t BCS(); // Branch if carry flag set
    uint8_t BEQ(); // Branch if zero flag set
    uint8_t BMI(); // Branch if negative flag set
    uint8_t BNE(); // Branch if zero flag clear
    uint8_t BPL(); // Branch if negative flag clear
    uint8_t BVC(); // Branch if overflow flag clear
    uint8_t BVS(); // Branch if overflow flag set

    //============Status flag operations============//
    uint8_t CLC(); // Clear carry flag
    uint8_t CLD(); // Clear decimal mode flag
    uint8_t CLI(); // Clear interrupt disable flag
    uint8_t CLV(); // Clear overflow flag
    uint8_t SEC(); // Set carry flag
    uint8_t SED(); // Set decimal mode flag
    uint8_t SEI(); // Set interrupt disable flag

    //==============System operations===============//
    uint8_t BRK(); // Force an interrupt
    uint8_t NOP() const; // No operation
    uint8_t RTI(); // Return from interrupt

    //=================Non-function=================//
    uint8_t XXX(); // Illegal opcode

};

#endif //INC_6502_H
