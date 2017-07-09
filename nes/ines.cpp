#include <cstdint>
#include <iostream>
#include <cstring>
#include <vector>
#include "include/ines.h"
//#include "include/rom.h"

const int NES_MAGIC = 0x1a53454e;

rom load_nes(const char *path) {
    std::cout << "Opening nes file" << std::endl;
    // TODO switch to streams
    std::FILE *file = std::fopen(path, "rb");
    if (file == nullptr) {
        std::perror("ERROR: failed to open ROM file!");
    }

    ines_header header{};
    std::fread(&header, sizeof(header), 1, file);

    if (header.magic != NES_MAGIC) {
        std::cerr << "ERROR: invalid .nes file!" << std::endl;
    }

    int prg_bytes = header.prg_rom_size << 14;
    std::vector<uint8_t> PRG(static_cast<unsigned long>(prg_bytes));
    std::fread(&PRG[0], static_cast<size_t>(prg_bytes), 1, file);

    int chr_bytes = header.chr_rom_size << 13;
    std::vector<uint8_t> CHR(static_cast<unsigned long>(chr_bytes));
    std::fread(&CHR[0], static_cast<size_t>(prg_bytes), 1, file);

    fclose(file);

    return rom (header, PRG, CHR);
}
