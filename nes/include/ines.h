#include <cstdint>
#include <iostream>
#include <cstring>
#include <vector>

const int NES_MAGIC = 0x1a53454e;

namespace ines {
    struct header {
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

    void load(const char *path) {
        std::cout << "Opening nes file" << std::endl;
        // TODO switch to streams
        std::FILE *file = std::fopen(path, "rb");
        if (!file) {
            std::perror("ERROR: failed to open ROM file!");
            return;
        }

        header header{};
        std::fread(&header, sizeof(header), 1, file);

        if (header.magic != NES_MAGIC) {
            std::cerr << "ERROR: invalid .nes file!" << std::endl;
            return;
        }

        int prg_bytes = header.prg_rom_size << 14;
        auto *PRG = new uint8_t[prg_bytes];
        std::fread(PRG, static_cast<size_t>(prg_bytes), 1, file);
        std::vector<uint8_t> prg_rom(std::begin(PRG), std::end(PRG));

        int chr_bytes = header.chr_rom_size << 13;
        auto *CHR = new uint8_t[chr_bytes];
        std::vector<uint8_t> chr_rom(std::begin(CHR), std::end(CHR));

        fclose(file);

    }
}
