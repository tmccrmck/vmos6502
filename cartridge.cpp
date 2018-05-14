#include "cartridge.h"

Cartridge::Cartridge(const std::string path, const std::string SRAM_path) : initialized(false) {
	FILE* fp = fopen(path.c_str(), "rb");
	if (fp == nullptr) {
		std::cerr << "ERROR: failed to open ROM file!" << std::endl;
		return;
	}

	iNESHeader header{};
	size_t elems_read = fread(&header, sizeof(header), 1, fp);
	if (elems_read != 1) {
		std::cerr << "ERROR: failed to read ROM header!" << std::endl;
		return;
	}

	if (header.magic != INES_MAGIC) {
		std::cerr << "ERROR: invalid .nes file!" << std::endl;
		return;
	}

	int mapper1 = header.ctrl1 >> 4;
	int mapper2 = header.ctrl2 >> 4;
	mapper = static_cast<byte>(mapper1 | (mapper2 << 4));

	int mirror1 = header.ctrl1 & 1;
	int mirror2 = (header.ctrl1 >> 3) & 1;
	mirror = static_cast<byte>(mirror1 | (mirror2 << 1));

	battery_present = static_cast<byte>((header.ctrl1 >> 1) & 1);

	trainer_present = false;
	if ((header.ctrl1 & 4) == 4) {
		trainer_present = true;
		trainer = new byte[512];
		elems_read = fread(trainer, 512, 1, fp);
		if (elems_read != 1) {
			std::cerr << "ERROR: failed to read trainer!" << std::endl;
			return;
		}
	}

	prg_size = static_cast<int>(header.num_prg) << 14;
	PRG = new byte[prg_size];
	elems_read = fread(PRG, prg_size, 1, fp);
	if (elems_read != 1) {
		std::cerr << "ERROR: failed to read PRG-ROM!" << std::endl;
		return;
	}

	chr_size = static_cast<int>(header.num_chr) << 13;
	if (chr_size == 0) {
		chr_size = 8192;
		CHR = new byte[8192];
		memset(CHR, 0, 8192);
	}
	else {
		CHR = new byte[chr_size];
		elems_read = fread(CHR, chr_size, 1, fp);
		if (elems_read != 1) {
			std::cerr << "ERROR: failed to read CHR-ROM!" << std::endl;
			return;
		}
	}

	fclose(fp);

	SRAM = new byte[8192];

	memset(SRAM, 0, 8192);
	if (battery_present) {
		// try to read saved SRAM
		std::cout << "Attempting to read previously saved SRAM..." << std::endl;
		fp = fopen(SRAM_path.c_str(), "rb");
		if (fp == nullptr || (fread(SRAM, 8192, 1, fp) != 1)) {
			std::cout << "WARN: failed to open SRAM file!" << std::endl;
		}
		else {
			fclose(fp);
		}
	}
	initialized = true;
}
