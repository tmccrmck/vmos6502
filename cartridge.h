#ifndef VMOS6502_CARTRIDGE_H
#define VMOS6502_CARTRIDGE_H

#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>

struct Cartridge {
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

	Cartridge(const std::string path, const std::string SRAM_path);
};
#endif //VMOS6502_CARTRIDGE_H
