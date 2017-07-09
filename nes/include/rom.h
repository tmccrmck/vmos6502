#ifndef VMOS6502_ROM_H
#define VMOS6502_ROM_H

#include <cstdint>
#include "ines_header.h"

class rom {
    ines_header header;
    std::vector<uint8_t> prg;
    std::vector<uint8_t> chr;
public:
    rom(ines_header header, std::vector<uint8_t> prg_rom, std::vector<uint8_t> chr_rom);
};

#endif //VMOS6502_ROM_H
