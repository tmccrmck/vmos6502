#include "include/rom.h"

rom::rom(ines_header h, std::vector<uint8_t> prg_rom, std::vector<uint8_t> chr_rom) {
    header = h;
    prg = prg_rom;
    chr = chr_rom;
}
