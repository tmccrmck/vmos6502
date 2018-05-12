#include "cpu.h"

template<typename Mem>
byte CPU<Mem>::readb(uint16_t addr) {
	return nes->readByte(addr);
}
// do addresses represent different pages?
template <class Mem>
bool CPU<Mem>::pagesDiffer(uint16_t a, uint16_t b) {
	return (a & 0xFF00) != (b & 0xFF00);
}

template <class Mem>
void CPU<Mem>::executeOpcode(byte opcode, uint16_t address, Instruction instruction) {
    	switch(opcode){
		case 0x0:
			this->brk(address, instruction.mode);
			break;
		case 0x1:
			this->ora(address, instruction.mode);
			break;
		case 0x2:
			this->nop(address, instruction.mode);
			break;
		case 0x3:
			this->nop(address, instruction.mode);
			break;
		case 0x4:
			this->nop(address, instruction.mode);
			break;
		case 0x5:
			this->ora(address, instruction.mode);
			break;
		case 0x6:
			this->asl(address, instruction.mode);
			break;
		case 0x7:
			this->nop(address, instruction.mode);
			break;
		case 0x8:
			this->php(address, instruction.mode);
			break;
		case 0x9:
			this->ora(address, instruction.mode);
			break;
		case 0xa:
			this->asl(address, instruction.mode);
			break;
		case 0xb:
			this->nop(address, instruction.mode);
			break;
		case 0xc:
			this->nop(address, instruction.mode);
			break;
		case 0xd:
			this->ora(address, instruction.mode);
			break;
		case 0xe:
			this->asl(address, instruction.mode);
			break;
		case 0xf:
			this->nop(address, instruction.mode);
			break;
		case 0x10:
			this->bpl(address, instruction.mode);
			break;
		case 0x11:
			this->ora(address, instruction.mode);
			break;
		case 0x12:
			this->nop(address, instruction.mode);
			break;
		case 0x13:
			this->nop(address, instruction.mode);
			break;
		case 0x14:
			this->nop(address, instruction.mode);
			break;
		case 0x15:
			this->ora(address, instruction.mode);
			break;
		case 0x16:
			this->asl(address, instruction.mode);
			break;
		case 0x17:
			this->nop(address, instruction.mode);
			break;
		case 0x18:
			this->clc(address, instruction.mode);
			break;
		case 0x19:
			this->ora(address, instruction.mode);
			break;
		case 0x1a:
			this->nop(address, instruction.mode);
			break;
		case 0x1b:
			this->nop(address, instruction.mode);
			break;
		case 0x1c:
			this->nop(address, instruction.mode);
			break;
		case 0x1d:
			this->ora(address, instruction.mode);
			break;
		case 0x1e:
			this->asl(address, instruction.mode);
			break;
		case 0x1f:
			this->nop(address, instruction.mode);
			break;
		case 0x20:
			this->jsr(address, instruction.mode);
			break;
		case 0x21:
			this->and_instruction(address, instruction.mode);
			break;
		case 0x22:
			this->nop(address, instruction.mode);
			break;
		case 0x23:
			this->nop(address, instruction.mode);
			break;
		case 0x24:
			this->bit(address, instruction.mode);
			break;
		case 0x25:
			this->and_instruction(address, instruction.mode);
			break;
		case 0x26:
			this->rol(address, instruction.mode);
			break;
		case 0x27:
			this->nop(address, instruction.mode);
			break;
		case 0x28:
			this->plp(address, instruction.mode);
			break;
		case 0x29:
			this->and_instruction(address, instruction.mode);
			break;
		case 0x2a:
			this->rol(address, instruction.mode);
			break;
		case 0x2b:
			this->nop(address, instruction.mode);
			break;
		case 0x2c:
			this->bit(address, instruction.mode);
			break;
		case 0x2d:
			this->and_instruction(address, instruction.mode);
			break;
		case 0x2e:
			this->rol(address, instruction.mode);
			break;
		case 0x2f:
			this->nop(address, instruction.mode);
			break;
		case 0x30:
			this->bmi(address, instruction.mode);
			break;
		case 0x31:
			this->and_instruction(address, instruction.mode);
			break;
		case 0x32:
			this->and_instruction(address, instruction.mode);
			break;
		case 0x33:
			this->nop(address, instruction.mode);
			break;
		case 0x34:
			this->nop(address, instruction.mode);
			break;
		case 0x35:
			this->nop(address, instruction.mode);
			break;
		case 0x36:
			this->rol(address, instruction.mode);
			break;
		case 0x37:
			this->nop(address, instruction.mode);
			break;
		case 0x38:
			this->sec(address, instruction.mode);
			break;
		case 0x39:
			this->and_instruction(address, instruction.mode);
			break;
		case 0x3a:
			this->nop(address, instruction.mode);
			break;
		case 0x3b:
			this->nop(address, instruction.mode);
			break;
		case 0x3c:
			this->nop(address, instruction.mode);
			break;
		case 0x3d:
			this->and_instruction(address, instruction.mode);
			break;
		case 0x3e:
			this->rol(address, instruction.mode);
			break;
		case 0x3f:
			this->nop(address, instruction.mode);
			break;
		case 0x40:
			this->rti(address, instruction.mode);
			break;
		case 0x41:
			this->eor(address, instruction.mode);
			break;
		case 0x42:
			this->nop(address, instruction.mode);
			break;
		case 0x43:
			this->nop(address, instruction.mode);
			break;
		case 0x44:
			this->nop(address, instruction.mode);
			break;
		case 0x45:
			this->eor(address, instruction.mode);
			break;
		case 0x46:
			this->lsr(address, instruction.mode);
			break;
		case 0x47:
			this->nop(address, instruction.mode);
			break;
		case 0x48:
			this->pha(address, instruction.mode);
			break;
		case 0x49:
			this->eor(address, instruction.mode);
			break;
		case 0x4a:
			this->lsr(address, instruction.mode);
			break;
		case 0x4b:
			this->nop(address, instruction.mode);
			break;
		case 0x4c:
			this->jmp(address, instruction.mode);
			break;
		case 0x4d:
			this->eor(address, instruction.mode);
			break;
		case 0x4e:
			this->lsr(address, instruction.mode);
			break;
		case 0x4f:
			this->nop(address, instruction.mode);
			break;
		case 0x50:
			this->bvc(address, instruction.mode);
			break;
		case 0x51:
			this->eor(address, instruction.mode);
			break;
		case 0x52:
			this->nop(address, instruction.mode);
			break;
		case 0x53:
			this->nop(address, instruction.mode);
			break;
		case 0x54:
			this->nop(address, instruction.mode);
			break;
		case 0x55:
			this->eor(address, instruction.mode);
			break;
		case 0x56:
			this->lsr(address, instruction.mode);
			break;
		case 0x57:
			this->nop(address, instruction.mode);
			break;
		case 0x58:
			this->cli(address, instruction.mode);
			break;
		case 0x59:
			this->eor(address, instruction.mode);
			break;
		case 0x5a:
			this->nop(address, instruction.mode);
			break;
		case 0x5b:
			this->nop(address, instruction.mode);
			break;
		case 0x5c:
			this->nop(address, instruction.mode);
			break;
		case 0x5d:
			this->eor(address, instruction.mode);
			break;
		case 0x5e:
			this->lsr(address, instruction.mode);
			break;
		case 0x5f:
			this->nop(address, instruction.mode);
			break;
		case 0x60:
			this->rts(address, instruction.mode);
			break;
		case 0x61:
			this->adc(address, instruction.mode);
			break;
		case 0x62:
			this->nop(address, instruction.mode);
			break;
		case 0x63:
			this->nop(address, instruction.mode);
			break;
		case 0x64:
			this->nop(address, instruction.mode);
			break;
		case 0x65:
			this->adc(address, instruction.mode);
			break;
		case 0x66:
			this->ror(address, instruction.mode);
			break;
		case 0x67:
			this->nop(address, instruction.mode);
			break;
		case 0x68:
			this->pla(address, instruction.mode);
			break;
		case 0x69:
			this->adc(address, instruction.mode);
			break;
		case 0x6a:
			this->ror(address, instruction.mode);
			break;
		case 0x6b:
			this->nop(address, instruction.mode);
			break;
		case 0x6c:
			this->jmp(address, instruction.mode);
			break;
		case 0x6d:
			this->adc(address, instruction.mode);
			break;
		case 0x6e:
			this->ror(address, instruction.mode);
			break;
		case 0x6f:
			this->nop(address, instruction.mode);
			break;
		case 0x70:
			this->bvs(address, instruction.mode);
			break;
		case 0x71:
			this->adc(address, instruction.mode);
			break;
		case 0x72:
			this->nop(address, instruction.mode);
			break;
		case 0x73:
			this->nop(address, instruction.mode);
			break;
		case 0x74:
			this->nop(address, instruction.mode);
			break;
		case 0x75:
			this->adc(address, instruction.mode);
			break;
		case 0x76:
			this->ror(address, instruction.mode);
			break;
		case 0x77:
			this->nop(address, instruction.mode);
			break;
		case 0x78:
			this->sei(address, instruction.mode);
			break;
		case 0x79:
			this->adc(address, instruction.mode);
			break;
		case 0x7a:
			this->nop(address, instruction.mode);
			break;
		case 0x7b:
			this->nop(address, instruction.mode);
			break;
		case 0x7c:
			this->nop(address, instruction.mode);
			break;
		case 0x7d:
			this->adc(address, instruction.mode);
			break;
		case 0x7e:
			this->ror(address, instruction.mode);
			break;
		case 0x7f:
			this->nop(address, instruction.mode);
			break;
		case 0x80:
			this->nop(address, instruction.mode);
			break;
		case 0x81:
			this->sta(address, instruction.mode);
			break;
		case 0x82:
			this->nop(address, instruction.mode);
			break;
		case 0x83:
			this->nop(address, instruction.mode);
			break;
		case 0x84:
			this->sty(address, instruction.mode);
			break;
		case 0x85:
			this->sta(address, instruction.mode);
			break;
		case 0x86:
			this->stx(address, instruction.mode);
			break;
		case 0x87:
			this->nop(address, instruction.mode);
			break;
		case 0x88:
			this->dey(address, instruction.mode);
			break;
		case 0x89:
			this->nop(address, instruction.mode);
			break;
		case 0x8a:
			this->txa(address, instruction.mode);
			break;
		case 0x8b:
			this->nop(address, instruction.mode);
			break;
		case 0x8c:
			this->sty(address, instruction.mode);
			break;
		case 0x8d:
			this->sta(address, instruction.mode);
			break;
		case 0x8e:
			this->stx(address, instruction.mode);
			break;
		case 0x8f:
			this->nop(address, instruction.mode);
			break;
		case 0x90:
			this->bcc(address, instruction.mode);
			break;
		case 0x91:
			this->sta(address, instruction.mode);
			break;
		case 0x92:
			this->nop(address, instruction.mode);
			break;
		case 0x93:
			this->nop(address, instruction.mode);
			break;
		case 0x94:
			this->sty(address, instruction.mode);
			break;
		case 0x95:
			this->sta(address, instruction.mode);
			break;
		case 0x96:
			this->stx(address, instruction.mode);
			break;
		case 0x97:
			this->nop(address, instruction.mode);
			break;
		case 0x98:
			this->tya(address, instruction.mode);
			break;
		case 0x99:
			this->sta(address, instruction.mode);
			break;
		case 0x9a:
			this->txs(address, instruction.mode);
			break;
		case 0x9b:
			this->nop(address, instruction.mode);
			break;
		case 0x9c:
			this->nop(address, instruction.mode);
			break;
		case 0x9d:
			this->sta(address, instruction.mode);
			break;
		case 0x9e:
			this->nop(address, instruction.mode);
			break;
		case 0x9f:
			this->nop(address, instruction.mode);
			break;
		case 0xa0:
			this->ldy(address, instruction.mode);
			break;
		case 0xa1:
			this->lda(address, instruction.mode);
			break;
		case 0xa2:
			this->ldx(address, instruction.mode);
			break;
		case 0xa3:
			this->nop(address, instruction.mode);
			break;
		case 0xa4:
			this->ldy(address, instruction.mode);
			break;
		case 0xa5:
			this->lda(address, instruction.mode);
			break;
		case 0xa6:
			this->ldx(address, instruction.mode);
			break;
		case 0xa7:
			this->nop(address, instruction.mode);
			break;
		case 0xa8:
			this->tay(address, instruction.mode);
			break;
		case 0xa9:
			this->lda(address, instruction.mode);
			break;
		case 0xaa:
			this->tax(address, instruction.mode);
			break;
		case 0xab:
			this->nop(address, instruction.mode);
			break;
		case 0xac:
			this->ldy(address, instruction.mode);
			break;
		case 0xad:
			this->lda(address, instruction.mode);
			break;
		case 0xae:
			this->ldx(address, instruction.mode);
			break;
		case 0xaf:
			this->nop(address, instruction.mode);
			break;
		case 0xb0:
			this->bcs(address, instruction.mode);
			break;
		case 0xb1:
			this->lda(address, instruction.mode);
			break;
		case 0xb2:
			this->nop(address, instruction.mode);
			break;
		case 0xb3:
			this->nop(address, instruction.mode);
			break;
		case 0xb4:
			this->ldy(address, instruction.mode);
			break;
		case 0xb5:
			this->lda(address, instruction.mode);
			break;
		case 0xb6:
			this->ldx(address, instruction.mode);
			break;
		case 0xb7:
			this->nop(address, instruction.mode);
			break;
		case 0xb8:
			this->clv(address, instruction.mode);
			break;
		case 0xb9:
			this->lda(address, instruction.mode);
			break;
		case 0xba:
			this->tsx(address, instruction.mode);
			break;
		case 0xbb:
			this->nop(address, instruction.mode);
			break;
		case 0xbc:
			this->ldy(address, instruction.mode);
			break;
		case 0xbd:
			this->lda(address, instruction.mode);
			break;
		case 0xbe:
			this->ldx(address, instruction.mode);
			break;
		case 0xbf:
			this->nop(address, instruction.mode);
			break;
		case 0xc0:
			this->cpy(address, instruction.mode);
			break;
		case 0xc1:
			this->cmp(address, instruction.mode);
			break;
		case 0xc2:
			this->nop(address, instruction.mode);
			break;
		case 0xc3:
			this->nop(address, instruction.mode);
			break;
		case 0xc4:
			this->cpy(address, instruction.mode);
			break;
		case 0xc5:
			this->cmp(address, instruction.mode);
			break;
		case 0xc6:
			this->dec(address, instruction.mode);
			break;
		case 0xc7:
			this->nop(address, instruction.mode);
			break;
		case 0xc8:
			this->iny(address, instruction.mode);
			break;
		case 0xc9:
			this->cmp(address, instruction.mode);
			break;
		case 0xca:
			this->dex(address, instruction.mode);
			break;
		case 0xcb:
			this->nop(address, instruction.mode);
			break;
		case 0xcc:
			this->cpy(address, instruction.mode);
			break;
		case 0xcd:
			this->cmp(address, instruction.mode);
			break;
		case 0xce:
			this->dec(address, instruction.mode);
			break;
		case 0xcf:
			this->nop(address, instruction.mode);
			break;
		case 0xd0:
			this->bne(address, instruction.mode);
			break;
		case 0xd1:
			this->cmp(address, instruction.mode);
			break;
		case 0xd2:
			this->nop(address, instruction.mode);
			break;
		case 0xd3:
			this->nop(address, instruction.mode);
			break;
		case 0xd4:
			this->nop(address, instruction.mode);
			break;
		case 0xd5:
			this->cmp(address, instruction.mode);
			break;
		case 0xd6:
			this->dec(address, instruction.mode);
			break;
		case 0xd7:
			this->nop(address, instruction.mode);
			break;
		case 0xd8:
			this->cld(address, instruction.mode);
			break;
		case 0xd9:
			this->cmp(address, instruction.mode);
			break;
		case 0xda:
			this->nop(address, instruction.mode);
			break;
		case 0xdb:
			this->nop(address, instruction.mode);
			break;
		case 0xdc:
			this->nop(address, instruction.mode);
			break;
		case 0xdd:
			this->cmp(address, instruction.mode);
			break;
		case 0xde:
			this->dec(address, instruction.mode);
			break;
		case 0xdf:
			this->nop(address, instruction.mode);
			break;
		case 0xe0:
			this->cpx(address, instruction.mode);
			break;
		case 0xe1:
			this->sbc(address, instruction.mode);
			break;
		case 0xe2:
			this->nop(address, instruction.mode);
			break;
		case 0xe3:
			this->nop(address, instruction.mode);
			break;
		case 0xe4:
			this->cpx(address, instruction.mode);
			break;
		case 0xe5:
			this->sbc(address, instruction.mode);
			break;
		case 0xe6:
			this->inc(address, instruction.mode);
			break;
		case 0xe7:
			this->nop(address, instruction.mode);
			break;
		case 0xe8:
			this->inx(address, instruction.mode);
			break;
		case 0xe9:
			this->sbc(address, instruction.mode);
			break;
		case 0xea:
			this->nop(address, instruction.mode);
			break;
		case 0xeb:
			this->sbc(address, instruction.mode);
			break;
		case 0xec:
			this->cpx(address, instruction.mode);
			break;
		case 0xed:
			this->sbc(address, instruction.mode);
			break;
		case 0xee:
			this->inc(address, instruction.mode);
			break;
		case 0xef:
			this->nop(address, instruction.mode);
			break;
		case 0xf0:
			this->beq(address, instruction.mode);
			break;
		case 0xf1:
			this->sbc(address, instruction.mode);
			break;
		case 0xf2:
			this->nop(address, instruction.mode);
			break;
		case 0xf3:
			this->nop(address, instruction.mode);
			break;
		case 0xf4:
			this->nop(address, instruction.mode);
			break;
		case 0xf5:
			this->sbc(address, instruction.mode);
			break;
		case 0xf6:
			this->inc(address, instruction.mode);
			break;
		case 0xf7:
			this->nop(address, instruction.mode);
			break;
		case 0xf8:
			this->sed(address, instruction.mode);
			break;
		case 0xf9:
			this->sbc(address, instruction.mode);
			break;
		case 0xfa:
			this->nop(address, instruction.mode);
			break;
		case 0xfb:
			this->nop(address, instruction.mode);
			break;
		case 0xfc:
			this->nop(address, instruction.mode);
			break;
		case 0xfd:
			this->sbc(address, instruction.mode);
			break;
		case 0xfe:
			this->inc(address, instruction.mode);
			break;
		case 0xff:
			this->nop(address, instruction.mode);
			break;
	    default:
	        break;
		}
}

template <class Mem>
void CPU<Mem>::setZ(byte value) {
	this->flags = (this->flags & (~(1 << 1))) | (static_cast<byte>(value == 0) << 1);
}

template <class Mem>
byte CPU<Mem>::getZ() {
	return (this->flags & 2) >> 1;
}

template <class Mem>
void CPU<Mem>::setN(byte value) {
	this->flags = (this->flags & (~(1 << 7))) | (value & 128);
}

template <class Mem>
byte CPU<Mem>::getN() {
	return (this->flags & 128) >> 7;
}

template <class Mem>
void CPU<Mem>::setZN(byte value) {
	setZ(value);
	setN(value);
}

template <class Mem>
void CPU<Mem>::setC(bool value) {
	this->flags = (this->flags & (~(1))) | static_cast<byte>(value);
}

template <class Mem>
byte CPU<Mem>::getC() {
	return this->flags & 1;
}

template <class Mem>
void CPU<Mem>::setI(bool value) {
	this->flags = (this->flags & (~(1 << 2))) | (static_cast<byte>(value) << 2);
}

template <class Mem>
inline byte CPU<Mem>::getI() {
	return (this->flags & 4) >> 2;
}

template <class Mem>
void CPU<Mem>::setD(bool value) {
	this->flags = (this->flags & (~(1 << 3))) | (static_cast<byte>(value) << 3);
}

template <class Mem>
byte CPU<Mem>::getD() {
	return (this->flags & 8) >> 3;
}

template <class Mem>
void CPU<Mem>::setB(bool value) {
	this->flags = (this->flags & (~(1 << 4))) | (static_cast<byte>(value) << 4);
}

template <class Mem>
byte CPU<Mem>::getB() {
	return (this->flags & 16) >> 4;
}

// set overflow flag if 'value' is true
template <class Mem>
void CPU<Mem>::setV(bool value) {
	this->flags = (this->flags & (~(1 << 6))) | (static_cast<byte>(value) << 6);
}

// get overflow flag
template <class Mem>
byte CPU<Mem>::getV() {
	return (this->flags & 64) >> 6;
}

template <class Mem>
void CPU<Mem>::compare(byte a, byte b) {
	setZN(a - b);
	setC(a >= b);
}

// 1 cycle for taking a branch
// 1 cycle if the branch is to a new page
template <class Mem>
void CPU<Mem>::branchDelay(uint16_t address, uint16_t pc) {
	++this->cycles;
	if (pagesDiffer(pc, address)) {
		++this->cycles;
	}
}

// ADC - ADd with Carry
template <class Mem>
void CPU<Mem>::adc(uint16_t address, byte mode) {
	static_cast<void>(mode);
	const byte a = this->a;
	const byte b = nes->readByte(address);
	const byte c = getC();
	this->a = a + b + c;
	setZN(this->a);
	setC(static_cast<int>(a) + static_cast<int>(b) + static_cast<int>(c) > 0xFF);
	setV(((a^b) & 0x80) == 0 && ((a^this->a) & 0x80) != 0);
}

// AND - logical AND
// Nonstandard name to disambiguate from 'and' label
template <class Mem>
void CPU<Mem>::and_instruction(uint16_t address, byte mode) {
	static_cast<void>(mode);
	this->a &= nes->readByte(address);
	setZN(this->a);
}

// ASL - Arithmetic Shift Left
template <class Mem>
void CPU<Mem>::asl(uint16_t address, byte mode) {
	if (mode == modeAccumulator) {
		setC((this->a >> 7) & 1);
		this->a <<= 1;
		setZN(this->a);
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
template <class Mem>
void CPU<Mem>::bit(uint16_t address, byte mode) {
	static_cast<void>(mode);
	const byte value = nes->readByte(address);
	setV((value >> 6) & 1);
	setZ(value & this->a);
	setN(value);
}

// CMP - CoMPare
template <class Mem>
void CPU<Mem>::cmp(uint16_t address, byte mode) {
	static_cast<void>(mode);
	const byte value = nes->readByte(address);
	compare(this->a, value);
}

// CPX - ComPare X register
template <class Mem>
void CPU<Mem>::cpx(uint16_t address, byte mode) {
	static_cast<void>(mode);
	const byte value = nes->readByte(address);
	compare(this->x, value);
}

// CPY - ComPare Y register
template <class Mem>
void CPU<Mem>::cpy(uint16_t address, byte mode) {
	static_cast<void>(mode);
	const byte value = nes->readByte(address);
	compare(this->y, value);
}

// DEC - DECrement memory
template <class Mem>
void CPU<Mem>::dec(uint16_t address, byte mode) {
	static_cast<void>(mode);
	const byte value = nes->readByte(address) - 1;
	nes->writeByte(address, value);
	setZN(value);
}


// EOR - Exclusive OR
template <class Mem>
void CPU<Mem>::eor(uint16_t address, byte mode) {
	static_cast<void>(mode);
	this->a ^= nes->readByte(address);
	setZN(this->a);
}

// INC - INCrement memory
template <class Mem>
void CPU<Mem>::inc(uint16_t address, byte mode) {
	static_cast<void>(mode);
	const byte value = nes->readByte(address) + 1;
	nes->writeByte(address, value);
	setZN(value);
}

// JMP - JuMP
template <class Mem>
void CPU<Mem>::jmp(uint16_t address, byte mode) {
	static_cast<void>(nes);
	static_cast<void>(mode);
	this->pc = address;
}

// LDA - LoaD Accumulator
template <class Mem>
void CPU<Mem>::lda(uint16_t address, byte mode) {
	static_cast<void>(mode);
	this->a = nes->readByte(address);
	setZN(this->a);
}

// LDX - LoaD X register
template <class Mem>
void CPU<Mem>::ldx(uint16_t address, byte mode) {
	static_cast<void>(mode);
	this->x = nes->readByte(address);
	setZN(this->x);
}

// LDY - LoaD Y register
template <class Mem>
void CPU<Mem>::ldy(uint16_t address, byte mode) {
	static_cast<void>(mode);
	this->y = nes->readByte(address);
	setZN(this->y);
}

// LSR - Logical Shift Right
template <class Mem>
void CPU<Mem>::lsr(uint16_t address, byte mode) {
	if (mode == modeAccumulator) {
		setC(this->a & 1);
		this->a >>= 1;
		setZN(this->a);
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
template <class Mem>
void CPU<Mem>::ora(uint16_t address, byte mode) {
	static_cast<void>(mode);
	this->a |= nes->readByte(address);
	setZN(this->a);
}

// PHP - PusH Processor status
template <class Mem>
void CPU<Mem>::php(uint16_t address, byte mode) {
	static_cast<void>(address);
	static_cast<void>(mode);
	nes->push(this->flags | 0x10);
}

// ROL - ROtate Left
template <class Mem>
void CPU<Mem>::rol(uint16_t address, byte mode) {
	if (mode == modeAccumulator) {
		const byte c = getC();
		setC((this->a >> 7) & 1);
		this->a = (this->a << 1) | c;
		setZN(this->a);
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
template <class Mem>
void CPU<Mem>::ror(uint16_t address, byte mode) {
	if (mode == modeAccumulator) {
		const byte c = getC();
		setC(this->a & 1);
		this->a = (this->a >> 1) | (c << 7);
		setZN(this->a);
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
template <class Mem>
void CPU<Mem>::sbc(uint16_t address, byte mode) {
	static_cast<void>(mode);
	const byte a = this->a;
	const byte b = nes->readByte(address);
	const byte c = getC();
	this->a = a - b - (1 - c);
	setZN(this->a);
	setC(static_cast<int>(a) - static_cast<int>(b) - static_cast<int>(1 - c) >= 0);
	setV(((a^b) & 0x80) != 0 && ((a^this->a) & 0x80) != 0);
}

// SEI - SEt Interrupt disable
template <class Mem>
void CPU<Mem>::sei(uint16_t address, byte mode) {
	static_cast<void>(nes);
	static_cast<void>(address);
	static_cast<void>(mode);
	setI(true);
}

// STA - STore Accumulator
template <class Mem>
void CPU<Mem>::sta(uint16_t address, byte mode) {
	static_cast<void>(mode);
	nes->writeByte(address, this->a);
}

// STX - Store X Register
template <class Mem>
void CPU<Mem>::stx(uint16_t address, byte mode) {
	static_cast<void>(mode);
	nes->writeByte(address, this->x);
}

// STY - STore Y Register
template <class Mem>
void CPU<Mem>::sty(uint16_t address, byte mode) {
	static_cast<void>(mode);
	nes->writeByte(address, this->y);
}

// BRK - force interrupt BReaK
template <class Mem>
void CPU<Mem>::brk(uint16_t address, byte mode) {
	nes->push16(this->pc);
	php(address, mode);
	sei(address, mode);
	this->pc = nes->read16(0xFFFE);
}

// BPL - Branch if PLus (i.e. if positive)
template <class Mem>
void CPU<Mem>::bpl(uint16_t address, byte mode) {
	static_cast<void>(nes);
	static_cast<void>(mode);
	if (getN() == 0) {
		const uint16_t pc = this->pc;
		this->pc = address;
		branchDelay(address, pc);
	}
}

// CLC - CLear Carry flag
template <class Mem>
void CPU<Mem>::clc(uint16_t address, byte mode) {
	static_cast<void>(nes);
	static_cast<void>(address);
	static_cast<void>(mode);
	setC(false);
}

// JSR - Jump to SubRoutine   
template <class Mem>
void CPU<Mem>::jsr(uint16_t address, byte mode) {
	static_cast<void>(mode);
	nes->push16(this->pc - 1);
	this->pc = address;
}

// PLP - PuLl Processor status
template <class Mem>
void CPU<Mem>::plp(uint16_t address, byte mode) {
	static_cast<void>(address);
	static_cast<void>(mode);
	this->flags = (nes->pop() & 0xEF) | 0x20;
}

// BMI - Branch if MInus (i.e. if negative)
template <class Mem>
void CPU<Mem>::bmi(uint16_t address, byte mode) {
	static_cast<void>(nes);
	static_cast<void>(mode);
	if (getN()) {
		const uint16_t pc = this->pc;
		this->pc = address;
		branchDelay(address, pc);
	}
}

// SEC - SEt Carry flag
template <class Mem>
void CPU<Mem>::sec(uint16_t address, byte mode) {
	static_cast<void>(nes);
	static_cast<void>(address);
	static_cast<void>(mode);
	setC(true);
}

// RTI - ReTurn from Interrupt
template <class Mem>
void CPU<Mem>::rti(uint16_t address, byte mode) {
	static_cast<void>(address);
	static_cast<void>(mode);
	this->flags = (nes->pop() & 0xEF) | 0x20;
	this->pc = nes->pop16();
}

// BVC - Branch if oVerflow Clear
template <class Mem>
void CPU<Mem>::bvc(uint16_t address, byte mode) {
	static_cast<void>(nes);
	static_cast<void>(mode);
	if (getV() == 0) {
		const uint16_t pc = this->pc;
		this->pc = address;
		branchDelay(address, pc);
	}
}

// PHA - PusH Accumulator
template <class Mem>
void CPU<Mem>::pha(uint16_t address, byte mode) {
	static_cast<void>(address);
	static_cast<void>(mode);
	nes->push(this->a);
}

// CLI - CLear Interrupt disable
template <class Mem>
void CPU<Mem>::cli(uint16_t address, byte mode) {
	static_cast<void>(nes);
	static_cast<void>(address);
	static_cast<void>(mode);
	setI(false);
}

// RTS - ReTurn from Subroutine
template <class Mem>
void CPU<Mem>::rts(uint16_t address, byte mode) {
	static_cast<void>(address);
	static_cast<void>(mode);
	this->pc = nes->pop16() + 1;
}

// PLA - PuLl Accumulator
template <class Mem>
void CPU<Mem>::pla(uint16_t address, byte mode) {
	static_cast<void>(address);
	static_cast<void>(mode);
	this->a = nes->pop();
	setZN(this->a);
}

// BVS - Branch if oVerflow Set
template <class Mem>
void CPU<Mem>::bvs(uint16_t address, byte mode) {
	static_cast<void>(nes);
	static_cast<void>(mode);
	if (getV()) {
		const uint16_t pc = this->pc;
		this->pc = address;
		branchDelay(address, pc);
	}
}

// DEY - DEcrement Y register
template <class Mem>
void CPU<Mem>::dey(uint16_t address, byte mode) {
	static_cast<void>(nes);
	static_cast<void>(address);
	static_cast<void>(mode);
	--this->y;
	setZN(this->y);
}

// TXA - Transfer X to Accumulator
template <class Mem>
void CPU<Mem>::txa(uint16_t address, byte mode) {
	static_cast<void>(nes);
	static_cast<void>(address);
	static_cast<void>(mode);
	this->a = this->x;
	setZN(this->a);
}

// BCC - Branch if Carry Clear
template <class Mem>
void CPU<Mem>::bcc(uint16_t address, byte mode) {
	static_cast<void>(nes);
	static_cast<void>(mode);
	if (getC() == 0) {
		const uint16_t pc = this->pc;
		this->pc = address;
		branchDelay(address, pc);
	}
}

// TYA - Transfer Y to Accumulator
template <class Mem>
void CPU<Mem>::tya(uint16_t address, byte mode) {
	static_cast<void>(nes);
	static_cast<void>(address);
	static_cast<void>(mode);
	this->a = this->y;
	setZN(this->a);
}

// BCS - Branch if Carry Set
template <class Mem>
void CPU<Mem>::bcs(uint16_t address, byte mode) {
	static_cast<void>(nes);
	static_cast<void>(mode);
	if (getC()) {
		const uint16_t pc = this->pc;
		this->pc = address;
		branchDelay(address, pc);
	}
}

// TAY - Transfer Accumulator to Y
template <class Mem>
void CPU<Mem>::tay(uint16_t address, byte mode) {
	static_cast<void>(nes);
	static_cast<void>(address);
	static_cast<void>(mode);
	this->y = this->a;
	setZN(this->y);
}

// TXS - Transfer X to Stack pointer
template <class Mem>
void CPU<Mem>::txs(uint16_t address, byte mode) {
	static_cast<void>(nes);
	static_cast<void>(address);
	static_cast<void>(mode);
	this->sp = this->x;
}

// TAX - Transfer Accumulator to X
template <class Mem>
void CPU<Mem>::tax(uint16_t address, byte mode) {
	static_cast<void>(nes);
	static_cast<void>(address);
	static_cast<void>(mode);
	this->x = this->a;
	setZN(this->x);
}

// CLV - CLear oVerflow flag
template <class Mem>
void CPU<Mem>::clv(uint16_t address, byte mode) {
	static_cast<void>(nes);
	static_cast<void>(address);
	static_cast<void>(mode);
	setV(false);
}

// TSX - Transfer Stack pointer to X
template <class Mem>
void CPU<Mem>::tsx(uint16_t address, byte mode) {
	static_cast<void>(nes);
	static_cast<void>(address);
	static_cast<void>(mode);
	this->x = this->sp;
	setZN(this->x);
}

// INY - INcrement Y register
template <class Mem>
void CPU<Mem>::iny(uint16_t address, byte mode) {
	static_cast<void>(nes);
	static_cast<void>(address);
	static_cast<void>(mode);
	++this->y;
	setZN(this->y);
}

// DEX - DEcrement X register
template <class Mem>
void CPU<Mem>::dex(uint16_t address, byte mode) {
	static_cast<void>(nes);
	static_cast<void>(address);
	static_cast<void>(mode);
	--this->x;
	setZN(this->x);
}

// BNE - Branch if Not Equal
template <class Mem>
void CPU<Mem>::bne(uint16_t address, byte mode) {
	static_cast<void>(nes);
	static_cast<void>(mode);
	if (getZ() == 0) {
		const uint16_t pc = this->pc;
		this->pc = address;
		branchDelay(address, pc);
	}
}

// CLD - CLear Decimal mode
template <class Mem>
void CPU<Mem>::cld(uint16_t address, byte mode) {
	static_cast<void>(nes);
	static_cast<void>(address);
	static_cast<void>(mode);
	setD(false);
}

// INX - INcrement X register
template <class Mem>
void CPU<Mem>::inx(uint16_t address, byte mode) {
	static_cast<void>(nes);
	static_cast<void>(address);
	static_cast<void>(mode);
	++this->x;
	setZN(this->x);
}

// BEQ - Branch if EQual
template <class Mem>
void CPU<Mem>::beq(uint16_t address, byte mode) {
	static_cast<void>(nes);
	static_cast<void>(mode);
	if (getZ()) {
		const uint16_t pc = this->pc;
		this->pc = address;
		branchDelay(address, pc);
	}
}

// SED - SEt Decimal flag
template <class Mem>
void CPU<Mem>::sed(uint16_t address, byte mode) {
	static_cast<void>(nes);
	static_cast<void>(address);
	static_cast<void>(mode);
	setD(true);
}

// NOP - No OPeration
template <class Mem>
void CPU<Mem>::nop(uint16_t address, byte mode) {
	static_cast<void>(this);
	static_cast<void>(nes);
	static_cast<void>(address);
	static_cast<void>(mode);
}

