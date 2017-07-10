#include "stdint.h"

class cpu {
public:
    cpu();

    cpu(uint16_t PC);

    uint64_t cycles;
    uint16_t PC;       // program counter
    uint8_t SP;        // stack pointer
    uint8_t A;         // accumulator
    uint8_t X;         // X register
    uint8_t Y;         // Y register
    uint8_t flags;     // flags register
    uint8_t interrupt; // interrupt type
    int stall;
};

