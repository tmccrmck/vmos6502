#include <stdint.h>
#include <array>

class Memory {
public:
    Memory();
	virtual ~Memory();

    virtual uint8_t loadb(uint16_t addr) = 0;

    virtual void storeb(uint16_t addr, uint8_t val) = 0;

    void storew(uint16_t addr) {
        storeb(addr, uint8_t(addr & 0xff));
        storeb(addr + 1, uint8_t((addr >> 8) & 0xff));
    }

    uint16_t loadw(uint16_t addr) {
        return loadb(addr) | uint16_t(loadb(addr + 1)) << 8;
    }
};

class Ram : public Memory {
public:
    std::array<uint16_t, 2048> val;

	uint8_t loadb(uint16_t addr) {
		return 1;
	}

    void storeb(uint16_t addr, uint8_t val) {
		return;
    }
};


class Memmap: public Memory {
public:
    Memmap(const Ram &ram) : ram(ram) {}

    uint8_t loadb(uint16_t addr) {
        // TODO
        return 0;
    }

    void storeb(uint16_t self, uint8_t val) { return; /* TODO */ }
    Ram ram;
};

