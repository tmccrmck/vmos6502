#include "cpu.h"

// do addresses represent different pages?
bool CPU::pagesDiffer(uint16_t a, uint16_t b) {
	return (a & 0xFF00) != (b & 0xFF00);
}

void CPU::execute(byte opcode, NES* nes, uint16_t address, const Instruction instruction) {
    	switch(opcode){
		case 0x0:
			this->brk(nes, address, instruction.mode);
			break;
		case 0x1:
			this->ora(nes, address, instruction.mode);
			break;
		case 0x2:
			this->nop(nes, address, instruction.mode);
			break;
		case 0x3:
			this->nop(nes, address, instruction.mode);
			break;
		case 0x4:
			this->nop(nes, address, instruction.mode);
			break;
		case 0x5:
			this->ora(nes, address, instruction.mode);
			break;
		case 0x6:
			this->asl(nes, address, instruction.mode);
			break;
		case 0x7:
			this->nop(nes, address, instruction.mode);
			break;
		case 0x8:
			this->php(nes, address, instruction.mode);
			break;
		case 0x9:
			this->ora(nes, address, instruction.mode);
			break;
		case 0xa:
			this->asl(nes, address, instruction.mode);
			break;
		case 0xb:
			this->nop(nes, address, instruction.mode);
			break;
		case 0xc:
			this->nop(nes, address, instruction.mode);
			break;
		case 0xd:
			this->ora(nes, address, instruction.mode);
			break;
		case 0xe:
			this->asl(nes, address, instruction.mode);
			break;
		case 0xf:
			this->nop(nes, address, instruction.mode);
			break;
		case 0x10:
			this->bpl(nes, address, instruction.mode);
			break;
		case 0x11:
			this->ora(nes, address, instruction.mode);
			break;
		case 0x12:
			this->nop(nes, address, instruction.mode);
			break;
		case 0x13:
			this->nop(nes, address, instruction.mode);
			break;
		case 0x14:
			this->nop(nes, address, instruction.mode);
			break;
		case 0x15:
			this->ora(nes, address, instruction.mode);
			break;
		case 0x16:
			this->asl(nes, address, instruction.mode);
			break;
		case 0x17:
			this->nop(nes, address, instruction.mode);
			break;
		case 0x18:
			this->clc(nes, address, instruction.mode);
			break;
		case 0x19:
			this->ora(nes, address, instruction.mode);
			break;
		case 0x1a:
			this->nop(nes, address, instruction.mode);
			break;
		case 0x1b:
			this->nop(nes, address, instruction.mode);
			break;
		case 0x1c:
			this->nop(nes, address, instruction.mode);
			break;
		case 0x1d:
			this->ora(nes, address, instruction.mode);
			break;
		case 0x1e:
			this->asl(nes, address, instruction.mode);
			break;
		case 0x1f:
			this->nop(nes, address, instruction.mode);
			break;
		case 0x20:
			this->jsr(nes, address, instruction.mode);
			break;
		case 0x21:
			this->and_instruction(nes, address, instruction.mode);
			break;
		case 0x22:
			this->nop(nes, address, instruction.mode);
			break;
		case 0x23:
			this->nop(nes, address, instruction.mode);
			break;
		case 0x24:
			this->bit(nes, address, instruction.mode);
			break;
		case 0x25:
			this->and_instruction(nes, address, instruction.mode);
			break;
		case 0x26:
			this->rol(nes, address, instruction.mode);
			break;
		case 0x27:
			this->nop(nes, address, instruction.mode);
			break;
		case 0x28:
			this->plp(nes, address, instruction.mode);
			break;
		case 0x29:
			this->and_instruction(nes, address, instruction.mode);
			break;
		case 0x2a:
			this->rol(nes, address, instruction.mode);
			break;
		case 0x2b:
			this->nop(nes, address, instruction.mode);
			break;
		case 0x2c:
			this->bit(nes, address, instruction.mode);
			break;
		case 0x2d:
			this->and_instruction(nes, address, instruction.mode);
			break;
		case 0x2e:
			this->rol(nes, address, instruction.mode);
			break;
		case 0x2f:
			this->nop(nes, address, instruction.mode);
			break;
		case 0x30:
			this->bmi(nes, address, instruction.mode);
			break;
		case 0x31:
			this->and_instruction(nes, address, instruction.mode);
			break;
		case 0x32:
			this->and_instruction(nes, address, instruction.mode);
			break;
		case 0x33:
			this->nop(nes, address, instruction.mode);
			break;
		case 0x34:
			this->nop(nes, address, instruction.mode);
			break;
		case 0x35:
			this->nop(nes, address, instruction.mode);
			break;
		case 0x36:
			this->rol(nes, address, instruction.mode);
			break;
		case 0x37:
			this->nop(nes, address, instruction.mode);
			break;
		case 0x38:
			this->sec(nes, address, instruction.mode);
			break;
		case 0x39:
			this->and_instruction(nes, address, instruction.mode);
			break;
		case 0x3a:
			this->nop(nes, address, instruction.mode);
			break;
		case 0x3b:
			this->nop(nes, address, instruction.mode);
			break;
		case 0x3c:
			this->nop(nes, address, instruction.mode);
			break;
		case 0x3d:
			this->and_instruction(nes, address, instruction.mode);
			break;
		case 0x3e:
			this->rol(nes, address, instruction.mode);
			break;
		case 0x3f:
			this->nop(nes, address, instruction.mode);
			break;
		case 0x40:
			this->rti(nes, address, instruction.mode);
			break;
		case 0x41:
			this->eor(nes, address, instruction.mode);
			break;
		case 0x42:
			this->nop(nes, address, instruction.mode);
			break;
		case 0x43:
			this->nop(nes, address, instruction.mode);
			break;
		case 0x44:
			this->nop(nes, address, instruction.mode);
			break;
		case 0x45:
			this->eor(nes, address, instruction.mode);
			break;
		case 0x46:
			this->lsr(nes, address, instruction.mode);
			break;
		case 0x47:
			this->nop(nes, address, instruction.mode);
			break;
		case 0x48:
			this->pha(nes, address, instruction.mode);
			break;
		case 0x49:
			this->eor(nes, address, instruction.mode);
			break;
		case 0x4a:
			this->lsr(nes, address, instruction.mode);
			break;
		case 0x4b:
			this->nop(nes, address, instruction.mode);
			break;
		case 0x4c:
			this->jmp(nes, address, instruction.mode);
			break;
		case 0x4d:
			this->eor(nes, address, instruction.mode);
			break;
		case 0x4e:
			this->lsr(nes, address, instruction.mode);
			break;
		case 0x4f:
			this->nop(nes, address, instruction.mode);
			break;
		case 0x50:
			this->bvc(nes, address, instruction.mode);
			break;
		case 0x51:
			this->eor(nes, address, instruction.mode);
			break;
		case 0x52:
			this->nop(nes, address, instruction.mode);
			break;
		case 0x53:
			this->nop(nes, address, instruction.mode);
			break;
		case 0x54:
			this->nop(nes, address, instruction.mode);
			break;
		case 0x55:
			this->eor(nes, address, instruction.mode);
			break;
		case 0x56:
			this->lsr(nes, address, instruction.mode);
			break;
		case 0x57:
			this->nop(nes, address, instruction.mode);
			break;
		case 0x58:
			this->cli(nes, address, instruction.mode);
			break;
		case 0x59:
			this->eor(nes, address, instruction.mode);
			break;
		case 0x5a:
			this->nop(nes, address, instruction.mode);
			break;
		case 0x5b:
			this->nop(nes, address, instruction.mode);
			break;
		case 0x5c:
			this->nop(nes, address, instruction.mode);
			break;
		case 0x5d:
			this->eor(nes, address, instruction.mode);
			break;
		case 0x5e:
			this->lsr(nes, address, instruction.mode);
			break;
		case 0x5f:
			this->nop(nes, address, instruction.mode);
			break;
		case 0x60:
			this->rts(nes, address, instruction.mode);
			break;
		case 0x61:
			this->adc(nes, address, instruction.mode);
			break;
		case 0x62:
			this->nop(nes, address, instruction.mode);
			break;
		case 0x63:
			this->nop(nes, address, instruction.mode);
			break;
		case 0x64:
			this->nop(nes, address, instruction.mode);
			break;
		case 0x65:
			this->adc(nes, address, instruction.mode);
			break;
		case 0x66:
			this->ror(nes, address, instruction.mode);
			break;
		case 0x67:
			this->nop(nes, address, instruction.mode);
			break;
		case 0x68:
			this->pla(nes, address, instruction.mode);
			break;
		case 0x69:
			this->adc(nes, address, instruction.mode);
			break;
		case 0x6a:
			this->ror(nes, address, instruction.mode);
			break;
		case 0x6b:
			this->nop(nes, address, instruction.mode);
			break;
		case 0x6c:
			this->jmp(nes, address, instruction.mode);
			break;
		case 0x6d:
			this->adc(nes, address, instruction.mode);
			break;
		case 0x6e:
			this->ror(nes, address, instruction.mode);
			break;
		case 0x6f:
			this->nop(nes, address, instruction.mode);
			break;
		case 0x70:
			this->bvs(nes, address, instruction.mode);
			break;
		case 0x71:
			this->adc(nes, address, instruction.mode);
			break;
		case 0x72:
			this->nop(nes, address, instruction.mode);
			break;
		case 0x73:
			this->nop(nes, address, instruction.mode);
			break;
		case 0x74:
			this->nop(nes, address, instruction.mode);
			break;
		case 0x75:
			this->adc(nes, address, instruction.mode);
			break;
		case 0x76:
			this->ror(nes, address, instruction.mode);
			break;
		case 0x77:
			this->nop(nes, address, instruction.mode);
			break;
		case 0x78:
			this->sei(nes, address, instruction.mode);
			break;
		case 0x79:
			this->adc(nes, address, instruction.mode);
			break;
		case 0x7a:
			this->nop(nes, address, instruction.mode);
			break;
		case 0x7b:
			this->nop(nes, address, instruction.mode);
			break;
		case 0x7c:
			this->nop(nes, address, instruction.mode);
			break;
		case 0x7d:
			this->adc(nes, address, instruction.mode);
			break;
		case 0x7e:
			this->ror(nes, address, instruction.mode);
			break;
		case 0x7f:
			this->nop(nes, address, instruction.mode);
			break;
		case 0x80:
			this->nop(nes, address, instruction.mode);
			break;
		case 0x81:
			this->sta(nes, address, instruction.mode);
			break;
		case 0x82:
			this->nop(nes, address, instruction.mode);
			break;
		case 0x83:
			this->nop(nes, address, instruction.mode);
			break;
		case 0x84:
			this->sty(nes, address, instruction.mode);
			break;
		case 0x85:
			this->sta(nes, address, instruction.mode);
			break;
		case 0x86:
			this->stx(nes, address, instruction.mode);
			break;
		case 0x87:
			this->nop(nes, address, instruction.mode);
			break;
		case 0x88:
			this->dey(nes, address, instruction.mode);
			break;
		case 0x89:
			this->nop(nes, address, instruction.mode);
			break;
		case 0x8a:
			this->txa(nes, address, instruction.mode);
			break;
		case 0x8b:
			this->nop(nes, address, instruction.mode);
			break;
		case 0x8c:
			this->sty(nes, address, instruction.mode);
			break;
		case 0x8d:
			this->sta(nes, address, instruction.mode);
			break;
		case 0x8e:
			this->stx(nes, address, instruction.mode);
			break;
		case 0x8f:
			this->nop(nes, address, instruction.mode);
			break;
		case 0x90:
			this->bcc(nes, address, instruction.mode);
			break;
		case 0x91:
			this->sta(nes, address, instruction.mode);
			break;
		case 0x92:
			this->nop(nes, address, instruction.mode);
			break;
		case 0x93:
			this->nop(nes, address, instruction.mode);
			break;
		case 0x94:
			this->sty(nes, address, instruction.mode);
			break;
		case 0x95:
			this->sta(nes, address, instruction.mode);
			break;
		case 0x96:
			this->stx(nes, address, instruction.mode);
			break;
		case 0x97:
			this->nop(nes, address, instruction.mode);
			break;
		case 0x98:
			this->tya(nes, address, instruction.mode);
			break;
		case 0x99:
			this->sta(nes, address, instruction.mode);
			break;
		case 0x9a:
			this->txs(nes, address, instruction.mode);
			break;
		case 0x9b:
			this->nop(nes, address, instruction.mode);
			break;
		case 0x9c:
			this->nop(nes, address, instruction.mode);
			break;
		case 0x9d:
			this->sta(nes, address, instruction.mode);
			break;
		case 0x9e:
			this->nop(nes, address, instruction.mode);
			break;
		case 0x9f:
			this->nop(nes, address, instruction.mode);
			break;
		case 0xa0:
			this->ldy(nes, address, instruction.mode);
			break;
		case 0xa1:
			this->lda(nes, address, instruction.mode);
			break;
		case 0xa2:
			this->ldx(nes, address, instruction.mode);
			break;
		case 0xa3:
			this->nop(nes, address, instruction.mode);
			break;
		case 0xa4:
			this->ldy(nes, address, instruction.mode);
			break;
		case 0xa5:
			this->lda(nes, address, instruction.mode);
			break;
		case 0xa6:
			this->ldx(nes, address, instruction.mode);
			break;
		case 0xa7:
			this->nop(nes, address, instruction.mode);
			break;
		case 0xa8:
			this->tay(nes, address, instruction.mode);
			break;
		case 0xa9:
			this->lda(nes, address, instruction.mode);
			break;
		case 0xaa:
			this->tax(nes, address, instruction.mode);
			break;
		case 0xab:
			this->nop(nes, address, instruction.mode);
			break;
		case 0xac:
			this->ldy(nes, address, instruction.mode);
			break;
		case 0xad:
			this->lda(nes, address, instruction.mode);
			break;
		case 0xae:
			this->ldx(nes, address, instruction.mode);
			break;
		case 0xaf:
			this->nop(nes, address, instruction.mode);
			break;
		case 0xb0:
			this->bcs(nes, address, instruction.mode);
			break;
		case 0xb1:
			this->lda(nes, address, instruction.mode);
			break;
		case 0xb2:
			this->nop(nes, address, instruction.mode);
			break;
		case 0xb3:
			this->nop(nes, address, instruction.mode);
			break;
		case 0xb4:
			this->ldy(nes, address, instruction.mode);
			break;
		case 0xb5:
			this->lda(nes, address, instruction.mode);
			break;
		case 0xb6:
			this->ldx(nes, address, instruction.mode);
			break;
		case 0xb7:
			this->nop(nes, address, instruction.mode);
			break;
		case 0xb8:
			this->clv(nes, address, instruction.mode);
			break;
		case 0xb9:
			this->lda(nes, address, instruction.mode);
			break;
		case 0xba:
			this->tsx(nes, address, instruction.mode);
			break;
		case 0xbb:
			this->nop(nes, address, instruction.mode);
			break;
		case 0xbc:
			this->ldy(nes, address, instruction.mode);
			break;
		case 0xbd:
			this->lda(nes, address, instruction.mode);
			break;
		case 0xbe:
			this->ldx(nes, address, instruction.mode);
			break;
		case 0xbf:
			this->nop(nes, address, instruction.mode);
			break;
		case 0xc0:
			this->cpy(nes, address, instruction.mode);
			break;
		case 0xc1:
			this->cmp(nes, address, instruction.mode);
			break;
		case 0xc2:
			this->nop(nes, address, instruction.mode);
			break;
		case 0xc3:
			this->nop(nes, address, instruction.mode);
			break;
		case 0xc4:
			this->cpy(nes, address, instruction.mode);
			break;
		case 0xc5:
			this->cmp(nes, address, instruction.mode);
			break;
		case 0xc6:
			this->dec(nes, address, instruction.mode);
			break;
		case 0xc7:
			this->nop(nes, address, instruction.mode);
			break;
		case 0xc8:
			this->iny(nes, address, instruction.mode);
			break;
		case 0xc9:
			this->cmp(nes, address, instruction.mode);
			break;
		case 0xca:
			this->dex(nes, address, instruction.mode);
			break;
		case 0xcb:
			this->nop(nes, address, instruction.mode);
			break;
		case 0xcc:
			this->cpy(nes, address, instruction.mode);
			break;
		case 0xcd:
			this->cmp(nes, address, instruction.mode);
			break;
		case 0xce:
			this->dec(nes, address, instruction.mode);
			break;
		case 0xcf:
			this->nop(nes, address, instruction.mode);
			break;
		case 0xd0:
			this->bne(nes, address, instruction.mode);
			break;
		case 0xd1:
			this->cmp(nes, address, instruction.mode);
			break;
		case 0xd2:
			this->nop(nes, address, instruction.mode);
			break;
		case 0xd3:
			this->nop(nes, address, instruction.mode);
			break;
		case 0xd4:
			this->nop(nes, address, instruction.mode);
			break;
		case 0xd5:
			this->cmp(nes, address, instruction.mode);
			break;
		case 0xd6:
			this->dec(nes, address, instruction.mode);
			break;
		case 0xd7:
			this->nop(nes, address, instruction.mode);
			break;
		case 0xd8:
			this->cld(nes, address, instruction.mode);
			break;
		case 0xd9:
			this->cmp(nes, address, instruction.mode);
			break;
		case 0xda:
			this->nop(nes, address, instruction.mode);
			break;
		case 0xdb:
			this->nop(nes, address, instruction.mode);
			break;
		case 0xdc:
			this->nop(nes, address, instruction.mode);
			break;
		case 0xdd:
			this->cmp(nes, address, instruction.mode);
			break;
		case 0xde:
			this->dec(nes, address, instruction.mode);
			break;
		case 0xdf:
			this->nop(nes, address, instruction.mode);
			break;
		case 0xe0:
			this->cpx(nes, address, instruction.mode);
			break;
		case 0xe1:
			this->sbc(nes, address, instruction.mode);
			break;
		case 0xe2:
			this->nop(nes, address, instruction.mode);
			break;
		case 0xe3:
			this->nop(nes, address, instruction.mode);
			break;
		case 0xe4:
			this->cpx(nes, address, instruction.mode);
			break;
		case 0xe5:
			this->sbc(nes, address, instruction.mode);
			break;
		case 0xe6:
			this->inc(nes, address, instruction.mode);
			break;
		case 0xe7:
			this->nop(nes, address, instruction.mode);
			break;
		case 0xe8:
			this->inx(nes, address, instruction.mode);
			break;
		case 0xe9:
			this->sbc(nes, address, instruction.mode);
			break;
		case 0xea:
			this->nop(nes, address, instruction.mode);
			break;
		case 0xeb:
			this->sbc(nes, address, instruction.mode);
			break;
		case 0xec:
			this->cpx(nes, address, instruction.mode);
			break;
		case 0xed:
			this->sbc(nes, address, instruction.mode);
			break;
		case 0xee:
			this->inc(nes, address, instruction.mode);
			break;
		case 0xef:
			this->nop(nes, address, instruction.mode);
			break;
		case 0xf0:
			this->beq(nes, address, instruction.mode);
			break;
		case 0xf1:
			this->sbc(nes, address, instruction.mode);
			break;
		case 0xf2:
			this->nop(nes, address, instruction.mode);
			break;
		case 0xf3:
			this->nop(nes, address, instruction.mode);
			break;
		case 0xf4:
			this->nop(nes, address, instruction.mode);
			break;
		case 0xf5:
			this->sbc(nes, address, instruction.mode);
			break;
		case 0xf6:
			this->inc(nes, address, instruction.mode);
			break;
		case 0xf7:
			this->nop(nes, address, instruction.mode);
			break;
		case 0xf8:
			this->sed(nes, address, instruction.mode);
			break;
		case 0xf9:
			this->sbc(nes, address, instruction.mode);
			break;
		case 0xfa:
			this->nop(nes, address, instruction.mode);
			break;
		case 0xfb:
			this->nop(nes, address, instruction.mode);
			break;
		case 0xfc:
			this->nop(nes, address, instruction.mode);
			break;
		case 0xfd:
			this->sbc(nes, address, instruction.mode);
			break;
		case 0xfe:
			this->inc(nes, address, instruction.mode);
			break;
		case 0xff:
			this->nop(nes, address, instruction.mode);
			break;
	    default:
	        break;
		}
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
	static_cast<void>(this);
	static_cast<void>(nes);
	static_cast<void>(address);
	static_cast<void>(mode);
}
