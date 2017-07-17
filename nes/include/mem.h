class Mem {
public:
    uint8_t loadb(uint16_t addr);
    void storeb(uint16_t addr, uint8_t val);

    void storew(uint16_t addr) {
        storeb(addr, uint8_t(addr & 0xff));
        storeb(addr + 1, uint8_t((addr >> 8) & 0xff));
    }

    uint16_t loadw(uint16_t addr) {
        return loadb(addr) | uint16_t(loadb(addr + 1)) << 8;
    }
};

class Memmap: public Mem {
public:
	uint8_t loadb(uint16_t addr) {
		// TODO
		return 0;
	}
	void storeb(uint16_t self, uint8_t val) { return; /* TODO */ }
};