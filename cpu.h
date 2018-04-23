#ifndef VMOS6502_CPU_H
#define VMOS6502_CPU_H

#include <cstdint>
#include <cstdio>
#include <cstdlib>

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
};

#endif //VMOS6502_CPU_H
