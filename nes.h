#ifndef VMOS6502_NES_H
#define VMOS6502_NES_H

#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>

#include "cartridge.h"
#include "controller.h"
#include "mapper.h"
#include "apu.h"
#include "ppu.h"

constexpr int INES_MAGIC = 0x1a53454e;
constexpr double CPU_FREQ = 1789773.0;
constexpr double FRAME_CTR_FREQ = CPU_FREQ / 240.0;
constexpr double SAMPLE_RATE = CPU_FREQ / (44100.0);

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

enum Interrupts {
	interruptNone = 1,
	interruptNMI = 2,
	interruptIRQ = 3
};

enum AddressingModes {
	modeAbsolute = 1,
	modeAbsoluteX = 2,
	modeAbsoluteY = 3,
	modeAccumulator = 4,
	modeImmediate = 5,
	modeImplied = 6,
	modeIndexedIndirect = 7,
	modeIndirect = 8,
	modeIndirectIndexed = 9,
	modeRelative = 10,
	modeZeroPage = 11,
	modeZeroPageX = 12,
	modeZeroPageY = 13
};

bool pagesDiffer(uint16_t a, uint16_t b);
uint16_t mirrorAddress(byte mode, uint16_t address);

class NES {
public:
	bool initialized;
	CPU* cpu;
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

    void tickAPU(APU* apu);
	void tickPPU(CPU* cpu, PPU* ppu);
    void writePPU(uint16_t address, byte value);
    void writeRegisterPPU(uint16_t address, byte value);
    byte readPPU(uint16_t address);
	byte readPPURegister(uint16_t address);

    byte readByte(uint16_t address);
    void push16(uint16_t value);
    void push(byte value);
    byte pop();
    uint16_t  pop16();
    uint16_t read16(uint16_t address);
    uint16_t read16_ff_bug(uint16_t address);
    void writeByte(uint16_t address, byte value);

    void printState() {
		std::cout << "CPU status: "
			<< "PC=" << cpu->PC <<  std::endl;
		std::cout << "APU status: "
			<< "DM=" << apu->dmc.enabled << " P1=" << apu->pulse1.enabled << " P2=" << apu->pulse2.enabled << " TR=" << apu->triangle.enabled<< " NO=" << apu->noise.enabled << std::endl;
		std::cout << "PPU status: "
			<< "BG=" << unsigned(ppu->flag_show_background) << " BL=" << unsigned(ppu->flag_show_left_background) << " SP=" << unsigned(ppu->flag_show_sprites) << " SL="<< unsigned(ppu->flag_show_left_sprites) << std::endl;
		
	}
};

constexpr byte duty_tbl[4][8] = {
	{ 0, 1, 0, 0, 0, 0, 0, 0 },
	{ 0, 1, 1, 0, 0, 0, 0, 0 },
	{ 0, 1, 1, 1, 1, 0, 0, 0 },
	{ 1, 0, 0, 1, 1, 1, 1, 1 },
};

void php(CPU* cpu, NES* nes, uint16_t address, byte mode);

void setI(CPU* cpu, bool value);
byte getI(CPU* cpu);

#endif