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

uint8_t acc_addressing_mode::load(Cpu cpu) {
    return cpu.A;
}

void acc_addressing_mode::store(Cpu cpu, uint8_t val) {
    cpu.A = val;
}

uint8_t imm_addressing_mode::load(Cpu cpu) {
    return cpu.loadb_bump_pc();
}

uint8_t mem_addressing_mode::load(Cpu cpu) {
    return cpu.loadb(this->mem);
}

void mem_addressing_mode::store(Cpu cpu, uint8_t val) {
    cpu.storeb(this->mem, val);
};

Cpu::Cpu() : cycles(0), PC(0xc000), SP(0xfd), A(0), X(0), Y(0), flags(0x24), interrupt(0), stall(0) {}

void Cpu::storeb(uint16_t addr, uint8_t val) {
	mem.storeb(addr, val);
}

void Cpu::storew(uint16_t addr, uint16_t val){
    mem.storeb(addr, uint8_t(val & 0xff));
    mem.storeb(addr + 1, uint8_t((val >> 8) & 0xff));
}

uint8_t Cpu::loadb(uint16_t addr) {
	return mem.loadb(addr);
}

uint16_t Cpu::loadw(uint16_t addr) {
    return mem.loadb(addr) | uint16_t(mem.loadb(addr + 1)) << 8;
}

uint8_t Cpu::loadb_bump_pc() {
	auto val = this->loadb(PC);
	PC += 1;
	return val;
}

uint16_t Cpu::loadw_bump_pc() {
    auto val = this->loadw(PC);
    PC += 1;
    return val;
}

void Cpu::pushb(uint8_t val) {
	this->storeb(0x100 + uint16_t(SP), val);
    SP -= 1;
}

void Cpu::pushw(uint16_t val) {
    pushb(val >> 8);
    pushb(val & 0xff);
}

uint8_t Cpu::popb() {
    auto val = loadb(0x100 + uint16_t(SP) + 1);
    SP += 1;
    return val;
}

uint16_t Cpu::popw() {
    auto val = loadw(0x100 + uint16_t(SP) + 1);
    SP += 2;
    return val;
}

bool Cpu::get_flag(uint8_t flag) {
    return (flags & flag) != 0;
}

void Cpu::set_flag(uint8_t flag, bool on) {
    if (on)
        flags = flags | flag;
    else
        flags = flags & !flag;
}

imm_addressing_mode Cpu::immediate() { imm_addressing_mode();}

acc_addressing_mode Cpu::accumulator() { acc_addressing_mode();}

mem_addressing_mode Cpu::zero_page() {
    mem_addressing_mode(loadb_bump_pc());
}
