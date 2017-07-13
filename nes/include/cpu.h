#include "mem.h"

typedef uint64_t Cycles;

class Cpu {
public:
    Cpu();
	void step();
	uint8_t loadb(uint16_t addr);
    uint16_t loadw(uint16_t addr);
	void storeb(uint16_t addr, uint8_t val);
    void storew(uint16_t addr, uint16_t val);
	uint8_t loadb_bump_pc();
    uint16_t loadw_bump_pc();
	void pushb(uint8_t val);
    void pushw(uint16_t val);
    uint8_t popb();
    uint16_t popw();
    bool get_flag(uint8_t flag);
    void set_flag(uint8_t flag, bool on);


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

