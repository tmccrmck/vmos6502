#include <cstdint>
#include <array>

class Memory {
public:
    Memory();
	virtual ~Memory();

    virtual uint8_t loadb(uint16_t addr) = 0;

    virtual void storeb(uint16_t addr, uint8_t val) = 0;

    void storew(uint16_t addr) {
        storeb(addr, uint8_t(addr & 0xff));
        storeb(static_cast<uint16_t>(addr + 1), uint8_t((addr >> 8) & 0xff));
    }

    uint16_t loadw(uint16_t addr) {
        return loadb(addr) | uint16_t(loadb(static_cast<uint16_t>(addr + 1))) << 8;
    }
};

class Ppu : public Memory {
public:
    uint8_t loadb(uint16_t addr) {
        return 1;
    }

    void storeb(uint16_t addr, uint8_t val) {
    }
};

class Apu : public Memory {
public:
    uint8_t loadb(uint16_t addr) {
        return 1;
    }

    void storeb(uint16_t addr, uint8_t val) {
    }
};


class Ram : public Memory {
public:
    std::array<uint16_t, 2048> val;

	uint8_t loadb(uint16_t addr) {
		return 1;
	}

    void storeb(uint16_t addr, uint8_t val) {
    }
};


class Memmap: public Memory {
public:
    Memmap(const Ram &ram, const Ppu &ppu, const Apu &apu) : ram(ram), ppu(ppu), apu(apu) {}
    Ram ram;
    Ppu ppu;
    Apu apu;

    uint8_t loadb(uint16_t addr) {
        if(addr < 0x2000 ){
            return ram.loadb(addr);
        } else if( addr < 0x4000 ){
            return ppu.loadb(addr);
        } else if( addr == 0x4016) {
            return input.loadb(addr);
        } else if( addr <= 0x4018) {
            return apu.loadb(addr);
        } else if( addr < 0x6000 ){
            return 0;   // FIXME: I think some mappers use regs in this area?
        } else {
            mapper.prg_loadb(addr);
        }
    }

    void storeb(uint16_t addr, uint8_t val) {
         if (addr < 0x2000 ){
            return ram.storeb(addr, val);
        } else if (addr < 0x4000 ){
            return ppu.storeb(addr, val);
        } else if (addr == 0x4016) {
            return input.storeb(addr, val);
        } else if (addr <= 0x4018) {
            return apu.storeb(addr, val);
        } else if (addr < 0x6000 ){
            return; // TODO?
        } else {
            mapper.prg_storeb(addr, val);
        } 
    }
};

