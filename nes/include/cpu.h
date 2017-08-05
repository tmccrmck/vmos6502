#include "mem.h"

template <class Mem> class Cpu;
typedef uint64_t Cycles;

template <class Mem>
class AddressingMode {
public:
    virtual uint8_t load(Cpu<Mem>& cpu) = 0;
    virtual void store(Cpu<Mem>& cpu, uint8_t val) = 0;
};

template <class Mem>
class AccAddressingMode: public AddressingMode<Mem> {
public:
    uint8_t load(Cpu<Mem>& cpu);
    void store(Cpu<Mem>& cpu, uint8_t val);
};

template <class Mem>
class ImmAddressingMode: public AddressingMode<Mem> {
public:
    uint8_t load(Cpu<Mem>& cpu);
};

template <class Mem>
class MemoryAddressingMode: public AddressingMode<Mem> {
public:
    uint16_t cur_val{};

    explicit MemoryAddressingMode(uint16_t val) : cur_val(val) {}
    uint8_t load(Cpu<Mem>& cpu);
    void store(Cpu<Mem>& cpu, uint8_t val);
};

template <class Mem>
class Cpu: public Memory {
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
    uint8_t set_zn(uint8_t val);

    ImmAddressingMode<Mem> immediate();
    AccAddressingMode<Mem> accumulator();
    MemoryAddressingMode<Mem> zero_page();
	MemoryAddressingMode<Mem> absolute();

    void lda(AddressingMode<Mem> am);
    void ldx(AddressingMode<Mem> am);
    void ldy(AddressingMode<Mem> am);
	void sta(AddressingMode<Mem> am);
	void stx(AddressingMode<Mem> am);
	void sty(AddressingMode<Mem> am);

	void adc(AddressingMode<Mem> am);
	void sbc(AddressingMode<Mem> am);

	void cmp_base(uint8_t reg, AddressingMode<Mem> am);
	void cmp(AddressingMode<Mem> am);
	void cmpx(AddressingMode<Mem> am);
	void cmpy(AddressingMode<Mem> am);

    void anda(AddressingMode<Mem> am);
    void ora(AddressingMode<Mem> am);
    void xora(AddressingMode<Mem> am);
	void bit(AddressingMode<Mem> am);

    void shl_base(bool lsb, AddressingMode<Mem> am);
    void shr_base(bool msb, AddressingMode<Mem> am);
	void rol(AddressingMode<Mem> am);
	void ror(AddressingMode<Mem> am);
	void asl(AddressingMode<Mem> am);
	void lsr(AddressingMode<Mem> am);

    void inc(AddressingMode<Mem> am);
    void dec(AddressingMode<Mem> am);

    void inx() { X = set_zn(static_cast<uint8_t>(X + 1)); }
    void dex() { X = set_zn(static_cast<uint8_t>(X - 1)); }
    void iny() { Y = set_zn(static_cast<uint8_t>(Y + 1)); }
    void dey() { Y = set_zn(static_cast<uint8_t>(Y - 1)); }

    // Register moves
    void tax() { X = set_zn(A); }
    void tay() { Y = set_zn(A); }
    void txa() { A = set_zn(X); }
    void tya() { A = set_zn(Y); }
    void txs() { SP = X; }
    void tsx() { X = set_zn(static_cast<uint8_t>(PC)); }

    void clc();
    void sec();
    void cli();
    void sei();
    void clv();
    void cld();
    void sed();

    // Branches
    void bra_base(bool cond);
    void bpl();
    void bmi();
    void bvc();
    void bvs();
    void bcc();
    void bcs();
    void bne();
    void beq();

	void jmp();
	void jmpi();

    void jsr(); 
    void rts(); 
    void brk(); 
    void rti(); 
    void pha(); 
    void pla(); 
    void php(); 
    void plp(); 
    void nop(); 

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

