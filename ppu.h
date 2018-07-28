#ifndef VMOS6502_PPU_H
#define VMOS6502_PPU_H

#include <cstdint>
#include <cstring>
#include <iostream>
#include <algorithm>
#include <array>

#include "mapper.h"
#include "cartridge.h"
#include "cpu.h"

//template <class Mem>
class CPU;

class NES;

class Mapper;

typedef uint8_t byte;

constexpr std::array<uint32_t, 64> palette = {0xff666666, 0xff882a00, 0xffa71214, 0xffa4003b, 0xff7e005c, 0xff40006e,
                                              0xff00066c, 0xff001d56,
                                              0xff003533, 0xff00480b, 0xff005200, 0xff084f00, 0xff4d4000, 0xff000000,
                                              0xff000000, 0xff000000,
                                              0xffadadad, 0xffd95f15, 0xffff4042, 0xfffe2775, 0xffcc1aa0, 0xff7b1eb7,
                                              0xff2031b5, 0xff004e99,
                                              0xff006d6b, 0xff008738, 0xff00930c, 0xff328f00, 0xff8d7c00, 0xff000000,
                                              0xff000000, 0xff000000,
                                              0xfffffeff, 0xffffb064, 0xffff9092, 0xffff76c6, 0xffff6af3, 0xffcc6efe,
                                              0xff7081fe, 0xff229eea,
                                              0xff00bebc, 0xff00d888, 0xff30e45c, 0xff82e045, 0xffdecd48, 0xff4f4f4f,
                                              0xff000000, 0xff000000,
                                              0xfffffeff, 0xffffdfc0, 0xffffd2d3, 0xffffc8e8, 0xffffc2fb, 0xffeac4fe,
                                              0xffc5ccfe, 0xffa5d8f7,
                                              0xff94e5e4, 0xff96efcf, 0xffabf4bd, 0xffccf3b3, 0xfff2ebb5, 0xffb8b8b8,
                                              0xff000000, 0xff000000};

constexpr std::array<std::array<uint16_t, 4>, 5> mirror_tbl = {{
                                                                       {0, 0, 1, 1},
                                                                       {0, 1, 0, 1},
                                                                       {0, 0, 0, 0},
                                                                       {1, 1, 1, 1},
                                                                       {0, 1, 2, 3}
                                                               }};

class PPU {
public:
    int cycle; // 0-340
    int scanline; // 0-261. 0-239 is visible, 240 is postline, 241-260 is the v_blank interval, 261 is preline
    uint64_t frame;

    byte palette_tbl[32];
    byte name_tbl[2048];
    byte oam_tbl[256];

    uint32_t *front;
    uint32_t *back;

    // regs
    uint16_t v; // vram address
    uint16_t t; // temp vram address
    byte x;  // fine x scroll
    byte w;  // write flag
    byte f;  // even/odd flag

    byte reg;

    bool nmi_occurred;
    bool nmi_out;
    bool nmi_last;
    byte nmi_delay;

    byte name_tbl_u8;
    byte attrib_tbl_u8;
    byte low_tile_u8;
    byte high_tile_u8;
    uint64_t tile_data;

    int sprite_cnt;
    uint32_t sprite_patterns[8];
    byte sprite_pos[8];
    byte sprite_priorities[8];
    byte sprite_idx[8];

    // $2000 PPUCTRL
    byte flag_name_tbl;       // 0: $2000.  1: $2400. 2: $2800. 3: $2C00
    byte flag_increment;      // 0: add 1.  1: add 32
    byte flag_sprite_tbl;     // 0: $0000.  1: $1000
    byte flag_background_tbl; // 0: $0000.  1: $1000
    byte flag_sprite_size;    // 0: 8x8.    1: 8x16
    byte flag_rw;             // 0: read.   1: write

    // $2001 PPUMASK
    byte flag_gray;                 // 0: color.   1: grayscale
    byte flag_show_left_background; // 0: hide.    1: show
    byte flag_show_left_sprites;    // 0: hide.    1: show
    byte flag_show_background;      // 0: hide.    1: show
    byte flag_show_sprites;         // 0: hide.    1: show
    byte flag_red_tint;             // 0: normal.  1: emphasized
    byte flag_green_tint;           // 0: normal.  1: emphasized
    byte flag_blue_tint;            // 0: normal.  1: emphasized

    // $2002 PPUSTATUS
    byte flag_sprite_zero_hit;
    byte flag_sprite_overflow;

    // $2003 OAMADDR
    byte oam_addr;

    // $2007 PPUDATA
    byte buffered_data;

    PPU() : cycle(0), scanline(0), frame(0), v(0), t(0), x(0), w(0), f(0), reg(0), nmi_occurred(false), nmi_out(false),
            nmi_last(false),
            nmi_delay(0), name_tbl_u8(0), attrib_tbl_u8(0), low_tile_u8(0), high_tile_u8(0), tile_data(0),
            sprite_cnt(0), flag_name_tbl(0), flag_increment(0),
            flag_sprite_tbl(0), flag_background_tbl(0), flag_sprite_size(0), flag_rw(0), flag_gray(0),
            flag_show_left_background(0), flag_show_left_sprites(0),
            flag_show_background(0), flag_show_sprites(0), flag_red_tint(0), flag_green_tint(0), flag_blue_tint(0),
            flag_sprite_zero_hit(0), flag_sprite_overflow(0),
            oam_addr(0), buffered_data(0) {
        memset(palette_tbl, 0, 32);
        memset(name_tbl, 0, 2048);
        memset(oam_tbl, 0, 256);
        memset(sprite_patterns, 0, 32);
        memset(sprite_pos, 0, 8);
        memset(sprite_priorities, 0, 8);
        memset(sprite_idx, 0, 8);
    }

    void writePPUCtrl(byte x);

    void writePPUMask(byte x);

    void PPUnmiShift();

    byte readPalette(uint16_t address);

    void spritePixel(byte &i, byte &sprite);

    void tickPPU(CPU *cpu, Mapper *mapper);

    byte readPPU(uint16_t address, Mapper *mapper);

    void writePPU(uint16_t address, byte value, Mapper *mapper);

    byte readPPURegister(uint16_t address, Mapper *mapper);

    void writeRegisterPPU(uint16_t address, byte value, Mapper *mapper, CPU *cpu);
};

#endif //VMOS6502_PPU_H
