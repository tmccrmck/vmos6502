#include <cstdint>
#include <iostream>
#include <cstring>
#include <vector>

#ifndef VMOS6502_INES_HEADER_H
#define VMOS6502_INES_HEADER_H

struct ines_header {
    uint32_t magic; // four bytes 'N' 'E' 'S' '\x1a'
    uint8_t prg_rom_size; // number of 16k PRG-ROM
    uint8_t chr_rom_size; // number of 8k CHR-ROM
    uint8_t flags_6;
    uint8_t flags_7;
    uint8_t prg_ram_size; // number of 8k PRG-RAM
    uint8_t flags_9;
    uint8_t flags_10;
    uint8_t padding[5];
};

#endif //VMOS6502_INES_HEADER_H
