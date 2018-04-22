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
	uint8_t num_prg;    // number of 16k PRG-ROM banks
	uint8_t num_chr;    // number of 8k CHR-ROM banks
	uint8_t ctrl1;
	uint8_t ctrl2;
	uint8_t num_ram;    // number of 8k PRG-RAM banks
	uint8_t padding[7];
};

struct PPU {
	int cycle; // 0-340
	int scanline; // 0-261. 0-239 is visible, 240 is postline, 241-260 is the v_blank interval, 261 is preline
	uint64_t frame;

	uint8_t palette_tbl[32];
	uint8_t name_tbl[2048];
	uint8_t oam_tbl[256];

	uint32_t* front;
	uint32_t* back;

	// regs
	uint16_t v; // vram address
	uint16_t t; // temp vram address
	uint8_t x;  // fine x scroll
	uint8_t w;  // write flag
	uint8_t f;  // even/odd flag

	uint8_t reg;

	bool nmi_occurred;
	bool nmi_out;
	bool nmi_last;
	uint8_t nmi_delay;

	uint8_t name_tbl_u8;
	uint8_t attrib_tbl_u8;
	uint8_t low_tile_u8;
	uint8_t high_tile_u8;
	uint64_t tile_data;

	int sprite_cnt;
	uint32_t sprite_patterns[8];
	uint8_t sprite_pos[8];
	uint8_t sprite_priorities[8];
	uint8_t sprite_idx[8];

	// $2000 PPUCTRL
	uint8_t flag_name_tbl;       // 0: $2000.  1: $2400. 2: $2800. 3: $2C00
	uint8_t flag_increment;      // 0: add 1.  1: add 32
	uint8_t flag_sprite_tbl;     // 0: $0000.  1: $1000
	uint8_t flag_background_tbl; // 0: $0000.  1: $1000
	uint8_t flag_sprite_size;    // 0: 8x8.    1: 8x16
	uint8_t flag_rw;             // 0: read.   1: write

	// $2001 PPUMASK
	uint8_t flag_gray;                 // 0: color.   1: grayscale
	uint8_t flag_show_left_background; // 0: hide.    1: show
	uint8_t flag_show_left_sprites;    // 0: hide.    1: show
	uint8_t flag_show_background;      // 0: hide.    1: show
	uint8_t flag_show_sprites;         // 0: hide.    1: show
	uint8_t flag_red_tint;             // 0: normal.  1: emphasized
	uint8_t flag_green_tint;           // 0: normal.  1: emphasized
	uint8_t flag_blue_tint;            // 0: normal.  1: emphasized

	// $2002 PPUSTATUS
	uint8_t flag_sprite_zero_hit;
	uint8_t flag_sprite_overflow;

	// $2003 OAMADDR
	uint8_t oam_addr;

	// $2007 PPUDATA
	uint8_t buffered_data;

	PPU() : cycle(0), scanline(0), frame(0), v(0), t(0), x(0), w(0), f(0), reg(0), nmi_occurred(false), nmi_out(false), nmi_last(false),
		nmi_delay(0), name_tbl_u8(0), attrib_tbl_u8(0), low_tile_u8(0), high_tile_u8(0), tile_data(0), sprite_cnt(0), flag_name_tbl(0), flag_increment(0),
		flag_sprite_tbl(0), flag_background_tbl(0), flag_sprite_size(0), flag_rw(0), flag_gray(0), flag_show_left_background(0), flag_show_left_sprites(0),
		flag_show_background(0), flag_show_sprites(0), flag_red_tint(0), flag_green_tint(0), flag_blue_tint(0), flag_sprite_zero_hit(0), flag_sprite_overflow(0),
		oam_addr(0), buffered_data(0)
	{
		memset(palette_tbl, 0, 32);
		memset(name_tbl, 0, 2048);
		memset(oam_tbl, 0, 256);
		memset(sprite_patterns, 0, 32);
		memset(sprite_pos, 0, 8);
		memset(sprite_priorities, 0, 8);
		memset(sprite_idx, 0, 8);
	}
};

void writePPUCtrl(PPU* ppu, uint8_t x);
void writePPUMask(PPU* ppu, uint8_t x);

struct Controller {
	uint8_t buttons;
	uint8_t index;
	uint8_t strobe;

	Controller() : buttons(0), index(0), strobe(0) {}
};

struct NES {
	bool initialized;
	CPU* cpu;
	APU* apu;
	PPU* ppu;
	Cartridge* cartridge;
	Controller* controller1;
	Controller* controller2;
	Mapper* mapper;
	uint8_t* RAM;

	NES(const std::string path, const std::string SRAM_path);

    void printState() {
        printf("\rSTATUS CPU PC=%hu APU DM=%hhu P1=%hhu P2=%hhu TR=%hhu NO=%hhu PPU BG=%hhu BL=%hhu SP=%hhu SL=%hhu",
               cpu->PC,
               apu->dmc.enabled,
               apu->pulse1.enabled,
               apu->pulse2.enabled,
               apu->triangle.enabled,
               apu->noise.enabled,
               ppu->flag_show_background,
               ppu->flag_show_left_background,
               ppu->flag_show_sprites,
               ppu->flag_show_left_sprites);
    }
};

struct Instruction {
	const uint8_t opcode;
	const char* name;
	void(*dispatch)(CPU*, NES*, uint16_t, uint8_t);
	const uint8_t mode;
	const uint8_t size;
	const uint8_t cycles;
	const uint8_t page_cross_cycles;

	constexpr Instruction(const uint8_t _opcode, const char _name[4], void(*_dispatch)(CPU*, NES*, uint16_t, uint8_t), const uint8_t _mode, const uint8_t _size, const uint8_t _cycles, const uint8_t _page_crossed_cycles) : opcode(_opcode), name(_name), dispatch(_dispatch), mode(_mode), size(_size), cycles(_cycles), page_cross_cycles(_page_crossed_cycles) {}
};

void PPUnmiShift(PPU* ppu);

uint8_t readPalette(PPU* ppu, uint16_t address);
uint8_t readPPU(NES* nes, uint16_t address);
uint8_t readByte(NES* nes, uint16_t address);
void push16(NES* nes, uint16_t value);
void php(CPU* cpu, NES* nes, uint16_t address, uint8_t mode);

uint16_t read16(NES* nes, uint16_t address);
void execute(NES* nes, uint8_t opcode);
void writeByte(NES* nes, uint16_t address, uint8_t value);
void emulate(NES* nes, double seconds);

void setI(CPU* cpu, bool value);
uint8_t getI(CPU* cpu);

#endif