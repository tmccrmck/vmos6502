#include <cstdint>
#include <iostream>
#include <cstring>

const int NES_MAGIC = 0x1a53454e;

struct Header {
    uint32_t magic;     // iNES cartridge file magic number
    uint8_t numPrg;    // number of 16k PRG-ROM banks
    uint8_t numChr;    // number of 8k CHR-ROM banks
    uint8_t control1;
    uint8_t control2;
    uint8_t numRam;    // number of 8k PRG-RAM banks
    uint8_t padding[7];
};

void loadNesFile(const char* path) {
    std::cout << "Opening nes file" << std::endl;
    FILE *file = fopen(path, "rb");
    if (file == nullptr) {
        std::cerr << "ERROR: failed to open ROM file!" << std::endl;
        return;
    }

    Header header;
    auto amount_read = fread(&header, sizeof(header), 1, file);
    // TODO handle failure

    if (header.magic != NES_MAGIC) {
        std::cerr << "ERROR: invalid .nes file!" << std::endl;
        return;
    }

    int prgSize = header.numPrg << 14;
    uint8_t* PRG = new uint8_t[prgSize];
    amount_read = fread(PRG, prgSize, 1, file);
    // TODO handle failure

    int chrSize = header.numChr << 13;

    uint8_t *CHR;
    if (chrSize == 0) {
        chrSize = 8192;
        CHR = new uint8_t[8192];
        memset(CHR, 0, 8192);
    } else {
        CHR = new uint8_t[chrSize];
    }

    fclose(file);

}
