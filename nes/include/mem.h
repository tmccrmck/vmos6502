class Mem {
public:
    Mem();

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

class Ram : public Mem{
public:
    std::array<uint8_t, 0x800> val;

    void storeb(uint16_t addr, uint8_t val) {
        Mem::storeb(addr, val);
    }
};


class Memmap: public Mem {
public:
    Memmap(const Ram &ram) : ram(ram) {}

    uint8_t loadb(uint16_t addr) {
        // TODO
        return 0;
    }

    void storeb(uint16_t self, uint8_t val) { return; /* TODO */ }
private:
    Ram ram;
};

