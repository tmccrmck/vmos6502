#ifndef VMOS6502_CPU_H
#define VMOS6502_CPU_H

#include <cstdint>
#include <cstdio>
#include <cstdlib>

//#include "nes.h"
#include "instruction.h"

class NES;

class CPU {
public:
	uint64_t cycles;
	uint16_t PC;       // program counter
	uint8_t SP;        // stack pointer
	uint8_t A;         // accumulator
	uint8_t X;         // X register
	uint8_t Y;         // Y register
	uint8_t flags;     // flags register
	uint8_t interrupt; // interrupt type
	int stall;

	CPU() : cycles(0), PC(0), SP(0), A(0), X(0), Y(0), flags(0), interrupt(0), stall(0) {}

	void execute(byte opcode, NES* nes, uint16_t address, const Instruction instruction);

	bool pagesDiffer(uint16_t a, uint16_t b);

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
	void adc(NES* nes, uint16_t address, byte mode);

	// AND - logical AND
	// Nonstandard name to disambiguate from 'and' label
	void and_instruction(NES* nes, uint16_t address, byte mode);

	// ASL - Arithmetic Shift Left
	void asl(NES* nes, uint16_t address, byte mode);

	// BIT - BIt Test
	void bit(NES* nes, uint16_t address, byte mode);

	// CMP - CoMPare
	void cmp(NES* nes, uint16_t address, byte mode);

	// CPX - ComPare X register
	void cpx(NES* nes, uint16_t address, byte mode);

	// CPY - ComPare Y register
	void cpy(NES* nes, uint16_t address, byte mode);

	// DEC - DECrement memory
	void dec(NES* nes, uint16_t address, byte mode);


	// EOR - Exclusive OR
	void eor(NES* nes, uint16_t address, byte mode);

	// INC - INCrement memory
	void inc(NES* nes, uint16_t address, byte mode);

	// JMP - JuMP
	void jmp(NES* nes, uint16_t address, byte mode);

	// LDA - LoaD Accumulator
	void lda(NES* nes, uint16_t address, byte mode);

	// LDX - LoaD X register
	void ldx(NES* nes, uint16_t address, byte mode);

	// LDY - LoaD Y register
	void ldy(NES* nes, uint16_t address, byte mode);

	// LSR - Logical Shift Right
	void lsr(NES* nes, uint16_t address, byte mode);

	// ORA - logical OR with Accumulator
	void ora(NES* nes, uint16_t address, byte mode);

	// PHP - PusH Processor status
	void php(NES* nes, uint16_t address, byte mode);

	// ROL - ROtate Left
	void rol(NES* nes, uint16_t address, byte mode);

	// ROR - ROtate Right
	void ror(NES* nes, uint16_t address, byte mode);

	// SBC - SuBtract with Carry
	void sbc(NES* nes, uint16_t address, byte mode);

	// SEI - SEt Interrupt disable
	void sei(NES* nes, uint16_t address, byte mode);

	// STA - STore Accumulator
	void sta(NES* nes, uint16_t address, byte mode);

	// STX - Store X Register
	void stx(NES* nes, uint16_t address, byte mode);

	// STY - STore Y Register
	void sty(NES* nes, uint16_t address, byte mode);

	// BRK - force interrupt BReaK
	void brk(NES* nes, uint16_t address, byte mode);

	// BPL - Branch if PLus (i.e. if positive)
	void bpl(NES* nes, uint16_t address, byte mode);

	// CLC - CLear Carry flag
	void clc(NES* nes, uint16_t address, byte mode);

	// JSR - Jump to SubRoutine   
	void jsr(NES* nes, uint16_t address, byte mode);
	// PLP - PuLl Processor status
	void plp(NES* nes, uint16_t address, byte mode);
	// BMI - Branch if MInus (i.e. if negative)
	void bmi(NES* nes, uint16_t address, byte mode);

	// SEC - SEt Carry flag
	void sec(NES* nes, uint16_t address, byte mode);

	// RTI - ReTurn from Interrupt
	void rti(NES* nes, uint16_t address, byte mode);

	// BVC - Branch if oVerflow Clear
	void bvc(NES* nes, uint16_t address, byte mode);

	// PHA - PusH Accumulator
	void pha(NES* nes, uint16_t address, byte mode);
	// CLI - CLear Interrupt disable
	void cli(NES* nes, uint16_t address, byte mode);

	// RTS - ReTurn from Subroutine
	void rts(NES* nes, uint16_t address, byte mode);
	// PLA - PuLl Accumulator
	void pla(NES* nes, uint16_t address, byte mode);

	// BVS - Branch if oVerflow Set
	void bvs(NES* nes, uint16_t address, byte mode);

	// DEY - DEcrement Y register
	void dey(NES* nes, uint16_t address, byte mode);

	// TXA - Transfer X to Accumulator
	void txa(NES* nes, uint16_t address, byte mode);

	// BCC - Branch if Carry Clear
	void bcc(NES* nes, uint16_t address, byte mode);

	// TYA - Transfer Y to Accumulator
	void tya(NES* nes, uint16_t address, byte mode);
	

	// BCS - Branch if Carry Set
	void bcs(NES* nes, uint16_t address, byte mode);
	// TAY - Transfer Accumulator to Y
	void tay(NES* nes, uint16_t address, byte mode);
	

	// TXS - Transfer X to Stack pointer
	void txs(NES* nes, uint16_t address, byte mode);
	// TAX - Transfer Accumulator to X
	void tax(NES* nes, uint16_t address, byte mode);
	

	// CLV - CLear oVerflow flag
	void clv(NES* nes, uint16_t address, byte mode);
	// TSX - Transfer Stack pointer to X
	void tsx(NES* nes, uint16_t address, byte mode);
	

	// INY - INcrement Y register
	void iny(NES* nes, uint16_t address, byte mode);
	

	// DEX - DEcrement X register
	void dex(NES* nes, uint16_t address, byte mode);
	

	// BNE - Branch if Not Equal
	void bne(NES* nes, uint16_t address, byte mode);
	// CLD - CLear Decimal mode
	void cld(NES* nes, uint16_t address, byte mode);

	// INX - INcrement X register
	void inx(NES* nes, uint16_t address, byte mode);

	// BEQ - Branch if EQual
	void beq(NES* nes, uint16_t address, byte mode);

	// SED - SEt Decimal flag
	void sed(NES* nes, uint16_t address, byte mode);

	// NOP - No OPeration
	void nop(NES* nes, uint16_t address, byte mode);
};

#endif //VMOS6502_CPU_H
