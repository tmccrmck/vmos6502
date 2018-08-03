#ifndef VMOS6502_CPU_H
#define VMOS6502_CPU_H

#include <cstdint>
#include <cstdio>
#include <cstdlib>

#include "instruction.h"
#include "nes.h"

class NES;

enum Interrupts {
    interruptNone = 1,
    interruptNMI = 2,
    interruptIRQ = 3
};

enum AddressingModes {
    modeAbsolute = 1,
    modeAbsoluteX = 2,
    modeAbsoluteY = 3,
    modeAccumulator = 4,
    modeImmediate = 5,
    modeImplied = 6,
    modeIndexedIndirect = 7,
    modeIndirect = 8,
    modeIndirectIndexed = 9,
    modeRelative = 10,
    modeZeroPage = 11,
    modeZeroPageX = 12,
    modeZeroPageY = 13
};

class CPU {
public:
    uint64_t cycles;
    uint16_t pc;
    uint8_t sp;
    uint8_t a;
    uint8_t x;
    uint8_t y;
    uint8_t flags;
    uint8_t interrupt;
    int stall;
    NES *nes;

    explicit CPU(NES *nes_, uint16_t _pc) : cycles(0), pc(_pc), sp(0xFD), a(0), x(0), y(0), flags(0x24), interrupt(0), stall(0), nes(nes_) {}

    void executeOpcode(const byte opcode, const uint16_t address, const Instruction& instruction);

    bool pagesDiffer(uint16_t a, uint16_t b);

    byte readb(uint16_t addr);

    void writeb(uint16_t addr, byte value);

    // set zero flag if 'value' is zero
    void setZ(byte value);

    // get zero flag
    byte getZ();

    // set negative flag if 'value' is negative
    void setN(byte value);

    // get negative flag
    byte getN();

    // set zero and negative flags according to 'value'
    void setZN(byte value);

    // set carry flag if 'value' is true
    void setC(bool value);

    // get carry flag
    byte getC();

    // set interrupt disable flag if 'value' is true
    void setI(bool value);

    // get interrupt disable flag
    byte getI();

    // set decimal flag if 'value' is true
    void setD(bool value);

    // get decimal flag
    byte getD();

    // set software interrupt flag if 'value' is true
    void setB(bool value);

    // get software interrupt flag
    byte getB();

    // set overflow flag if 'value' is true
    void setV(bool value);

    // get overflow flag
    byte getV();

    // perform compare operation on a and b, setting
    // flags Z, N, C accordingly
    void compare(byte a, byte b);

    // 1 cycle for taking a branch
    // 1 cycle if the branch is to a new page
    void branchDelay(uint16_t address, uint16_t pc);

    // ADC - ADd with Carry
    void adc(uint16_t address);

    // AND - logical AND
    // Nonstandard name to disambiguate from 'and' label
    void and_instruction(uint16_t address);

    // ASL - Arithmetic Shift Left
    void asl(uint16_t address, byte mode);

    // BIT - BIt Test
    void bit(uint16_t address);

    // CMP - CoMPare
    void cmp(uint16_t address);

    // CPX - ComPare X register
    void cpx(uint16_t address);

    // CPY - ComPare Y register
    void cpy(uint16_t address);

    // DEC - DECrement memory
    void dec(uint16_t address);

    // EOR - Exclusive OR
    void eor(uint16_t address);

    // INC - INCrement memory
    void inc(uint16_t address);

    // JMP - JuMP
    void jmp(uint16_t address);

    // LDA - LoaD Accumulator
    void lda(uint16_t address);

    // LDX - LoaD X register
    void ldx(uint16_t address);

    // LDY - LoaD Y register
    void ldy(uint16_t address);

    // LSR - Logical Shift Right
    void lsr(uint16_t address, byte mode);

    // ORA - logical OR with Accumulator
    void ora(uint16_t address);

    // PHP - PusH Processor status
    void php();

    // ROL - ROtate Left
    void rol(uint16_t address, byte mode);

    // ROR - ROtate Right
    void ror(uint16_t address, byte mode);

    // SBC - SuBtract with Carry
    void sbc(uint16_t address);

    // SEI - SEt Interrupt disable
    void sei();

    // STA - STore Accumulator
    void sta(uint16_t address);

    // STX - Store X Register
    void stx(uint16_t address);

    // STY - STore Y Register
    void sty(uint16_t address);

    // BRK - force interrupt BReaK
    void brk();

    // BPL - Branch if PLus (i.e. if positive)
    void bpl(uint16_t address);

    // CLC - CLear Carry flag
    void clc();

    // JSR - Jump to SubRoutine
    void jsr(uint16_t address);

    // PLP - PuLl Processor status
    void plp();

    // BMI - Branch if MInus (i.e. if negative)
    void bmi(uint16_t address);

    // SEC - SEt Carry flag
    void sec();

    // RTI - ReTurn from Interrupt
    void rti();

    // BVC - Branch if oVerflow Clear
    void bvc(uint16_t address);

    // PHA - PusH Accumulator
    void pha();

    // CLI - CLear Interrupt disable
    void cli();

    // RTS - ReTurn from Subroutine
    void rts();

    // PLA - PuLl Accumulator
    void pla();

    // BVS - Branch if oVerflow Set
    void bvs(uint16_t address);

    // DEY - DEcrement Y register
    void dey();

    // TXA - Transfer X to Accumulator
    void txa();

    // BCC - Branch if Carry Clear
    void bcc(uint16_t address);

    // TYA - Transfer Y to Accumulator
    void tya();


    // BCS - Branch if Carry Set
    void bcs(uint16_t address);

    // TAY - Transfer Accumulator to Y
    void tay();


    // TXS - Transfer X to Stack pointer
    void txs();

    // TAX - Transfer Accumulator to X
    void tax();


    // CLV - CLear oVerflow flag
    void clv();

    // TSX - Transfer Stack pointer to X
    void tsx();


    // INY - INcrement Y register
    void iny();


    // DEX - DEcrement X register
    void dex();


    // BNE - Branch if Not Equal
    void bne(uint16_t address);

    // CLD - CLear Decimal mode
    void cld();

    // INX - INcrement X register
    void inx();

    // BEQ - Branch if EQual
    void beq(uint16_t address);

    // SED - SEt Decimal flag
    void sed();

    // NOP - No OPeration
    void nop();

    // note use getI
    void triggerIRQ();

};

#endif //VMOS6502_CPU_H
