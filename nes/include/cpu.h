#include "mem.h"

class Cpu;
typedef uint64_t Cycles;

class acc_addressing_mode {
public:
    uint8_t load(Cpu cpu);
    void store(Cpu cpu, uint8_t val);
};

class imm_addressing_mode {
public:
    uint8_t load(Cpu cpu);
};

class mem_addressing_mode {
public:
    uint16_t mem;

    explicit mem_addressing_mode(uint16_t _mem) : mem(_mem) {}
    uint8_t load(Cpu cpu);
    void store(Cpu cpu, uint8_t val);
};

template <class Mem>
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
    imm_addressing_mode immediate();
    acc_addressing_mode accumulator();
    mem_addressing_mode zero_page();
	mem_addressing_mode absolute();

	template <typename mode_t> void sta(mode_t am);
	template <typename mode_t> void stx(mode_t am);
	template <typename mode_t> void sty(mode_t am);


    Cycles cycles;
    uint16_t PC;       // program counter
    uint8_t SP;        // stack pointer
    uint8_t A;         // accumulator
    uint8_t X;         // X register
    uint8_t Y;         // Y register
	Mem mem;
    uint8_t flags;     // flags register
    uint8_t interrupt; // interrupt type
    int stall;
};

