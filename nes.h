#ifndef VMOS6502_NES_H
#define VMOS6502_NES_H

#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>

#include "cartridge.h"
#include "mapper.h"
#include "cpu.h"
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

struct iNESHeader {
	uint32_t magic;     // iNES cartridge file magic number
	byte num_prg;    // number of 16k PRG-ROM banks
	byte num_chr;    // number of 8k CHR-ROM banks
	byte ctrl1;
	byte ctrl2;
	byte num_ram;    // number of 8k PRG-RAM banks
	byte padding[7];
};

struct Controller {
	byte buttons;
	byte index;
	byte strobe;

	Controller() : buttons(0), index(0), strobe(0) {}
};

byte readController(Controller* c);
void writeController(Controller* c, byte value);

bool pagesDiffer(uint16_t a, uint16_t b);
void writeRegisterAPU(APU* apu, uint16_t address, byte value);
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

struct Instruction {
	const byte opcode;
	const char* name;
	void(*dispatch)(CPU*, NES*, uint16_t, byte);
	const byte mode;
	const byte size;
	const byte cycles;
	const byte page_cross_cycles;

	constexpr Instruction(const byte _opcode, const char _name[4], void(*_dispatch)(CPU*, NES*, uint16_t, byte), const byte _mode, const byte _size, const byte _cycles, const byte _page_crossed_cycles) : opcode(_opcode), name(_name), dispatch(_dispatch), mode(_mode), size(_size), cycles(_cycles), page_cross_cycles(_page_crossed_cycles) {}
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