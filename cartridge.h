#ifndef VMOS6502_CARTRIDGE_H
#define VMOS6502_CARTRIDGE_H

#include <iostream>
#include <cstring>
#include <cstdint>
#include <string>
#include <array>

typedef uint8_t byte;

constexpr int INES_MAGIC = 0x1a53454e;

struct iNESHeader {
    uint32_t magic;     // iNES cartridge file magic number
    byte num_prg;    // number of 16k PRG-ROM banks
    byte num_chr;    // number of 8k CHR-ROM banks
    byte ctrl1;
    byte ctrl2;
    byte num_ram;    // number of 8k PRG-RAM banks
    byte padding[7];
};

struct Cartridge {
    bool initialized;
    byte *PRG; // PRG-ROM banks
    int prg_size;
    byte *CHR; // CHR-ROM banks
    int chr_size;
    std::array<byte, 8192> SRAM; // Save RAM
    bool trainer_present;
    byte *trainer;
    byte mapper; // mapper type
    byte mirror; // mirroring mode
    byte battery_present; // battery present

    Cartridge(std::string path, std::string SRAM_path);
};

#endif //VMOS6502_CARTRIDGE_H
