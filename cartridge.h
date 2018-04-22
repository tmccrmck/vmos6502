#ifndef VMOS6502_CARTRIDGE_H
#define VMOS6502_CARTRIDGE_H

#include <cstdint>
#include <string>

typedef uint8_t byte;

struct Cartridge {
	bool initialized;
	byte* PRG; // PRG-ROM banks
	int prg_size;
	byte* CHR; // CHR-ROM banks
	int chr_size;
	byte* SRAM; // Save RAM
	bool trainer_present;
	byte* trainer;
	byte mapper; // mapper type
	byte mirror; // mirroring mode
	byte battery_present; // battery present

	Cartridge(const std::string path, const std::string SRAM_path);
};
#endif //VMOS6502_CARTRIDGE_H
