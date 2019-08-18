#ifndef VMOS6502_NES_H
#define VMOS6502_NES_H

#include <iostream>
#include <cstring>

#include "cartridge.h"
#include "controller.h"
#include "mapper.h"
#include "apu.h"
#include "ppu.h"
#include "cpu.h"

class APU;

class PPU;

class Mapper;

enum Buttons {
    ButtonA = 0,
    ButtonB = 1,
    ButtonSelect = 2,
    ButtonStart = 3,
    ButtonUp = 4,
    ButtonDown = 5,
    ButtonLeft = 6,
    ButtonRight = 7
};

class NES {
public:
    NES(std::string path, std::string SRAM_path);

    std::unique_ptr<CPU> cpu;
    std::unique_ptr<APU> apu;
    std::unique_ptr<PPU> ppu;
    std::unique_ptr<Controller> controller1;
    std::unique_ptr<Controller> controller2;
    std::unique_ptr<Mapper> mapper;
    std::array<byte, 2048> RAM = {0};

    void emulate(double seconds);
    byte readByte(uint16_t address);
    void writeByte(uint16_t address, byte value);
    uint16_t read16(uint16_t address);
    void push(byte value);
    void push16(uint16_t value);
    byte pop();
    uint16_t pop16();
private:
    void execute(byte opcode);
    uint16_t read16_ff_bug(uint16_t address);
    std::unique_ptr<Mapper> setMapper(const std::shared_ptr<Cartridge> &cartridge);
};

#endif