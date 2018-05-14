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

class PPU;
class Mapper;
//template <class Mem> class CPU;

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
	bool initialized;
	CPU<NES>* cpu;
	APU* apu;
	PPU* ppu;
	Cartridge* cartridge;
	Controller* controller1;
	Controller* controller2;
	Mapper* mapper;
	byte* RAM;

	NES(std::string path, std::string SRAM_path);
    void emulate(double seconds);
    void execute(byte opcode);

    byte readByte(uint16_t address);
    void push16(uint16_t value);
    void push(byte value);
    byte pop();
    uint16_t  pop16();
    uint16_t read16(uint16_t address);
    uint16_t read16_ff_bug(uint16_t address);
    void writeByte(uint16_t address, byte value);
};

#endif