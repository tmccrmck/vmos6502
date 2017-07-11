#include "stdint.h"

typedef uint64_t Cycles;

class memmap {
public:
	uint8_t loadb(uint16_t addr);
};

class Cpu {
public:
    Cpu();
	uint8_t loadb(uint16_t addr);
	void storeb(uint16_t addr, uint8_t val);
	uint8_t loadb_bump_pc();

    Cycles cycles;
    uint16_t PC;       // program counter
    uint8_t SP;        // stack pointer
    uint8_t A;         // accumulator
    uint8_t X;         // X register
    uint8_t Y;         // Y register
	memmap mem;
    uint8_t flags;     // flags register
    uint8_t interrupt; // interrupt type
    int stall;
};

