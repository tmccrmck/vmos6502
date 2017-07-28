#include <array>
#include <iomanip>
#include <iostream>
#include "include/cpu.h"

constexpr uint8_t CARRY_FLAG    = 1 << 0;
constexpr uint8_t ZERO_FLAG     = 1 << 1;
constexpr uint8_t IRQ_FLAG      = 1 << 2;
constexpr uint8_t DECIMAL_FLAG  = 1 << 3;
constexpr uint8_t BREAK_FLAG    = 1 << 4;
constexpr uint8_t OVERFLOW_FLAG = 1 << 6;
constexpr uint8_t NEGATIVE_FLAG = 1 << 7;

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

// Addressing modes

template <class Mem>
uint8_t AccAddressingMode<Mem>::load(Cpu<Mem>& cpu) {
    return cpu.A;
}

template <class Mem>
void AccAddressingMode<Mem>::store(Cpu<Mem>& cpu, uint8_t val) {
    cpu.A = val;
}

template <class Mem>
uint8_t ImmAddressingMode<Mem>::load(Cpu<Mem>& cpu) {
    return cpu.loadb_bump_pc();
}

template <class Mem>
uint8_t MemoryAddressingMode<Mem>::load(Cpu<Mem>& cpu) {
    return cpu.loadb(this->cur_val);
}

template <class Mem>
void MemoryAddressingMode<Mem>::store(Cpu<Mem>& cpu, uint8_t val) {
    cpu.storeb(this->cur_val, val);
};

// Cpu implementation

template <class Mem>
Cpu<Mem>::Cpu() : cycles(0), PC(0xc000), SP(0xfd), A(0), X(0), Y(0), flags(0x24), interrupt(0), stall(0) {}


template <class Mem>
void Cpu<Mem>::storeb(uint16_t addr, uint8_t val) { mem.storeb(addr, val); }

template <class Mem>
uint8_t Cpu<Mem>::loadb(uint16_t addr) {
	return mem.loadb(addr);
}

template <class Mem>
void Cpu<Mem>::storew(uint16_t addr, uint16_t val) {
    storeb(addr, static_cast<uint8_t>(val & 0xFF));
    storeb(static_cast<uint16_t>(addr + 1), static_cast<uint8_t>((val >> 8) & 0xFF));
}

template <class Mem>
uint16_t Cpu<Mem>::loadw(uint16_t addr) {
    return loadb(addr) | loadb(addr) << 8;
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
uint8_t Cpu<Mem>::set_zn(uint8_t val){
    set_flag(ZERO_FLAG, val == 0);
    set_flag(NEGATIVE_FLAG, (val & 0x80) != 0);
    return val;
}

template <class Mem>
ImmAddressingMode<Mem> Cpu<Mem>::immediate() { return ImmAddressingMode<Mem>(); }

template <class Mem>
AccAddressingMode<Mem> Cpu<Mem>::accumulator() { return AccAddressingMode<Mem>(); }

template <class Mem>
MemoryAddressingMode<Mem> Cpu<Mem>::zero_page() { return MemoryAddressingMode<Mem>(loadb_bump_pc() + X); }

template <class Mem>
MemoryAddressingMode<Mem> Cpu<Mem>::absolute() { return MemoryAddressingMode<Mem>(loadb_bump_pc()); }

template <class Mem>
void Cpu<Mem>::step() {
    auto op = loadb_bump_pc();

    switch(op){
        case 0xa5: { auto v = zero_page(); lda(v); break; }
        default: std::cerr << "Unrecognized operation: " << std::hex << op;

    }
}

/**
 * 6502 instructions
 *
 *
 *
 */

// loads

template <class Mem>
void Cpu<Mem>::lda(AddressingMode<Mem> am){
    auto val = am.load(*this);
    A = set_zn(val);
}

template <class Mem>
void Cpu<Mem>::ldx(AddressingMode<Mem> am){
    auto val = am.load(*this);
    X = set_zn(val);
}

template <class Mem>
void Cpu<Mem>::ldy(AddressingMode<Mem> am){
    auto val = am.load(*this);
    Y = set_zn(val);
}

// stores

template <class Mem>
void Cpu<Mem>::sta(AddressingMode<Mem> am) { am.store(*this, A); }

template <class Mem>
void Cpu<Mem>::stx(AddressingMode<Mem> am) { am.store(*this, X); }

template <class Mem>
void Cpu<Mem>::sty(AddressingMode<Mem> am) { am.store(*this, Y); }

// arithmetic

template <class Mem>
void Cpu<Mem>::adc(AddressingMode<Mem> am) {
	auto val = am.load(*this);
	auto result = uint32_t(A) + uint32_t(val);
	if (get_flag(CARRY_FLAG))
		result += 1;
	set_flag(CARRY_FLAG, (result & 0x100) != 0);
	result = uint8_t(result);
	set_flag(OVERFLOW_FLAG, ((A ^ val) & 0x80) == 0 && ((A ^ result) & 0x80) == 0x80);
	A = set_zn(result);
}

template <class Mem>
void Cpu<Mem>::sbc(AddressingMode<Mem> am) {
	auto val = am.load(*this);
	auto result = uint32_t(A) - uint32_t(val);
	if (!get_flag(CARRY_FLAG))
		result -= 1;
	set_flag(CARRY_FLAG, (result & 0x100) != 0);
	result = uint8_t(result);
	set_flag(OVERFLOW_FLAG, ((A ^ result) & 0x80) != 0 && ((A ^ val) & 0x80) == 0x80);
	A = set_zn(result);
}

// comparisons

template <class Mem>
void Cpu<Mem>::cmp_base(uint8_t reg, AddressingMode<Mem> am) {
	auto v = am.load(*this);
	auto result = uint32_t(reg) - uint32_t(v);
	set_flag(CARRY_FLAG, (result & 0x100) == 0);
	set_zn(result);
}

template <class Mem>
void Cpu<Mem>::cmp(AddressingMode<Mem> am) { cmp_base(A, am); }

template <class Mem>
void Cpu<Mem>::cmpx(AddressingMode<Mem> am) { cmp_base(X, am); }

template <class Mem>
void Cpu<Mem>::cmpy(AddressingMode<Mem> am) { cmp_base(Y, am); }

// bitwise

template <class Mem>
void Cpu<Mem>::anda(AddressingMode<Mem> am) {
    auto val = am.load(*this) & A;
    A = set_zn(val);
}

template <class Mem>
void Cpu<Mem>::ora(AddressingMode<Mem> am) {
    auto val = am.load(*this) | A;
    A = set_zn(val);
}

template <class Mem>
void Cpu<Mem>::xora(AddressingMode<Mem> am) {
    auto val = am.load(*this) ^ A;
    A = set_zn(val);
}

template <class Mem>
void Cpu<Mem>::bit(AddressingMode<Mem> am) {
	auto val = am.load(*this);
	set_flag(ZERO_FLAG, (val & A) == 0);
    set_flag(NEGATIVE_FLAG, (val & 0x80) != 0);
    set_flag(OVERFLOW_FLAG, (val & 0x40) != 0);
}

// shifts
template <class Mem>
void Cpu<Mem>::shl_base(bool lsb, AddressingMode<Mem> am) {
    auto val = am.load(*this);
    auto new_carry = (val & 0x80) != 0;
    auto result = val << 1;
    if (lsb)
        result |= 1;
    set_flag(CARRY_FLAG, new_carry);
    val = set_zn(uint8_t(result));
    am.store(*this, val);
}

template <class Mem>
void Cpu<Mem>::shr_base(bool msb, AddressingMode<Mem> am) {
    auto val = am.load(*this);
    auto new_carry = (val & 0x1) != 0;
    auto result = val >> 1;
    if (msb)
        result |= 0x80;
    set_flag(CARRY_FLAG, new_carry);
    val = set_zn(uint8_t(result));
    am.store(*this, val);
}

template <class Mem>
void Cpu<Mem>::rol(AddressingMode<Mem> am) {
    auto val = get_flag(CARRY_FLAG);
    shl_base(val, am);
}

template <class Mem>
void Cpu<Mem>::ror(AddressingMode<Mem> am) {
    auto val = get_flag(CARRY_FLAG);
    shr_base(val, am);

}

template <class Mem>
void Cpu<Mem>::asl(AddressingMode<Mem> am) { shl_base(false, am); }

template <class Mem>
void Cpu<Mem>::lsr(AddressingMode<Mem> am) { shr_base(false, am); }

template <class Mem>
void Cpu<Mem>::inc(AddressingMode<Mem> am) {
    auto val = am.load(*this);
    val = set_zn(val + 1);
    am.store(*this, val);
}

template <class Mem>
void Cpu<Mem>::dec(AddressingMode<Mem> am) {
    auto val = am.load(*this);
    val = set_zn(val - 1);
    am.store(*this, val);
}

template <class Mem>
void Cpu<Mem>::clc() { set_flag(CARRY_FLAG, false); }

template <class Mem>
void Cpu<Mem>::sec() { set_flag(CARRY_FLAG, true); }

template <class Mem>
void Cpu<Mem>::sed() { set_flag(DECIMAL_FLAG, true); }

template <class Mem>
void Cpu<Mem>::clv() { set_flag(OVERFLOW_FLAG, false); }

template <class Mem>
void Cpu<Mem>::cli() { set_flag(IRQ_FLAG, false); }

template <class Mem>
void Cpu<Mem>::sei() { set_flag(IRQ_FLAG, true); }

template <class Mem>
void Cpu<Mem>::cld() { set_flag(DECIMAL_FLAG, false); }

template <class Mem>
void Cpu<Mem>::bra_base(bool cond) {
    auto disp = loadb_bump_pc();
    if (cond) {
        PC = (PC + disp);
    }
}

template <class Mem>
void Cpu<Mem>::bpl() {
    auto flag = !get_flag(NEGATIVE_FLAG);
    bra_base(flag);
}

template <class Mem>
void Cpu<Mem>::bmi() {
    auto flag = get_flag(NEGATIVE_FLAG);
    bra_base(flag);
}

template <class Mem>
void Cpu<Mem>::bvc() {
    auto flag = !get_flag(OVERFLOW_FLAG);
    bra_base(flag);
}

template <class Mem>
void Cpu<Mem>::bvs() {
    auto flag = get_flag(OVERFLOW_FLAG);
    bra_base(flag);
}

template <class Mem>
void Cpu<Mem>::bcc() {
    auto flag = !get_flag(CARRY_FLAG);
    bra_base(flag);
}

template <class Mem>
void Cpu<Mem>::bcs() {
    auto flag = get_flag(CARRY_FLAG);
    bra_base(flag);
}

template <class Mem>
void Cpu<Mem>::bne() {
    auto flag = !get_flag(ZERO_FLAG);
    bra_base(flag);
}

template <class Mem>
void Cpu<Mem>::beq() {
    auto flag = get_flag(ZERO_FLAG);
    bra_base(flag);
}
