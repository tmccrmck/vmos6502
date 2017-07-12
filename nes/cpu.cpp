#include <array>
#include "include/cpu.h"

std::array<uint8_t, 256> instr_modes = {
		6, 7, 6, 7, 11, 11, 11, 11, 6, 5, 4, 5, 1, 1, 1, 1,
		10, 9, 6, 9, 12, 12, 12, 12, 6, 3, 6, 3, 2, 2, 2, 2,
		1, 7, 6, 7, 11, 11, 11, 11, 6, 5, 4, 5, 1, 1, 1, 1,
		10, 9, 6, 9, 12, 12, 12, 12, 6, 3, 6, 3, 2, 2, 2, 2,
		6, 7, 6, 7, 11, 11, 11, 11, 6, 5, 4, 5, 1, 1, 1, 1,
		10, 9, 6, 9, 12, 12, 12, 12, 6, 3, 6, 3, 2, 2, 2, 2,
		6, 7, 6, 7, 11, 11, 11, 11, 6, 5, 4, 5, 8, 1, 1, 1,
		10, 9, 6, 9, 12, 12, 12, 12, 6, 3, 6, 3, 2, 2, 2, 2,
		5, 7, 5, 7, 11, 11, 11, 11, 6, 5, 6, 5, 1, 1, 1, 1,
		10, 9, 6, 9, 12, 12, 13, 13, 6, 3, 6, 3, 2, 2, 3, 3,
		5, 7, 5, 7, 11, 11, 11, 11, 6, 5, 6, 5, 1, 1, 1, 1,
		10, 9, 6, 9, 12, 12, 13, 13, 6, 3, 6, 3, 2, 2, 3, 3,
		5, 7, 5, 7, 11, 11, 11, 11, 6, 5, 6, 5, 1, 1, 1, 1,
		10, 9, 6, 9, 12, 12, 12, 12, 6, 3, 6, 3, 2, 2, 2, 2,
		5, 7, 5, 7, 11, 11, 11, 11, 6, 5, 6, 5, 1, 1, 1, 1,
		10, 9, 6, 9, 12, 12, 12, 12, 6, 3, 6, 3, 2, 2, 2, 2,
};

std::array<uint8_t, 256> instr_cycles = {
	/*0x00*/ 7, 6, 2, 8, 3, 3, 5, 5, 3, 2, 2, 2, 4, 4, 6, 6, 
	/*0x10*/ 2, 5, 2, 8, 4, 4, 6, 6, 2, 4, 2, 7, 4, 4, 7, 7, 
	/*0x20*/ 6, 6, 2, 8, 3, 3, 5, 5, 4, 2, 2, 2, 4, 4, 6, 6, 
	/*0x30*/ 2, 5, 2, 8, 4, 4, 6, 6, 2, 4, 2, 7, 4, 4, 7, 7, 
	/*0x40*/ 6, 6, 2, 8, 3, 3, 5, 5, 3, 2, 2, 2, 3, 4, 6, 6, 
	/*0x50*/ 2, 5, 2, 8, 4, 4, 6, 6, 2, 4, 2, 7, 4, 4, 7, 7, 
	/*0x60*/ 6, 6, 2, 8, 3, 3, 5, 5, 4, 2, 2, 2, 5, 4, 6, 6, 
	/*0x70*/ 2, 5, 2, 8, 4, 4, 6, 6, 2, 4, 2, 7, 4, 4, 7, 7, 
	/*0x80*/ 2, 6, 2, 6, 3, 3, 3, 3, 2, 2, 2, 2, 4, 4, 4, 4, 
	/*0x90*/ 2, 6, 2, 6, 4, 4, 4, 4, 2, 5, 2, 5, 5, 5, 5, 5, 
	/*0xA0*/ 2, 6, 2, 6, 3, 3, 3, 3, 2, 2, 2, 2, 4, 4, 4, 4, 
	/*0xB0*/ 2, 5, 2, 5, 4, 4, 4, 4, 2, 4, 2, 4, 4, 4, 4, 4, 
	/*0xC0*/ 2, 6, 2, 8, 3, 3, 5, 5, 2, 2, 2, 2, 4, 4, 6, 6, 
	/*0xD0*/ 2, 5, 2, 8, 4, 4, 6, 6, 2, 4, 2, 7, 4, 4, 7, 7, 
	/*0xE0*/ 2, 6, 3, 8, 3, 3, 5, 5, 2, 2, 2, 2, 4, 4, 6, 6, 
	/*0xF0*/ 2, 5, 2, 8, 4, 4, 6, 6, 2, 4, 2, 7, 4, 4, 7, 7, 
};

std::array<std::string,  256> instr_names = {
		"BRK", "ORA", "KIL",  "SLO",  "NOP", "ORA", "ASL", "SLO",
		"PHP", "ORA", "ASL", "ANC", "NOP", "ORA", "ASL", "SLO",
		"BPL", "ORA", "KIL", "SLO", "NOP", "ORA", "ASL", "SLO",
		"CLC", "ORA", "NOP", "SLO", "NOP", "ORA", "ASL", "SLO",
		"JSR", "AND", "KIL", "RLA", "BIT", "AND", "ROL", "RLA",
		"PLP", "AND", "ROL", "ANC", "BIT", "AND", "ROL", "RLA",
		"BMI", "AND", "KIL", "RLA", "NOP", "AND", "ROL", "RLA",
		"SEC", "AND", "NOP", "RLA", "NOP", "AND", "ROL", "RLA",
		"RTI", "EOR", "KIL", "SRE", "NOP", "EOR", "LSR", "SRE",
		"PHA", "EOR", "LSR", "ALR", "JMP", "EOR", "LSR", "SRE",
		"BVC", "EOR", "KIL", "SRE", "NOP", "EOR", "LSR", "SRE",
		"CLI", "EOR", "NOP", "SRE", "NOP", "EOR", "LSR", "SRE",
		"RTS", "ADC", "KIL", "RRA", "NOP", "ADC", "ROR", "RRA",
		"PLA", "ADC", "ROR", "ARR", "JMP", "ADC", "ROR", "RRA",
		"BVS", "ADC", "KIL", "RRA", "NOP", "ADC", "ROR", "RRA",
		"SEI", "ADC", "NOP", "RRA", "NOP", "ADC", "ROR", "RRA",
		"NOP", "STA", "NOP", "SAX", "STY", "STA", "STX", "SAX",
		"DEY", "NOP", "TXA", "XAA", "STY", "STA", "STX", "SAX",
		"BCC", "STA", "KIL", "AHX", "STY", "STA", "STX", "SAX",
		"TYA", "STA", "TXS", "TAS", "SHY", "STA", "SHX", "AHX",
		"LDY", "LDA", "LDX", "LAX", "LDY", "LDA", "LDX", "LAX",
		"TAY", "LDA", "TAX", "LAX", "LDY", "LDA", "LDX", "LAX",
		"BCS", "LDA", "KIL", "LAX", "LDY", "LDA", "LDX", "LAX",
		"CLV", "LDA", "TSX", "LAS", "LDY", "LDA", "LDX", "LAX",
		"CPY", "CMP", "NOP", "DCP", "CPY", "CMP", "DEC", "DCP",
		"INY", "CMP", "DEX", "AXS", "CPY", "CMP", "DEC", "DCP",
		"BNE", "CMP", "KIL", "DCP", "NOP", "CMP", "DEC", "DCP",
		"CLD", "CMP", "NOP", "DCP", "NOP", "CMP", "DEC", "DCP",
		"CPX", "SBC", "NOP", "ISC", "CPX", "SBC", "INC", "ISC",
		"INX", "SBC", "NOP", "SBC", "CPX", "SBC", "INC", "ISC",
		"BEQ", "SBC", "KIL", "ISC", "NOP", "SBC", "INC", "ISC",
		"SED", "SBC", "NOP", "ISC", "NOP", "SBC", "INC", "ISC",
};


Cpu::Cpu() : cycles(0), PC(0xc000), SP(0xfd), A(0), X(0), Y(0), flags(0x24), interrupt(0), stall(0) {}

void Cpu::storeb(uint16_t addr, uint8_t val) {
	mem.storeb(addr, val);
}

uint8_t Cpu::loadb(uint16_t addr) {
	return mem.loadb(addr);
}

uint8_t Cpu::loadb_bump_pc() {
	auto val = loadb(PC);
	PC += 1;
	return val;
}

uint8_t memmap::loadb(uint16_t addr) {
	// TODO
	return 0;
}

void memmap::storeb(uint16_t self, uint8_t val) { return; /* TODO */ }

class acc_addressing_mode {
public:
	uint8_t load(Cpu cpu) {
		return cpu.A;
	}
	void store(Cpu cpu, uint8_t val) {
		cpu.A = val;
	}
};

class imm_addressing_mode {
public:
	uint8_t load(Cpu cpu) {
		return cpu.loadb_bump_pc();
	}
};

class mem_addressing_mode {
public:
	uint16_t mem;
	mem_addressing_mode(uint16_t _mem) : mem(_mem) {}
	uint8_t load(Cpu cpu) {
		return cpu.loadb(mem);
	}
	void store(Cpu cpu, uint8_t val) {
		return cpu.storeb(mem, val);
	}
};

