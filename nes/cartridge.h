#include <cstdint>

class cartridge {
public:
    cartridge(uint8_t* PRG, uint8_t* CHR);
private:
    bool initialized;
    uint8_t* PRG; // PRG-ROM banks
    int prg_size;
    uint8_t* CHR; // CHR-ROM banks
    int chr_size;
    uint8_t* SRAM; // Save RAM
    bool trainer_present;
    uint8_t* trainer;
    uint8_t mapper; // mapper type
    uint8_t mirror; // mirroring mode
    uint8_t battery_present; // battery present
};
