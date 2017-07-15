#include <array>
#include "include/cpu.h"

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

template <class Mem>
uint8_t acc_addressing_mode<Mem>::load(Cpu<Mem> cpu) {
    return cpu.A;
}

template <class Mem>
void acc_addressing_mode<Mem>::store(Cpu<Mem> cpu, uint8_t val) {
    cpu.A = val;
}

template <class Mem>
uint8_t imm_addressing_mode<Mem>::load(Cpu<Mem> cpu) {
    return cpu.loadb_bump_pc();
}

template <class Mem>
uint8_t mem_addressing_mode<Mem>::load(Cpu<Mem> cpu) {
    return cpu.loadb(this->mem);
}

template <class Mem>
void mem_addressing_mode<Mem>::store(Cpu<Mem> cpu, uint8_t val) {
    cpu.storeb(this->mem, val);
};

template <class Mem>
Cpu<Mem>::Cpu() : cycles(0), PC(0xc000), SP(0xfd), A(0), X(0), Y(0), flags(0x24), interrupt(0), stall(0) {}

template <class Mem>
void Cpu<Mem>::storeb(uint16_t addr, uint8_t val) {
	mem.storeb(addr, val);
}
template <class Mem>
void Cpu<Mem>::storew(uint16_t addr, uint16_t val){
    mem.storeb(addr, uint8_t(val & 0xff));
    mem.storeb(addr + 1, uint8_t((val >> 8) & 0xff));
}
template <class Mem> 
uint8_t Cpu<Mem>::loadb(uint16_t addr) {
	return mem.loadb(addr);
}

template <class Mem> 
uint16_t Cpu<Mem>::loadw(uint16_t addr) {
    return mem.loadb(addr) | uint16_t(mem.loadb(addr + 1)) << 8;
}
template <class Mem> 
uint8_t Cpu<Mem>::loadb_bump_pc() {
	auto val = this->loadb(PC);
	PC += 1;
	return val;
}

template <class Mem> 
uint16_t Cpu<Mem>::loadw_bump_pc() {
    auto val = this->loadw(PC);
    PC += 1;
    return val;
}
template <class Mem>
void Cpu<Mem>::pushb(uint8_t val) {
	this->storeb(0x100 + uint16_t(SP), val);
    SP -= 1;
}

template <class Mem>
void Cpu<Mem>::pushw(uint16_t val) {
    pushb(val >> 8);
    pushb(val & 0xff);
}

template <class Mem> 
uint8_t Cpu<Mem>::popb() {
    auto val = loadb(0x100 + uint16_t(this->SP) + 1);
    SP += 1;
    return val;
}

template <class Mem> 
uint16_t Cpu<Mem>::popw() {
    auto val = loadw(0x100 + uint16_t(SP) + 1);
    SP += 2;
    return val;
}

template <class Mem>
bool Cpu<Mem>::get_flag(uint8_t flag) {
    return (flags & flag) != 0;
}

template <class Mem>
void Cpu<Mem>::set_flag(uint8_t flag, bool on) {
    if (on)
        flags = flags | flag;
    else
        flags = flags & !flag;
}

template <class Mem>
imm_addressing_mode<Mem> Cpu<Mem>::immediate() { return imm_addressing_mode<Mem>(); }

template <class Mem>
acc_addressing_mode<Mem> Cpu<Mem>::accumulator() { return acc_addressing_mode<Mem>(); }

template <class Mem>
mem_addressing_mode<Mem> Cpu<Mem>::zero_page() { return mem_addressing_mode<Mem>(loadb_bump_pc() + X); }

template <class Mem>
mem_addressing_mode<Mem> Cpu<Mem>::absolute() { return mem_addressing_mode<Mem>(loadb_bump_pc()); }

/**
 * 6502 intructions
 *
 *
 *
 */

template <class Mem>
void Cpu<Mem>::sta(addressing_mode<Mem> am) { am.store(*this, A); }

template <class Mem>
void Cpu<Mem>::stx(addressing_mode<Mem> am) { am.store(*this, X); }

template <class Mem>
void Cpu<Mem>::sty(addressing_mode<Mem> am) { am.store(*this, Y); }
