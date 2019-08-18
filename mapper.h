#include <utility>

#ifndef VMOS6502_MAPPER_H
#define VMOS6502_MAPPER_H

#include <iostream>
#include <memory>

#include "cartridge.h"

class NES;

enum MirrorModes {
    MirrorHorizontal = 0,
    MirrorVertical = 1,
    MirrorSingle0 = 2,
    MirrorSingle1 = 3,
    MirrorFour = 4
};

class Mapper {
public:
    virtual uint8_t read(uint16_t address) = 0;

    virtual void write(uint16_t address, uint8_t value) = 0;

    virtual ~Mapper() = default;

    std::shared_ptr<Cartridge> cartridge;
};

class Mapper1 : public Mapper {
public:
    explicit Mapper1(std::shared_ptr<Cartridge> cartridge1, uint8_t _shift) : cartridge(std::move(cartridge1)),
                                                                              shift_reg(_shift), control(0),
                                                                              prg_mode(0), chr_mode(0),
                                                                              prg_bank(0), chr_bank0(0), chr_bank1(0),
                                                                              prg_offsets{prgBankOffset(-1), 0},
                                                                              chr_offsets{0, 0} {}

    uint8_t read(uint16_t address);
    void write(uint16_t address, uint8_t value);
    std::shared_ptr<Cartridge> cartridge;

private:
    uint8_t shift_reg;
    uint8_t control;
    uint8_t prg_mode;
    uint8_t chr_mode;
    uint8_t prg_bank;
    uint8_t chr_bank0;
    uint8_t chr_bank1;
    int prg_offsets[2];
    int chr_offsets[2];

    int prgBankOffset(int index);
    int chrBankOffset(int index);
    void updateOffsets();
    void writeCtrl(uint8_t value);

};

class Mapper2 : public Mapper {
public:
    Mapper2(std::shared_ptr<Cartridge> cartridge1, int _prgBanks, int _prgBank1, int _prgBank2) : cartridge(
            std::move(cartridge1)), prg_banks(_prgBanks), prg_bank1(_prgBank1), prg_bank2(_prgBank2) {}

    std::shared_ptr<Cartridge> cartridge;
    uint8_t read(uint16_t address);
    void write(uint16_t address, uint8_t value);

private:
    int prg_banks;
    int prg_bank1;
    int prg_bank2;
};

class Mapper3 : public Mapper {
public:
    Mapper3(std::shared_ptr<Cartridge> cartridge1, int _chrBank, int _prgBank1, int _prgBank2) : cartridge(
            std::move(cartridge1)),
                                                                                                 chr_bank(_chrBank),
                                                                                                 prg_bank1(_prgBank1),
                                                                                                 prg_bank2(_prgBank2) {}

    std::shared_ptr<Cartridge> cartridge;
    uint8_t read(uint16_t address);
    void write(uint16_t address, uint8_t value);

private:
    int chr_bank;
    int prg_bank1;
    int prg_bank2;
};

class Mapper4 : public Mapper {
public:
    explicit Mapper4(std::shared_ptr<Cartridge> cartridge1) : cartridge(cartridge1), reg(0),
                                                              regs{0, 0, 0, 0, 0, 0, 0, 0}, prg_mode(0), chr_mode(0),
                                                              prg_offsets{prgBankOffset(0), prgBankOffset(1),
                                                                          prgBankOffset(-2), prgBankOffset(-1)},
                                                              chr_offsets{0, 0, 0, 0, 0, 0, 0, 0}, reload(0),
                                                              counter(0), IRQ_enable(false) {};

    std::shared_ptr<Cartridge> cartridge;
    uint8_t read(uint16_t address);
    void write(uint16_t address, uint8_t value);

private:
    uint8_t reg;
    uint8_t regs[8];
    uint8_t prg_mode;
    uint8_t chr_mode;
    int prg_offsets[4];
    int chr_offsets[8];
    uint8_t reload;
    uint8_t counter;
    bool IRQ_enable;

    int prgBankOffset(int index);
    int chrBankOffset(int index);
    void updateOffsets();

};

class Mapper7 : public Mapper {
public:
    explicit Mapper7(std::shared_ptr<Cartridge> cartridge1) : cartridge(cartridge1), prg_bank(0) {}

    std::shared_ptr<Cartridge> cartridge;
    uint8_t read(uint16_t address) override;
    void write(uint16_t address, uint8_t value) override;

private:
    int prg_bank;
};

#endif //VMOS6502_MAPPER_H

