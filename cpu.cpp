#include "cpu.h"

// These methods are meant to abstract away NES class
byte CPU::readb(uint16_t addr) {
    return nes->readByte(addr);
}

void CPU::writeb(uint16_t addr, byte value) {
    nes->writeByte(addr, value);
}


// do addresses represent different pages?
bool CPU::pagesDiffer(uint16_t a, uint16_t b) {
    return (a & 0xFF00) != (b & 0xFF00);
}


void CPU::executeOpcode(const byte opcode, const uint16_t address, const Instruction& instruction) {
    switch (opcode) {
        case 0x0:
            this->brk();
            break;
        case 0x1:
            this->ora(address);
            break;
        case 0x2:
            this->nop();
            break;
        case 0x3:
            this->nop();
            break;
        case 0x4:
            this->nop();
            break;
        case 0x5:
            this->ora(address);
            break;
        case 0x6:
            this->asl(address, instruction.mode);
            break;
        case 0x7:
            this->nop();
            break;
        case 0x8:
            this->php();
            break;
        case 0x9:
            this->ora(address);
            break;
        case 0xa:
            this->asl(address, instruction.mode);
            break;
        case 0xb:
            this->nop();
            break;
        case 0xc:
            this->nop();
            break;
        case 0xd:
            this->ora(address);
            break;
        case 0xe:
            this->asl(address, instruction.mode);
            break;
        case 0xf:
            this->nop();
            break;
        case 0x10:
            this->bpl(address);
            break;
        case 0x11:
            this->ora(address);
            break;
        case 0x12:
            this->nop();
            break;
        case 0x13:
            this->nop();
            break;
        case 0x14:
            this->nop();
            break;
        case 0x15:
            this->ora(address);
            break;
        case 0x16:
            this->asl(address, instruction.mode);
            break;
        case 0x17:
            this->nop();
            break;
        case 0x18:
            this->clc();
            break;
        case 0x19:
            this->ora(address);
            break;
        case 0x1a:
            this->nop();
            break;
        case 0x1b:
            this->nop();
            break;
        case 0x1c:
            this->nop();
            break;
        case 0x1d:
            this->ora(address);
            break;
        case 0x1e:
            this->asl(address, instruction.mode);
            break;
        case 0x1f:
            this->nop();
            break;
        case 0x20:
            this->jsr(address);
            break;
        case 0x21:
            this->and_instruction(address);
            break;
        case 0x22:
            this->nop();
            break;
        case 0x23:
            this->nop();
            break;
        case 0x24:
            this->bit(address);
            break;
        case 0x25:
            this->and_instruction(address);
            break;
        case 0x26:
            this->rol(address, instruction.mode);
            break;
        case 0x27:
            this->nop();
            break;
        case 0x28:
            this->plp();
            break;
        case 0x29:
            this->and_instruction(address);
            break;
        case 0x2a:
            this->rol(address, instruction.mode);
            break;
        case 0x2b:
            this->nop();
            break;
        case 0x2c:
            this->bit(address);
            break;
        case 0x2d:
            this->and_instruction(address);
            break;
        case 0x2e:
            this->rol(address, instruction.mode);
            break;
        case 0x2f:
            this->nop();
            break;
        case 0x30:
            this->bmi(address);
            break;
        case 0x31:
            this->and_instruction(address);
            break;
        case 0x32:
            this->and_instruction(address);
            break;
        case 0x33:
            this->nop();
            break;
        case 0x34:
            this->nop();
            break;
        case 0x35:
            this->nop();
            break;
        case 0x36:
            this->rol(address, instruction.mode);
            break;
        case 0x37:
            this->nop();
            break;
        case 0x38:
            this->sec();
            break;
        case 0x39:
            this->and_instruction(address);
            break;
        case 0x3a:
            this->nop();
            break;
        case 0x3b:
            this->nop();
            break;
        case 0x3c:
            this->nop();
            break;
        case 0x3d:
            this->and_instruction(address);
            break;
        case 0x3e:
            this->rol(address, instruction.mode);
            break;
        case 0x3f:
            this->nop();
            break;
        case 0x40:
            this->rti();
            break;
        case 0x41:
            this->eor(address);
            break;
        case 0x42:
            this->nop();
            break;
        case 0x43:
            this->nop();
            break;
        case 0x44:
            this->nop();
            break;
        case 0x45:
            this->eor(address);
            break;
        case 0x46:
            this->lsr(address, instruction.mode);
            break;
        case 0x47:
            this->nop();
            break;
        case 0x48:
            this->pha();
            break;
        case 0x49:
            this->eor(address);
            break;
        case 0x4a:
            this->lsr(address, instruction.mode);
            break;
        case 0x4b:
            this->nop();
            break;
        case 0x4c:
            this->jmp(address);
            break;
        case 0x4d:
            this->eor(address);
            break;
        case 0x4e:
            this->lsr(address, instruction.mode);
            break;
        case 0x4f:
            this->nop();
            break;
        case 0x50:
            this->bvc(address);
            break;
        case 0x51:
            this->eor(address);
            break;
        case 0x52:
            this->nop();
            break;
        case 0x53:
            this->nop();
            break;
        case 0x54:
            this->nop();
            break;
        case 0x55:
            this->eor(address);
            break;
        case 0x56:
            this->lsr(address, instruction.mode);
            break;
        case 0x57:
            this->nop();
            break;
        case 0x58:
            this->cli();
            break;
        case 0x59:
            this->eor(address);
            break;
        case 0x5a:
            this->nop();
            break;
        case 0x5b:
            this->nop();
            break;
        case 0x5c:
            this->nop();
            break;
        case 0x5d:
            this->eor(address);
            break;
        case 0x5e:
            this->lsr(address, instruction.mode);
            break;
        case 0x5f:
            this->nop();
            break;
        case 0x60:
            this->rts();
            break;
        case 0x61:
            this->adc(address);
            break;
        case 0x62:
            this->nop();
            break;
        case 0x63:
            this->nop();
            break;
        case 0x64:
            this->nop();
            break;
        case 0x65:
            this->adc(address);
            break;
        case 0x66:
            this->ror(address, instruction.mode);
            break;
        case 0x67:
            this->nop();
            break;
        case 0x68:
            this->pla();
            break;
        case 0x69:
            this->adc(address);
            break;
        case 0x6a:
            this->ror(address, instruction.mode);
            break;
        case 0x6b:
            this->nop();
            break;
        case 0x6c:
            this->jmp(address);
            break;
        case 0x6d:
            this->adc(address);
            break;
        case 0x6e:
            this->ror(address, instruction.mode);
            break;
        case 0x6f:
            this->nop();
            break;
        case 0x70:
            this->bvs(address);
            break;
        case 0x71:
            this->adc(address);
            break;
        case 0x72:
            this->nop();
            break;
        case 0x73:
            this->nop();
            break;
        case 0x74:
            this->nop();
            break;
        case 0x75:
            this->adc(address);
            break;
        case 0x76:
            this->ror(address, instruction.mode);
            break;
        case 0x77:
            this->nop();
            break;
        case 0x78:
            this->sei();
            break;
        case 0x79:
            this->adc(address);
            break;
        case 0x7a:
            this->nop();
            break;
        case 0x7b:
            this->nop();
            break;
        case 0x7c:
            this->nop();
            break;
        case 0x7d:
            this->adc(address);
            break;
        case 0x7e:
            this->ror(address, instruction.mode);
            break;
        case 0x7f:
            this->nop();
            break;
        case 0x80:
            this->nop();
            break;
        case 0x81:
            this->sta(address);
            break;
        case 0x82:
            this->nop();
            break;
        case 0x83:
            this->nop();
            break;
        case 0x84:
            this->sty(address);
            break;
        case 0x85:
            this->sta(address);
            break;
        case 0x86:
            this->stx(address);
            break;
        case 0x87:
            this->nop();
            break;
        case 0x88:
            this->dey();
            break;
        case 0x89:
            this->nop();
            break;
        case 0x8a:
            this->txa();
            break;
        case 0x8b:
            this->nop();
            break;
        case 0x8c:
            this->sty(address);
            break;
        case 0x8d:
            this->sta(address);
            break;
        case 0x8e:
            this->stx(address);
            break;
        case 0x8f:
            this->nop();
            break;
        case 0x90:
            this->bcc(address);
            break;
        case 0x91:
            this->sta(address);
            break;
        case 0x92:
            this->nop();
            break;
        case 0x93:
            this->nop();
            break;
        case 0x94:
            this->sty(address);
            break;
        case 0x95:
            this->sta(address);
            break;
        case 0x96:
            this->stx(address);
            break;
        case 0x97:
            this->nop();
            break;
        case 0x98:
            this->tya();
            break;
        case 0x99:
            this->sta(address);
            break;
        case 0x9a:
            this->txs();
            break;
        case 0x9b:
            this->nop();
            break;
        case 0x9c:
            this->nop();
            break;
        case 0x9d:
            this->sta(address);
            break;
        case 0x9e:
            this->nop();
            break;
        case 0x9f:
            this->nop();
            break;
        case 0xa0:
            this->ldy(address);
            break;
        case 0xa1:
            this->lda(address);
            break;
        case 0xa2:
            this->ldx(address);
            break;
        case 0xa3:
            this->nop();
            break;
        case 0xa4:
            this->ldy(address);
            break;
        case 0xa5:
            this->lda(address);
            break;
        case 0xa6:
            this->ldx(address);
            break;
        case 0xa7:
            this->nop();
            break;
        case 0xa8:
            this->tay();
            break;
        case 0xa9:
            this->lda(address);
            break;
        case 0xaa:
            this->tax();
            break;
        case 0xab:
            this->nop();
            break;
        case 0xac:
            this->ldy(address);
            break;
        case 0xad:
            this->lda(address);
            break;
        case 0xae:
            this->ldx(address);
            break;
        case 0xaf:
            this->nop();
            break;
        case 0xb0:
            this->bcs(address);
            break;
        case 0xb1:
            this->lda(address);
            break;
        case 0xb2:
            this->nop();
            break;
        case 0xb3:
            this->nop();
            break;
        case 0xb4:
            this->ldy(address);
            break;
        case 0xb5:
            this->lda(address);
            break;
        case 0xb6:
            this->ldx(address);
            break;
        case 0xb7:
            this->nop();
            break;
        case 0xb8:
            this->clv();
            break;
        case 0xb9:
            this->lda(address);
            break;
        case 0xba:
            this->tsx();
            break;
        case 0xbb:
            this->nop();
            break;
        case 0xbc:
            this->ldy(address);
            break;
        case 0xbd:
            this->lda(address);
            break;
        case 0xbe:
            this->ldx(address);
            break;
        case 0xbf:
            this->nop();
            break;
        case 0xc0:
            this->cpy(address);
            break;
        case 0xc1:
            this->cmp(address);
            break;
        case 0xc2:
            this->nop();
            break;
        case 0xc3:
            this->nop();
            break;
        case 0xc4:
            this->cpy(address);
            break;
        case 0xc5:
            this->cmp(address);
            break;
        case 0xc6:
            this->dec(address);
            break;
        case 0xc7:
            this->nop();
            break;
        case 0xc8:
            this->iny();
            break;
        case 0xc9:
            this->cmp(address);
            break;
        case 0xca:
            this->dex();
            break;
        case 0xcb:
            this->nop();
            break;
        case 0xcc:
            this->cpy(address);
            break;
        case 0xcd:
            this->cmp(address);
            break;
        case 0xce:
            this->dec(address);
            break;
        case 0xcf:
            this->nop();
            break;
        case 0xd0:
            this->bne(address);
            break;
        case 0xd1:
            this->cmp(address);
            break;
        case 0xd2:
            this->nop();
            break;
        case 0xd3:
            this->nop();
            break;
        case 0xd4:
            this->nop();
            break;
        case 0xd5:
            this->cmp(address);
            break;
        case 0xd6:
            this->dec(address);
            break;
        case 0xd7:
            this->nop();
            break;
        case 0xd8:
            this->cld();
            break;
        case 0xd9:
            this->cmp(address);
            break;
        case 0xda:
            this->nop();
            break;
        case 0xdb:
            this->nop();
            break;
        case 0xdc:
            this->nop();
            break;
        case 0xdd:
            this->cmp(address);
            break;
        case 0xde:
            this->dec(address);
            break;
        case 0xdf:
            this->nop();
            break;
        case 0xe0:
            this->cpx(address);
            break;
        case 0xe1:
            this->sbc(address);
            break;
        case 0xe2:
            this->nop();
            break;
        case 0xe3:
            this->nop();
            break;
        case 0xe4:
            this->cpx(address);
            break;
        case 0xe5:
            this->sbc(address);
            break;
        case 0xe6:
            this->inc(address);
            break;
        case 0xe7:
            this->nop();
            break;
        case 0xe8:
            this->inx();
            break;
        case 0xe9:
            this->sbc(address);
            break;
        case 0xea:
            this->nop();
            break;
        case 0xeb:
            this->sbc(address);
            break;
        case 0xec:
            this->cpx(address);
            break;
        case 0xed:
            this->sbc(address);
            break;
        case 0xee:
            this->inc(address);
            break;
        case 0xef:
            this->nop();
            break;
        case 0xf0:
            this->beq(address);
            break;
        case 0xf1:
            this->sbc(address);
            break;
        case 0xf2:
            this->nop();
            break;
        case 0xf3:
            this->nop();
            break;
        case 0xf4:
            this->nop();
            break;
        case 0xf5:
            this->sbc(address);
            break;
        case 0xf6:
            this->inc(address);
            break;
        case 0xf7:
            this->nop();
            break;
        case 0xf8:
            this->sed();
            break;
        case 0xf9:
            this->sbc(address);
            break;
        case 0xfa:
            this->nop();
            break;
        case 0xfb:
            this->nop();
            break;
        case 0xfc:
            this->nop();
            break;
        case 0xfd:
            this->sbc(address);
            break;
        case 0xfe:
            this->inc(address);
            break;
        case 0xff:
            this->nop();
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

void CPU::adc(uint16_t address) {
    const byte a = this->a;
    const byte b = readb(address);
    const byte c = getC();
    this->a = a + b + c;
    setZN(this->a);
    setC(static_cast<int>(a) + static_cast<int>(b) + static_cast<int>(c) > 0xFF);
    setV(((a ^ b) & 0x80) == 0 && ((a ^ this->a) & 0x80) != 0);
}

// AND - logical AND
// Nonstandard name to disambiguate from 'and' label

void CPU::and_instruction(uint16_t address) {
    this->a &= readb(address);
    setZN(this->a);
}

// ASL - Arithmetic Shift Left

void CPU::asl(uint16_t address, byte mode) {
    if (mode == modeAccumulator) {
        setC((this->a >> 7) & 1);
        this->a <<= 1;
        setZN(this->a);
    } else {
        byte value = readb(address);
        setC((value >> 7) & 1);
        value <<= 1;
        writeb(address, value);
        setZN(value);
    }
}

// BIT - BIt Test

void CPU::bit(uint16_t address) {
    const byte value = readb(address);
    setV((value >> 6) & 1);
    setZ(value & this->a);
    setN(value);
}

// CMP - CoMPare

void CPU::cmp(uint16_t address) {
    const byte value = readb(address);
    compare(this->a, value);
}

// CPX - ComPare X register

void CPU::cpx(uint16_t address) {
    const byte value = readb(address);
    compare(this->x, value);
}

// CPY - ComPare Y register

void CPU::cpy(uint16_t address) {
    const byte value = readb(address);
    compare(this->y, value);
}

// DEC - DECrement memory

void CPU::dec(uint16_t address) {
    const byte value = readb(address) - 1;
    writeb(address, value);
    setZN(value);
}


// EOR - Exclusive OR

void CPU::eor(uint16_t address) {
    this->a ^= readb(address);
    setZN(this->a);
}

// INC - INCrement memory

void CPU::inc(uint16_t address) {
    const byte value = readb(address) + 1;
    writeb(address, value);
    setZN(value);
}

// JMP - JuMP

void CPU::jmp(uint16_t address) {
    this->pc = address;
}

// LDA - LoaD Accumulator

void CPU::lda(uint16_t address) {
    this->a = readb(address);
    setZN(this->a);
}

// LDX - LoaD X register

void CPU::ldx(uint16_t address) {
    this->x = readb(address);
    setZN(this->x);
}

// LDY - LoaD Y register

void CPU::ldy(uint16_t address) {
    this->y = readb(address);
    setZN(this->y);
}

// LSR - Logical Shift Right

void CPU::lsr(uint16_t address, byte mode) {
    if (mode == modeAccumulator) {
        setC(this->a & 1);
        this->a >>= 1;
        setZN(this->a);
    } else {
        byte value = readb(address);
        setC(value & 1);
        value >>= 1;
        writeb(address, value);
        setZN(value);
    }
}

// ORA - logical OR with Accumulator

void CPU::ora(uint16_t address) {
    this->a |= readb(address);
    setZN(this->a);
}

// PHP - PusH Processor status

void CPU::php() {
    nes->push(this->flags | 0x10);
}

// ROL - ROtate Left

void CPU::rol(uint16_t address, byte mode) {
    if (mode == modeAccumulator) {
        const byte c = getC();
        setC((this->a >> 7) & 1);
        this->a = (this->a << 1) | c;
        setZN(this->a);
    } else {
        const byte c = getC();
        byte value = readb(address);
        setC((value >> 7) & 1);
        value = (value << 1) | c;
        writeb(address, value);
        setZN(value);
    }
}

// ROR - ROtate Right

void CPU::ror(uint16_t address, byte mode) {
    if (mode == modeAccumulator) {
        const byte c = getC();
        setC(this->a & 1);
        this->a = (this->a >> 1) | (c << 7);
        setZN(this->a);
    } else {
        const byte c = getC();
        byte value = readb(address);
        setC(value & 1);
        value = (value >> 1) | (c << 7);
        writeb(address, value);
        setZN(value);
    }
}

// SBC - SuBtract with Carry

void CPU::sbc(uint16_t address) {
    const byte a = this->a;
    const byte b = readb(address);
    const byte c = getC();
    this->a = a - b - (1 - c);
    setZN(this->a);
    setC(static_cast<int>(a) - static_cast<int>(b) - static_cast<int>(1 - c) >= 0);
    setV(((a ^ b) & 0x80) != 0 && ((a ^ this->a) & 0x80) != 0);
}

// SEI - SEt Interrupt disable

void CPU::sei() {
    setI(true);
}

// STA - STore Accumulator

void CPU::sta(uint16_t address) {
    writeb(address, this->a);
}

// STX - Store X Register

void CPU::stx(uint16_t address) {
    writeb(address, this->x);
}

// STY - STore Y Register

void CPU::sty(uint16_t address) {
    writeb(address, this->y);
}

// BRK - force interrupt BReaK

void CPU::brk() {
    nes->push16(this->pc);
    php();
    sei();
    this->pc = nes->read16(0xFFFE);
}

// BPL - Branch if PLus (i.e. if positive)

void CPU::bpl(uint16_t address) {
    if (getN() == 0) {
        const uint16_t pc = this->pc;
        this->pc = address;
        branchDelay(address, pc);
    }
}

// CLC - CLear Carry flag

void CPU::clc() {
    setC(false);
}

// JSR - Jump to SubRoutine   

void CPU::jsr(uint16_t address) {
    nes->push16(this->pc - 1);
    this->pc = address;
}

// PLP - PuLl Processor status

void CPU::plp() {
    this->flags = (nes->pop() & 0xEF) | 0x20;
}

// BMI - Branch if MInus (i.e. if negative)

void CPU::bmi(uint16_t address) {
    if (getN()) {
        const uint16_t pc = this->pc;
        this->pc = address;
        branchDelay(address, pc);
    }
}

// SEC - SEt Carry flag

void CPU::sec() {
    setC(true);
}

// RTI - ReTurn from Interrupt

void CPU::rti() {
    this->flags = (nes->pop() & 0xEF) | 0x20;
    this->pc = nes->pop16();
}

// BVC - Branch if oVerflow Clear

void CPU::bvc(uint16_t address) {
    if (getV() == 0) {
        const uint16_t pc = this->pc;
        this->pc = address;
        branchDelay(address, pc);
    }
}

// PHA - PusH Accumulator

void CPU::pha() {
    nes->push(this->a);
}

// CLI - CLear Interrupt disable

void CPU::cli() {
    setI(false);
}

// RTS - ReTurn from Subroutine

void CPU::rts() {
    this->pc = nes->pop16() + 1;
}

// PLA - PuLl Accumulator

void CPU::pla() {
    this->a = nes->pop();
    setZN(this->a);
}

// BVS - Branch if oVerflow Set

void CPU::bvs(uint16_t address) {
    if (getV()) {
        const uint16_t pc = this->pc;
        this->pc = address;
        branchDelay(address, pc);
    }
}

// DEY - DEcrement Y register

void CPU::dey() {
    --this->y;
    setZN(this->y);
}

// TXA - Transfer X to Accumulator

void CPU::txa() {
    this->a = this->x;
    setZN(this->a);
}

// BCC - Branch if Carry Clear

void CPU::bcc(uint16_t address) {
    if (getC() == 0) {
        const uint16_t pc = this->pc;
        this->pc = address;
        branchDelay(address, pc);
    }
}

// TYA - Transfer Y to Accumulator

void CPU::tya() {
    this->a = this->y;
    setZN(this->a);
}

// BCS - Branch if Carry Set

void CPU::bcs(uint16_t address) {
    if (getC()) {
        const uint16_t pc = this->pc;
        this->pc = address;
        branchDelay(address, pc);
    }
}

// TAY - Transfer Accumulator to Y

void CPU::tay() {
    this->y = this->a;
    setZN(this->y);
}

// TXS - Transfer X to Stack pointer

void CPU::txs() {
    this->sp = this->x;
}

// TAX - Transfer Accumulator to X

void CPU::tax() {
    this->x = this->a;
    setZN(this->x);
}

// CLV - CLear oVerflow flag

void CPU::clv() {
    setV(false);
}

// TSX - Transfer Stack pointer to X

void CPU::tsx() {
    this->x = this->sp;
    setZN(this->x);
}

// INY - INcrement Y register

void CPU::iny() {
    ++this->y;
    setZN(this->y);
}

// DEX - DEcrement X register

void CPU::dex() {
    --this->x;
    setZN(this->x);
}

// BNE - Branch if Not Equal

void CPU::bne(uint16_t address) {
    if (getZ() == 0) {
        const uint16_t pc = this->pc;
        this->pc = address;
        branchDelay(address, pc);
    }
}

// CLD - CLear Decimal mode

void CPU::cld() {
    setD(false);
}

// INX - INcrement X register

void CPU::inx() {
    ++this->x;
    setZN(this->x);
}

// BEQ - Branch if EQual

void CPU::beq(uint16_t address) {
    if (getZ()) {
        const uint16_t pc = this->pc;
        this->pc = address;
        branchDelay(address, pc);
    }
}

// SED - SEt Decimal flag

void CPU::sed() {
    setD(true);
}

// NOP - No OPeration

void CPU::nop() {
}

void CPU::triggerIRQ() {
    if (this->getI() == 0) {
        this->interrupt = interruptIRQ;
    }
}

