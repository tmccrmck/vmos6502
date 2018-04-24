#include "nes.h"

// do addresses represent different pages?
bool pagesDiffer(uint16_t a, uint16_t b) {
	return (a & 0xFF00) != (b & 0xFF00);
}

constexpr Instruction instructions[256] = {
	{ 0  , "BRK", &CPU::brk, 6, 1, 7, 0 },
	{ 1  , "ORA", ora, 7, 2, 6, 0 },
	{ 2  , "KIL", nop, 6, 0, 2, 0 },
	{ 3  , "SLO", nop, 7, 0, 8, 0 },
	{ 4  , "NOP", nop, 11, 2, 3, 0 },
	{ 5  , "ORA", ora, 11, 2, 3, 0 },
	{ 6  , "ASL", asl, 11, 2, 5, 0 },
	{ 7  , "SLO", nop, 11, 0, 5, 0 },
	{ 8  , "PHP", php, 6, 1, 3, 0 },
	{ 9  , "ORA", ora, 5, 2, 2, 0 },
	{ 10 , "ASL", asl, 4, 1, 2, 0 },
	{ 11 , "ANC", nop, 5, 0, 2, 0 },
	{ 12 , "NOP", nop, 1, 3, 4, 0 },
	{ 13 , "ORA", ora, 1, 3, 4, 0 },
	{ 14 , "ASL", asl, 1, 3, 6, 0 },
	{ 15 , "SLO", nop, 1, 0, 6, 0 },
	{ 16 , "BPL", bpl, 10, 2, 2, 1 },
	{ 17 , "ORA", ora, 9, 2, 5, 1 },
	{ 18 , "KIL", nop, 6, 0, 2, 0 },
	{ 19 , "SLO", nop, 9, 0, 8, 0 },
	{ 20 , "NOP", nop, 12, 2, 4, 0 },
	{ 21 , "ORA", ora, 12, 2, 4, 0 },
	{ 22 , "ASL", asl, 12, 2, 6, 0 },
	{ 23 , "SLO", nop, 12, 0, 6, 0 },
	{ 24 , "CLC", clc, 6, 1, 2, 0 },
	{ 25 , "ORA", ora, 3, 3, 4, 1 },
	{ 26 , "NOP", nop, 6, 1, 2, 0 },
	{ 27 , "SLO", nop, 3, 0, 7, 0 },
	{ 28 , "NOP", nop, 2, 3, 4, 1 },
	{ 29 , "ORA", ora, 2, 3, 4, 1 },
	{ 30 , "ASL", asl, 2, 3, 7, 0 },
	{ 31 , "SLO", nop, 2, 0, 7, 0 },
	{ 32 , "JSR", jsr, 1, 3, 6, 0 },
	{ 33 , "AND", and_instruction, 7, 2, 6, 0 },
	{ 34 , "KIL", nop, 6, 0, 2, 0 },
	{ 35 , "RLA", nop, 7, 0, 8, 0 },
	{ 36 , "BIT", bit, 11, 2, 3, 0 },
	{ 37 , "AND", and_instruction, 11, 2, 3, 0 },
	{ 38 , "ROL", rol, 11, 2, 5, 0 },
	{ 39 , "RLA", nop, 11, 0, 5, 0 },
	{ 40 , "PLP", plp, 6, 1, 4, 0 },
	{ 41 , "AND", and_instruction, 5, 2, 2, 0 },
	{ 42 , "ROL", rol, 4, 1, 2, 0 },
	{ 43 , "ANC", nop, 5, 0, 2, 0 },
	{ 44 , "BIT", bit, 1, 3, 4, 0 },
	{ 45 , "AND", and_instruction, 1, 3, 4, 0 },
	{ 46 , "ROL", rol, 1, 3, 6, 0 },
	{ 47 , "RLA", nop, 1, 0, 6, 0 },
	{ 48 , "BMI", bmi, 10, 2, 2, 1 },
	{ 49 , "AND", and_instruction, 9, 2, 5, 1 },
	{ 50 , "KIL", and_instruction, 6, 0, 2, 0 },
	{ 51 , "RLA", nop, 9, 0, 8, 0 },
	{ 52 , "NOP", nop, 12, 2, 4, 0 },
	{ 53 , "AND", nop, 12, 2, 4, 0 },
	{ 54 , "ROL", rol, 12, 2, 6, 0 },
	{ 55 , "RLA", nop, 12, 0, 6, 0 },
	{ 56 , "SEC", sec, 6, 1, 2, 0 },
	{ 57 , "AND", and_instruction, 3, 3, 4, 1 },
	{ 58 , "NOP", nop, 6, 1, 2, 0 },
	{ 59 , "RLA", nop, 3, 0, 7, 0 },
	{ 60 , "NOP", nop, 2, 3, 4, 1 },
	{ 61 , "AND", and_instruction, 2, 3, 4, 1 },
	{ 62 , "ROL", rol, 2, 3, 7, 0 },
	{ 63 , "RLA", nop, 2, 0, 7, 0 },
	{ 64 , "RTI", rti, 6, 1, 6, 0 },
	{ 65 , "EOR", eor, 7, 2, 6, 0 },
	{ 66 , "KIL", nop, 6, 0, 2, 0 },
	{ 67 , "SRE", nop, 7, 0, 8, 0 },
	{ 68 , "NOP", nop, 11, 2, 3, 0 },
	{ 69 , "EOR", eor, 11, 2, 3, 0 },
	{ 70 , "LSR", lsr, 11, 2, 5, 0 },
	{ 71 , "SRE", nop, 11, 0, 5, 0 },
	{ 72 , "PHA", pha, 6, 1, 3, 0 },
	{ 73 , "EOR", eor, 5, 2, 2, 0 },
	{ 74 , "LSR", lsr, 4, 1, 2, 0 },
	{ 75 , "ALR", nop, 5, 0, 2, 0 },
	{ 76 , "JMP", jmp, 1, 3, 3, 0 },
	{ 77 , "EOR", eor, 1, 3, 4, 0 },
	{ 78 , "LSR", lsr, 1, 3, 6, 0 },
	{ 79 , "SRE", nop, 1, 0, 6, 0 },
	{ 80 , "BVC", bvc, 10, 2, 2, 1 },
	{ 81 , "EOR", eor, 9, 2, 5, 1 },
	{ 82 , "KIL", nop, 6, 0, 2, 0 },
	{ 83 , "SRE", nop, 9, 0, 8, 0 },
	{ 84 , "NOP", nop, 12, 2, 4, 0 },
	{ 85 , "EOR", eor, 12, 2, 4, 0 },
	{ 86 , "LSR", lsr, 12, 2, 6, 0 },
	{ 87 , "SRE", nop, 12, 0, 6, 0 },
	{ 88 , "CLI", cli, 6, 1, 2, 0 },
	{ 89 , "EOR", eor, 3, 3, 4, 1 },
	{ 90 , "NOP", nop, 6, 1, 2, 0 },
	{ 91 , "SRE", nop, 3, 0, 7, 0 },
	{ 92 , "NOP", nop, 2, 3, 4, 1 },
	{ 93 , "EOR", eor, 2, 3, 4, 1 },
	{ 94 , "LSR", lsr, 2, 3, 7, 0 },
	{ 95 , "SRE", nop, 2, 0, 7, 0 },
	{ 96 , "RTS", rts, 6, 1, 6, 0 },
	{ 97 , "ADC", adc, 7, 2, 6, 0 },
	{ 98 , "KIL", nop, 6, 0, 2, 0 },
	{ 99 , "RRA", nop, 7, 0, 8, 0 },
	{ 100, "NOP", nop, 11, 2, 3, 0 },
	{ 101, "ADC", adc, 11, 2, 3, 0 },
	{ 102, "ROR", ror, 11, 2, 5, 0 },
	{ 103, "RRA", nop, 11, 0, 5, 0 },
	{ 104, "PLA", pla, 6, 1, 4, 0 },
	{ 105, "ADC", adc, 5, 2, 2, 0 },
	{ 106, "ROR", ror, 4, 1, 2, 0 },
	{ 107, "ARR", nop, 5, 0, 2, 0 },
	{ 108, "JMP", jmp, 8, 3, 5, 0 },
	{ 109, "ADC", adc, 1, 3, 4, 0 },
	{ 110, "ROR", ror, 1, 3, 6, 0 },
	{ 111, "RRA", nop, 1, 0, 6, 0 },
	{ 112, "BVS", bvs, 10, 2, 2, 1 },
	{ 113, "ADC", adc, 9, 2, 5, 1 },
	{ 114, "KIL", nop, 6, 0, 2, 0 },
	{ 115, "RRA", nop, 9, 0, 8, 0 },
	{ 116, "NOP", nop, 12, 2, 4, 0 },
	{ 117, "ADC", adc, 12, 2, 4, 0 },
	{ 118, "ROR", ror, 12, 2, 6, 0 },
	{ 119, "RRA", nop, 12, 0, 6, 0 },
	{ 120, "SEI", sei, 6, 1, 2, 0 },
	{ 121, "ADC", adc, 3, 3, 4, 1 },
	{ 122, "NOP", nop, 6, 1, 2, 0 },
	{ 123, "RRA", nop, 3, 0, 7, 0 },
	{ 124, "NOP", nop, 2, 3, 4, 1 },
	{ 125, "ADC", adc, 2, 3, 4, 1 },
	{ 126, "ROR", ror, 2, 3, 7, 0 },
	{ 127, "RRA", nop, 2, 0, 7, 0 },
	{ 128, "NOP", nop, 5, 2, 2, 0 },
	{ 129, "STA", sta, 7, 2, 6, 0 },
	{ 130, "NOP", nop, 5, 0, 2, 0 },
	{ 131, "SAX", nop, 7, 0, 6, 0 },
	{ 132, "STY", sty, 11, 2, 3, 0 },
	{ 133, "STA", sta, 11, 2, 3, 0 },
	{ 134, "STX", stx, 11, 2, 3, 0 },
	{ 135, "SAX", nop, 11, 0, 3, 0 },
	{ 136, "DEY", dey, 6, 1, 2, 0 },
	{ 137, "NOP", nop, 5, 0, 2, 0 },
	{ 138, "TXA", txa, 6, 1, 2, 0 },
	{ 139, "XAA", nop, 5, 0, 2, 0 },
	{ 140, "STY", sty, 1, 3, 4, 0 },
	{ 141, "STA", sta, 1, 3, 4, 0 },
	{ 142, "STX", stx, 1, 3, 4, 0 },
	{ 143, "SAX", nop, 1, 0, 4, 0 },
	{ 144, "BCC", bcc, 10, 2, 2, 1 },
	{ 145, "STA", sta, 9, 2, 6, 0 },
	{ 146, "KIL", nop, 6, 0, 2, 0 },
	{ 147, "AHX", nop, 9, 0, 6, 0 },
	{ 148, "STY", sty, 12, 2, 4, 0 },
	{ 149, "STA", sta, 12, 2, 4, 0 },
	{ 150, "STX", stx, 13, 2, 4, 0 },
	{ 151, "SAX", nop, 13, 0, 4, 0 },
	{ 152, "TYA", tya, 6, 1, 2, 0 },
	{ 153, "STA", sta, 3, 3, 5, 0 },
	{ 154, "TXS", txs, 6, 1, 2, 0 },
	{ 155, "TAS", nop, 3, 0, 5, 0 },
	{ 156, "SHY", nop, 2, 0, 5, 0 },
	{ 157, "STA", sta, 2, 3, 5, 0 },
	{ 158, "SHX", nop, 3, 0, 5, 0 },
	{ 159, "AHX", nop, 3, 0, 5, 0 },
	{ 160, "LDY", ldy, 5, 2, 2, 0 },
	{ 161, "LDA", lda, 7, 2, 6, 0 },
	{ 162, "LDX", ldx, 5, 2, 2, 0 },
	{ 163, "LAX", nop, 7, 0, 6, 0 },
	{ 164, "LDY", ldy, 11, 2, 3, 0 },
	{ 165, "LDA", lda, 11, 2, 3, 0 },
	{ 166, "LDX", ldx, 11, 2, 3, 0 },
	{ 167, "LAX", nop, 11, 0, 3, 0 },
	{ 168, "TAY", tay, 6, 1, 2, 0 },
	{ 169, "LDA", lda, 5, 2, 2, 0 },
	{ 170, "TAX", tax, 6, 1, 2, 0 },
	{ 171, "LAX", nop, 5, 0, 2, 0 },
	{ 172, "LDY", ldy, 1, 3, 4, 0 },
	{ 173, "LDA", lda, 1, 3, 4, 0 },
	{ 174, "LDX", ldx, 1, 3, 4, 0 },
	{ 175, "LAX", nop, 1, 0, 4, 0 },
	{ 176, "BCS", bcs, 10, 2, 2, 1 },
	{ 177, "LDA", lda, 9, 2, 5, 1 },
	{ 178, "KIL", nop, 6, 0, 2, 0 },
	{ 179, "LAX", nop, 9, 0, 5, 1 },
	{ 180, "LDY", ldy, 12, 2, 4, 0 },
	{ 181, "LDA", lda, 12, 2, 4, 0 },
	{ 182, "LDX", ldx, 13, 2, 4, 0 },
	{ 183, "LAX", nop, 13, 0, 4, 0 },
	{ 184, "CLV", clv, 6, 1, 2, 0 },
	{ 185, "LDA", lda, 3, 3, 4, 1 },
	{ 186, "TSX", tsx, 6, 1, 2, 0 },
	{ 187, "LAS", nop, 3, 0, 4, 1 },
	{ 188, "LDY", ldy, 2, 3, 4, 1 },
	{ 189, "LDA", lda, 2, 3, 4, 1 },
	{ 190, "LDX", ldx, 3, 3, 4, 1 },
	{ 191, "LAX", nop, 3, 0, 4, 1 },
	{ 192, "CPY", cpy, 5, 2, 2, 0 },
	{ 193, "CMP", cmp, 7, 2, 6, 0 },
	{ 194, "NOP", nop, 5, 0, 2, 0 },
	{ 195, "DCP", nop, 7, 0, 8, 0 },
	{ 196, "CPY", cpy, 11, 2, 3, 0 },
	{ 197, "CMP", cmp, 11, 2, 3, 0 },
	{ 198, "DEC", dec, 11, 2, 5, 0 },
	{ 199, "DCP", nop, 11, 0, 5, 0 },
	{ 200, "INY", iny, 6, 1, 2, 0 },
	{ 201, "CMP", cmp, 5, 2, 2, 0 },
	{ 202, "DEX", dex, 6, 1, 2, 0 },
	{ 203, "AXS", nop, 5, 0, 2, 0 },
	{ 204, "CPY", cpy, 1, 3, 4, 0 },
	{ 205, "CMP", cmp, 1, 3, 4, 0 },
	{ 206, "DEC", dec, 1, 3, 6, 0 },
	{ 207, "DCP", nop, 1, 0, 6, 0 },
	{ 208, "BNE", bne, 10, 2, 2, 1 },
	{ 209, "CMP", cmp, 9, 2, 5, 1 },
	{ 210, "KIL", nop, 6, 0, 2, 0 },
	{ 211, "DCP", nop, 9, 0, 8, 0 },
	{ 212, "NOP", nop, 12, 2, 4, 0 },
	{ 213, "CMP", cmp, 12, 2, 4, 0 },
	{ 214, "DEC", dec, 12, 2, 6, 0 },
	{ 215, "DCP", nop, 12, 0, 6, 0 },
	{ 216, "CLD", cld, 6, 1, 2, 0 },
	{ 217, "CMP", cmp, 3, 3, 4, 1 },
	{ 218, "NOP", nop, 6, 1, 2, 0 },
	{ 219, "DCP", nop, 3, 0, 7, 0 },
	{ 220, "NOP", nop, 2, 3, 4, 1 },
	{ 221, "CMP", cmp, 2, 3, 4, 1 },
	{ 222, "DEC", dec, 2, 3, 7, 0 },
	{ 223, "DCP", nop, 2, 0, 7, 0 },
	{ 224, "CPX", cpx, 5, 2, 2, 0 },
	{ 225, "SBC", sbc, 7, 2, 6, 0 },
	{ 226, "NOP", nop, 5, 0, 2, 0 },
	{ 227, "ISC", nop, 7, 0, 8, 0 },
	{ 228, "CPX", cpx, 11, 2, 3, 0 },
	{ 229, "SBC", sbc, 11, 2, 3, 0 },
	{ 230, "INC", inc, 11, 2, 5, 0 },
	{ 231, "ISC", nop, 11, 0, 5, 0 },
	{ 232, "INX", inx, 6, 1, 2, 0 },
	{ 233, "SBC", sbc, 5, 2, 2, 0 },
	{ 234, "NOP", nop, 6, 1, 2, 0 },
	{ 235, "SBC", sbc, 5, 0, 2, 0 },
	{ 236, "CPX", cpx, 1, 3, 4, 0 },
	{ 237, "SBC", sbc, 1, 3, 4, 0 },
	{ 238, "INC", inc, 1, 3, 6, 0 },
	{ 239, "ISC", nop, 1, 0, 6, 0 },
	{ 240, "BEQ", beq, 10, 2, 2, 1 },
	{ 241, "SBC", sbc, 9, 2, 5, 1 },
	{ 242, "KIL", nop, 6, 0, 2, 0 },
	{ 243, "ISC", nop, 9, 0, 8, 0 },
	{ 244, "NOP", nop, 12, 2, 4, 0 },
	{ 245, "SBC", sbc, 12, 2, 4, 0 },
	{ 246, "INC", inc, 12, 2, 6, 0 },
	{ 247, "ISC", nop, 12, 0, 6, 0 },
	{ 248, "SED", sed, 6, 1, 2, 0 },
	{ 249, "SBC", sbc, 3, 3, 4, 1 },
	{ 250, "NOP", nop, 6, 1, 2, 0 },
	{ 251, "ISC", nop, 3, 0, 7, 0 },
	{ 252, "NOP", nop, 2, 3, 4, 1 },
	{ 253, "SBC", sbc, 2, 3, 4, 1 },
	{ 254, "INC", inc, 2, 3, 7, 0 },
	{ 255, "ISC", nop, 2, 0, 7, 0 }
};

// Note: move to nes.cpp
void NES::execute(byte opcode) {
	const Instruction& instruction = instructions[opcode];
	CPU* cpu = this->cpu;

	uint16_t address = 0;
	bool page_crossed = false;
	uint16_t offset;

	switch (instruction.mode) {
	case modeAbsolute:
		address = read16(cpu->PC + 1);
		break;
	case modeAbsoluteX:
		address = read16(cpu->PC + 1) + static_cast<uint16_t>(cpu->X);
		page_crossed = pagesDiffer(address - static_cast<uint16_t>(cpu->X), address);
		break;
	case modeAbsoluteY:
		address = read16(cpu->PC + 1) + static_cast<uint16_t>(cpu->Y);
		page_crossed = pagesDiffer(address - static_cast<uint16_t>(cpu->Y), address);
		break;
	case modeAccumulator:
		address = 0;
		break;
	case modeImmediate:
		address = cpu->PC + 1;
		break;
	case modeImplied:
		address = 0;
		break;
	case modeIndexedIndirect:
		address = read16_ff_bug(static_cast<uint16_t>(static_cast<byte>(readByte(cpu->PC + 1) + cpu->X)));
		break;
	case modeIndirect:
		address = read16_ff_bug(read16(cpu->PC + 1));
		break;
	case modeIndirectIndexed:
		address = read16_ff_bug(static_cast<uint16_t>(readByte(cpu->PC + 1))) + static_cast<uint16_t>(cpu->Y);
		page_crossed = pagesDiffer(address - static_cast<uint16_t>(cpu->Y), address);
		break;
	case modeRelative:
		offset = static_cast<uint16_t>(readByte(cpu->PC + 1));
		address = cpu->PC + 2 + offset - ((offset >= 128) << 8);
		break;
	case modeZeroPage:
		address = static_cast<uint16_t>(readByte(cpu->PC + 1));
		break;
	case modeZeroPageX:
		address = static_cast<uint16_t>(static_cast<byte>(readByte(cpu->PC + 1) + cpu->X));
		break;
	case modeZeroPageY:
		address = static_cast<uint16_t>(static_cast<byte>(readByte(cpu->PC + 1) + cpu->Y));
		break;
	}

	cpu->PC += static_cast<uint16_t>(instruction.size);
	cpu->cycles += static_cast<uint64_t>(instruction.cycles);
	if (page_crossed) {
		cpu->cycles += static_cast<uint64_t>(instruction.page_cross_cycles);
	}

	instruction.dispatch(cpu, this, address, instruction.mode);
}

void CPU::setZ(byte value) {
	this->flags = (this->flags & (~(1 << 1))) | (static_cast<byte>(value == 0) << 1);
}

byte CPU::getZ() {
	return (this->flags & 2) >> 1;
}

void CPU::setN(byte value) {
	this->flags = (this->flags & (~(1 << 7))) | (value & 128);
}

byte CPU::getN() {
	return (this->flags & 128) >> 7;
}

void CPU::setZN(byte value) {
	setZ(value);
	setN(value);
}

void CPU::setC(bool value) {
	this->flags = (this->flags & (~(1))) | static_cast<byte>(value);
}

byte CPU::getC() {
	return this->flags & 1;
}

void CPU::setI(bool value) {
	this->flags = (this->flags & (~(1 << 2))) | (static_cast<byte>(value) << 2);
}

byte CPU::getI() {
	return (this->flags & 4) >> 2;
}

void CPU::setD(bool value) {
	this->flags = (this->flags & (~(1 << 3))) | (static_cast<byte>(value) << 3);
}

byte CPU::getD() {
	return (this->flags & 8) >> 3;
}

void CPU::setB(bool value) {
	this->flags = (this->flags & (~(1 << 4))) | (static_cast<byte>(value) << 4);
}

byte CPU::getB() {
	return (this->flags & 16) >> 4;
}

// set overflow flag if 'value' is true
void CPU::setV(bool value) {
	this->flags = (this->flags & (~(1 << 6))) | (static_cast<byte>(value) << 6);
}

// get overflow flag
byte CPU::getV() {
	return (this->flags & 64) >> 6;
}

void CPU::compare(byte a, byte b) {
	setZN(a - b);
	setC(a >= b);
}

// 1 cycle for taking a branch
// 1 cycle if the branch is to a new page
void CPU::branchDelay(uint16_t address, uint16_t pc) {
	++this->cycles;
	if (pagesDiffer(pc, address)) {
		++this->cycles;
	}
}

// ADC - ADd with Carry
void CPU::adc(NES* nes, uint16_t address, byte mode) {
	static_cast<void>(mode);
	const byte a = this->A;
	const byte b = nes->readByte(address);
	const byte c = getC();
	this->A = a + b + c;
	setZN(this->A);
	setC(static_cast<int>(a) + static_cast<int>(b) + static_cast<int>(c) > 0xFF);
	setV(((a^b) & 0x80) == 0 && ((a^this->A) & 0x80) != 0);
}

// AND - logical AND
// Nonstandard name to disambiguate from 'and' label
void CPU::and_instruction(NES* nes, uint16_t address, byte mode) {
	static_cast<void>(mode);
	this->A &= nes->readByte(address);
	setZN(this->A);
}

// ASL - Arithmetic Shift Left
void CPU::asl(NES* nes, uint16_t address, byte mode) {
	if (mode == modeAccumulator) {
		setC((this->A >> 7) & 1);
		this->A <<= 1;
		setZN(this->A);
	}
	else {
		byte value = nes->readByte(address);
		setC((value >> 7) & 1);
		value <<= 1;
		nes->writeByte(address, value);
		setZN(value);
	}
}

// BIT - BIt Test
void CPU::bit(NES* nes, uint16_t address, byte mode) {
	static_cast<void>(mode);
	const byte value = nes->readByte(address);
	setV((value >> 6) & 1);
	setZ(value & this->A);
	setN(value);
}

// CMP - CoMPare
void CPU::cmp(NES* nes, uint16_t address, byte mode) {
	static_cast<void>(mode);
	const byte value = nes->readByte(address);
	compare(this->A, value);
}

// CPX - ComPare X register
void CPU::cpx(NES* nes, uint16_t address, byte mode) {
	static_cast<void>(mode);
	const byte value = nes->readByte(address);
	compare(this->X, value);
}

// CPY - ComPare Y register
void CPU::cpy(NES* nes, uint16_t address, byte mode) {
	static_cast<void>(mode);
	const byte value = nes->readByte(address);
	compare(this->Y, value);
}

// DEC - DECrement memory
void CPU::dec(NES* nes, uint16_t address, byte mode) {
	static_cast<void>(mode);
	const byte value = nes->readByte(address) - 1;
	nes->writeByte(address, value);
	setZN(value);
}


// EOR - Exclusive OR
void CPU::eor(NES* nes, uint16_t address, byte mode) {
	static_cast<void>(mode);
	this->A ^= nes->readByte(address);
	setZN(this->A);
}

// INC - INCrement memory
void CPU::inc(NES* nes, uint16_t address, byte mode) {
	static_cast<void>(mode);
	const byte value = nes->readByte(address) + 1;
	nes->writeByte(address, value);
	setZN(value);
}

// JMP - JuMP
void CPU::jmp(NES* nes, uint16_t address, byte mode) {
	static_cast<void>(nes);
	static_cast<void>(mode);
	this->PC = address;
}

// LDA - LoaD Accumulator
void CPU::lda(NES* nes, uint16_t address, byte mode) {
	static_cast<void>(mode);
	this->A = nes->readByte(address);
	setZN(this->A);
}

// LDX - LoaD X register
void CPU::ldx(NES* nes, uint16_t address, byte mode) {
	static_cast<void>(mode);
	this->X = nes->readByte(address);
	setZN(this->X);
}

// LDY - LoaD Y register
void CPU::ldy(NES* nes, uint16_t address, byte mode) {
	static_cast<void>(mode);
	this->Y = nes->readByte(address);
	setZN(this->Y);
}

// LSR - Logical Shift Right
void CPU::lsr(NES* nes, uint16_t address, byte mode) {
	if (mode == modeAccumulator) {
		setC(this->A & 1);
		this->A >>= 1;
		setZN(this->A);
	}
	else {
		byte value = nes->readByte(address);
		setC(value & 1);
		value >>= 1;
		nes->writeByte(address, value);
		setZN(value);
	}
}

// ORA - logical OR with Accumulator
void CPU::ora(NES* nes, uint16_t address, byte mode) {
	static_cast<void>(mode);
	this->A |= nes->readByte(address);
	setZN(this->A);
}

// PHP - PusH Processor status
void CPU::php(NES* nes, uint16_t address, byte mode) {
	static_cast<void>(address);
	static_cast<void>(mode);
	nes->push(this->flags | 0x10);
}

// ROL - ROtate Left
void CPU::rol(NES* nes, uint16_t address, byte mode) {
	if (mode == modeAccumulator) {
		const byte c = getC();
		setC((this->A >> 7) & 1);
		this->A = (this->A << 1) | c;
		setZN(this->A);
	}
	else {
		const byte c = getC();
		byte value = nes->readByte(address);
		setC((value >> 7) & 1);
		value = (value << 1) | c;
		nes->writeByte(address, value);
		setZN(value);
	}
}

// ROR - ROtate Right
void CPU::ror(NES* nes, uint16_t address, byte mode) {
	if (mode == modeAccumulator) {
		const byte c = getC();
		setC(this->A & 1);
		this->A = (this->A >> 1) | (c << 7);
		setZN(this->A);
	}
	else {
		const byte c = getC();
		byte value = nes->readByte(address);
		setC(value & 1);
		value = (value >> 1) | (c << 7);
		nes->writeByte(address, value);
		setZN(value);
	}
}

// SBC - SuBtract with Carry
void CPU::sbc(NES* nes, uint16_t address, byte mode) {
	static_cast<void>(mode);
	const byte a = this->A;
	const byte b = nes->readByte(address);
	const byte c = getC();
	this->A = a - b - (1 - c);
	setZN(this->A);
	setC(static_cast<int>(a) - static_cast<int>(b) - static_cast<int>(1 - c) >= 0);
	setV(((a^b) & 0x80) != 0 && ((a^this->A) & 0x80) != 0);
}

// SEI - SEt Interrupt disable
void CPU::sei(NES* nes, uint16_t address, byte mode) {
	static_cast<void>(nes);
	static_cast<void>(address);
	static_cast<void>(mode);
	setI(true);
}

// STA - STore Accumulator
void CPU::sta(NES* nes, uint16_t address, byte mode) {
	static_cast<void>(mode);
	nes->writeByte(address, this->A);
}

// STX - Store X Register
void CPU::stx(NES* nes, uint16_t address, byte mode) {
	static_cast<void>(mode);
	nes->writeByte(address, this->X);
}

// STY - STore Y Register
void CPU::sty(NES* nes, uint16_t address, byte mode) {
	static_cast<void>(mode);
	nes->writeByte(address, this->Y);
}

// BRK - force interrupt BReaK
void CPU::brk(NES* nes, uint16_t address, byte mode) {
	nes->push16(this->PC);
	php(nes, address, mode);
	sei(nes, address, mode);
	this->PC = nes->read16(0xFFFE);
}

// BPL - Branch if PLus (i.e. if positive)
void CPU::bpl(NES* nes, uint16_t address, byte mode) {
	static_cast<void>(nes);
	static_cast<void>(mode);
	if (getN() == 0) {
		const uint16_t pc = this->PC;
		this->PC = address;
		branchDelay(address, pc);
	}
}

// CLC - CLear Carry flag
void CPU::clc(NES* nes, uint16_t address, byte mode) {
	static_cast<void>(nes);
	static_cast<void>(address);
	static_cast<void>(mode);
	setC(false);
}

// JSR - Jump to SubRoutine   
void CPU::jsr(NES* nes, uint16_t address, byte mode) {
	static_cast<void>(mode);
	nes->push16(this->PC - 1);
	this->PC = address;
}

// PLP - PuLl Processor status
void CPU::plp(NES* nes, uint16_t address, byte mode) {
	static_cast<void>(address);
	static_cast<void>(mode);
	this->flags = (nes->pop() & 0xEF) | 0x20;
}

// BMI - Branch if MInus (i.e. if negative)
void CPU::bmi(NES* nes, uint16_t address, byte mode) {
	static_cast<void>(nes);
	static_cast<void>(mode);
	if (getN()) {
		const uint16_t pc = this->PC;
		this->PC = address;
		branchDelay(address, pc);
	}
}

// SEC - SEt Carry flag
void CPU::sec(NES* nes, uint16_t address, byte mode) {
	static_cast<void>(nes);
	static_cast<void>(address);
	static_cast<void>(mode);
	setC(true);
}

// RTI - ReTurn from Interrupt
void CPU::rti(NES* nes, uint16_t address, byte mode) {
	static_cast<void>(address);
	static_cast<void>(mode);
	this->flags = (nes->pop() & 0xEF) | 0x20;
	this->PC = nes->pop16();
}

// BVC - Branch if oVerflow Clear
void CPU::bvc(NES* nes, uint16_t address, byte mode) {
	static_cast<void>(nes);
	static_cast<void>(mode);
	if (getV() == 0) {
		const uint16_t pc = this->PC;
		this->PC = address;
		branchDelay(address, pc);
	}
}

// PHA - PusH Accumulator
void CPU::pha(NES* nes, uint16_t address, byte mode) {
	static_cast<void>(address);
	static_cast<void>(mode);
	nes->push(this->A);
}

// CLI - CLear Interrupt disable
void CPU::cli(NES* nes, uint16_t address, byte mode) {
	static_cast<void>(nes);
	static_cast<void>(address);
	static_cast<void>(mode);
	setI(false);
}

// RTS - ReTurn from Subroutine
void CPU::rts(NES* nes, uint16_t address, byte mode) {
	static_cast<void>(address);
	static_cast<void>(mode);
	this->PC = nes->pop16() + 1;
}

// PLA - PuLl Accumulator
void CPU::pla(NES* nes, uint16_t address, byte mode) {
	static_cast<void>(address);
	static_cast<void>(mode);
	this->A = nes->pop();
	setZN(this->A);
}

// BVS - Branch if oVerflow Set
void CPU::bvs(NES* nes, uint16_t address, byte mode) {
	static_cast<void>(nes);
	static_cast<void>(mode);
	if (getV()) {
		const uint16_t pc = this->PC;
		this->PC = address;
		branchDelay(address, pc);
	}
}

// DEY - DEcrement Y register
void CPU::dey(NES* nes, uint16_t address, byte mode) {
	static_cast<void>(nes);
	static_cast<void>(address);
	static_cast<void>(mode);
	--this->Y;
	setZN(this->Y);
}

// TXA - Transfer X to Accumulator
void CPU::txa(NES* nes, uint16_t address, byte mode) {
	static_cast<void>(nes);
	static_cast<void>(address);
	static_cast<void>(mode);
	this->A = this->X;
	setZN(this->A);
}

// BCC - Branch if Carry Clear
void CPU::bcc(NES* nes, uint16_t address, byte mode) {
	static_cast<void>(nes);
	static_cast<void>(mode);
	if (getC() == 0) {
		const uint16_t pc = this->PC;
		this->PC = address;
		branchDelay(address, pc);
	}
}

// TYA - Transfer Y to Accumulator
void CPU::tya(NES* nes, uint16_t address, byte mode) {
	static_cast<void>(nes);
	static_cast<void>(address);
	static_cast<void>(mode);
	this->A = this->Y;
	setZN(this->A);
}

// BCS - Branch if Carry Set
void CPU::bcs(NES* nes, uint16_t address, byte mode) {
	static_cast<void>(nes);
	static_cast<void>(mode);
	if (getC()) {
		const uint16_t pc = this->PC;
		this->PC = address;
		branchDelay(address, pc);
	}
}

// TAY - Transfer Accumulator to Y
void CPU::tay(NES* nes, uint16_t address, byte mode) {
	static_cast<void>(nes);
	static_cast<void>(address);
	static_cast<void>(mode);
	this->Y = this->A;
	setZN(this->Y);
}

// TXS - Transfer X to Stack pointer
void CPU::txs(NES* nes, uint16_t address, byte mode) {
	static_cast<void>(nes);
	static_cast<void>(address);
	static_cast<void>(mode);
	this->SP = this->X;
}

// TAX - Transfer Accumulator to X
void CPU::tax(NES* nes, uint16_t address, byte mode) {
	static_cast<void>(nes);
	static_cast<void>(address);
	static_cast<void>(mode);
	this->X = this->A;
	setZN(this->X);
}

// CLV - CLear oVerflow flag
void CPU::clv(NES* nes, uint16_t address, byte mode) {
	static_cast<void>(nes);
	static_cast<void>(address);
	static_cast<void>(mode);
	setV(false);
}

// TSX - Transfer Stack pointer to X
void CPU::tsx(NES* nes, uint16_t address, byte mode) {
	static_cast<void>(nes);
	static_cast<void>(address);
	static_cast<void>(mode);
	this->X = this->SP;
	setZN(this->X);
}

// INY - INcrement Y register
void CPU::iny(NES* nes, uint16_t address, byte mode) {
	static_cast<void>(nes);
	static_cast<void>(address);
	static_cast<void>(mode);
	++this->Y;
	setZN(this->Y);
}

// DEX - DEcrement X register
void CPU::dex(NES* nes, uint16_t address, byte mode) {
	static_cast<void>(nes);
	static_cast<void>(address);
	static_cast<void>(mode);
	--this->X;
	setZN(this->X);
}

// BNE - Branch if Not Equal
void CPU::bne(NES* nes, uint16_t address, byte mode) {
	static_cast<void>(nes);
	static_cast<void>(mode);
	if (getZ() == 0) {
		const uint16_t pc = this->PC;
		this->PC = address;
		branchDelay(address, pc);
	}
}

// CLD - CLear Decimal mode
void CPU::cld(NES* nes, uint16_t address, byte mode) {
	static_cast<void>(nes);
	static_cast<void>(address);
	static_cast<void>(mode);
	setD(false);
}

// INX - INcrement X register
void CPU::inx(NES* nes, uint16_t address, byte mode) {
	static_cast<void>(nes);
	static_cast<void>(address);
	static_cast<void>(mode);
	++this->X;
	setZN(this->X);
}

// BEQ - Branch if EQual
void CPU::beq(NES* nes, uint16_t address, byte mode) {
	static_cast<void>(nes);
	static_cast<void>(mode);
	if (getZ()) {
		const uint16_t pc = this->PC;
		this->PC = address;
		branchDelay(address, pc);
	}
}

// SED - SEt Decimal flag
void CPU::sed(NES* nes, uint16_t address, byte mode) {
	static_cast<void>(nes);
	static_cast<void>(address);
	static_cast<void>(mode);
	setD(true);
}

// NOP - No OPeration
void CPU::nop(NES* nes, uint16_t address, byte mode) {
	static_cast<void>();
	static_cast<void>(nes);
	static_cast<void>(address);
	static_cast<void>(mode);
}
