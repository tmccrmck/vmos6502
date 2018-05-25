#ifndef VMOS6502_MAPPER_H
#define VMOS6502_MAPPER_H

#include <iostream>

#include "cpu.h"
#include "cartridge.h"
#include "ppu.h"

class NES;

//template <class Mem>
//class CPU;

enum MirrorModes {
	MirrorHorizontal = 0,
	MirrorVertical = 1,
	MirrorSingle0 = 2,
	MirrorSingle1 = 3,
	MirrorFour = 4
};

struct Mapper {
	virtual uint8_t read(Cartridge* cartridge, uint16_t address) = 0;
	virtual void write(Cartridge* cartridge, uint16_t address, uint8_t value) = 0;
};

struct Mapper1 : public Mapper {
	uint8_t shift_reg;
	uint8_t control;
	uint8_t prg_mode;
	uint8_t chr_mode;
	uint8_t prg_bank;
	uint8_t chr_bank0;
	uint8_t chr_bank1;
	int prg_offsets[2];
	int chr_offsets[2];

	int prgBankOffset(Cartridge* c, int index);
	int chrBankOffset(Cartridge* cartridge, int index);
	void updateOffsets(Cartridge* cartridge);
	void writeCtrl(Cartridge* cartridge, uint8_t value);

	uint8_t read(Cartridge* cartridge, uint16_t address);

	void write(Cartridge* cartridge, uint16_t address, uint8_t value) {
		if (address < 0x2000) {
			const uint16_t bank = address >> 12;
			const uint16_t offset = address & 4095;
			cartridge->CHR[chr_offsets[bank] + static_cast<int>(offset)] = value;
		}
		else if (address >= 0x8000) {
			if ((value & 0x80) == 0x80) {
				shift_reg = 0x10;
				writeCtrl(cartridge, control | 0x0C);
				updateOffsets(cartridge);
			}
			else {
				const bool complete = (shift_reg & 1) == 1;
				shift_reg >>= 1;
				shift_reg |= (value & 1) << 4;
				if (complete) {
					if (address <= 0x9FFF) {
						writeCtrl(cartridge, shift_reg);
					}
					else if (address <= 0xBFFF) {
						// CHRbank 0 ($A000-$BFFF)
						chr_bank0 = shift_reg;
					}
					else if (address <= 0xDFFF) {
						// CHRbank 1 ($C000-$DFFF)
						chr_bank1 = shift_reg;
					}
					else {
						// PRGbank ($E000-$FFFF)
						prg_bank = shift_reg & 0x0F;
					}
					updateOffsets(cartridge);
					shift_reg = 0x10;
				}
			}
		}
		else if (address >= 0x6000) {
			cartridge->SRAM[static_cast<int>(address) - 0x6000] = value;
		}
		else {
			std::cerr << "ERROR: Mapper1 encountered unrecognized write (address 0x" << std::hex << address << std::dec << ')' << std::endl;
		}
	}

	Mapper1() : shift_reg(0), control(0), prg_mode(0), chr_mode(0), prg_bank(0), chr_bank0(0), chr_bank1(0), prg_offsets{ 0, 0 }, chr_offsets{ 0, 0 } {}
};

struct Mapper2 : public Mapper {
	int prg_banks;
	int prg_bank1;
	int prg_bank2;

	uint8_t read(Cartridge* cartridge, uint16_t address) {
		if (address < 0x2000) {
			return cartridge->CHR[address];
		}
		else if (address >= 0xC000) {
			const int index = (prg_bank2 << 14) + static_cast<int>(address - 0xC000);
			return cartridge->PRG[index];
		}
		else if (address >= 0x8000) {
			const int index = (prg_bank1 << 14) + static_cast<int>(address - 0x8000);
			return cartridge->PRG[index];
		}
		else if (address >= 0x6000) {
			const int index = static_cast<int>(address) - 0x6000;
			return cartridge->SRAM[index];
		}
		else {
			std::cerr << "ERROR: Mapper2 encountered unrecognized read (address 0x" << std::hex << address << std::dec << ')' << std::endl;
			return 0;
		}
	}

	void write(Cartridge* cartridge, uint16_t address, uint8_t value) {
		if (address < 0x2000) {
			cartridge->CHR[address] = value;
		}
		else if (address >= 0x8000) {
			prg_bank1 = static_cast<int>(value) % prg_banks;
		}
		else if (address >= 0x6000) {
			const int index = static_cast<int>(address) - 0x6000;
			cartridge->SRAM[index] = value;
		}
		else {
			std::cerr << "ERROR: Mapper2 encountered unrecognized write (address 0x" << std::hex << address << std::dec << ')' << std::endl;
		}
	}

	Mapper2(int _prgBanks, int _prgBank1, int _prgBank2) : prg_banks(_prgBanks), prg_bank1(_prgBank1), prg_bank2(_prgBank2) {}
};

struct Mapper3 : public Mapper {
	int chr_bank;
	int prg_bank1;
	int prg_bank2;

	uint8_t read(Cartridge* cartridge, uint16_t address) {
		if (address < 0x2000) {
			const int index = chr_bank * 0x2000 + static_cast<int>(address);
			return cartridge->CHR[index];
		}
		else if (address >= 0xC000) {
			const int index = prg_bank2 * 0x4000 + static_cast<int>(address - 0xC000);
			return cartridge->PRG[index];
		}
		else if (address >= 0x8000) {
			const int index = prg_bank1 * 0x4000 + static_cast<int>(address - 0x8000);
			return cartridge->PRG[index];
		}
		else if (address >= 0x6000) {
			const int index = int(address) - 0x6000;
			return cartridge->SRAM[index];
		}
		else {
			std::cerr << "ERROR: Mapper3 encountered unrecognized read (address 0x" << std::hex << address << std::dec << ')' << std::endl;
			return 0;
		}
	}

	void write(Cartridge* cartridge, uint16_t address, uint8_t value) {
		if (address < 0x2000) {
			const int index = chr_bank * 0x2000 + static_cast<int>(address);
			cartridge->CHR[index] = value;
		}
		else if (address >= 0x8000) {
			chr_bank = static_cast<int>(value & 3);
		}
		else if (address >= 0x6000) {
			const int index = static_cast<int>(address) - 0x6000;
			cartridge->SRAM[index] = value;
		}
		else {
			std::cerr << "ERROR: Mapper3 encountered unrecognized write (address 0x" << std::hex << address << std::dec << ')' << std::endl;
		}
	}

	Mapper3(int _chrBank, int _prgBank1, int _prgBank2) : chr_bank(_chrBank), prg_bank1(_prgBank1), prg_bank2(_prgBank2) {}
};

struct Mapper4 : public Mapper {
	uint8_t reg;
	uint8_t regs[8];
	uint8_t prg_mode;
	uint8_t chr_mode;
	int prg_offsets[4];
	int chr_offsets[8];
	uint8_t reload;
	uint8_t counter;
	bool IRQ_enable;

	int prgBankOffset(Cartridge* c, int index);
	int chrBankOffset(Cartridge* cartridge, int index);
	void updateOffsets(Cartridge* cartridge);

	uint8_t read(Cartridge* cartridge, uint16_t address) {
		if (address < 0x2000) {
			const uint16_t bank = address >> 10;
			const uint16_t offset = address & 1023;
			return cartridge->CHR[chr_offsets[bank] + static_cast<int>(offset)];
		}
		else if (address >= 0x8000) {
			address -= 0x8000;
			const uint16_t bank = address >> 13;
			const uint16_t offset = address & 8191;
			return cartridge->PRG[prg_offsets[bank] + static_cast<int>(offset)];
		}
		else if (address >= 0x6000) {
			return cartridge->SRAM[static_cast<int>(address) - 0x6000];
		}
		else {
			std::cerr << "ERROR: Mapper4 encountered unrecognized read (address 0x" << std::hex << address << std::dec << ')' << std::endl;
			return 0;
		}
	}

	void write(Cartridge* cartridge, uint16_t address, uint8_t value) {
		if (address < 0x2000) {
			const uint16_t bank = address >> 10;
			const uint16_t offset = address & 1023;
			cartridge->CHR[chr_offsets[bank] + static_cast<int>(offset)] = value;
		}
		else if (address >= 0x8000) {
			if (address <= 0x9FFF && (address & 1) == 0) {
				// bank select
				prg_mode = (value >> 6) & 1;
				chr_mode = (value >> 7) & 1;
				reg = value & 7;
				updateOffsets(cartridge);
			}
			else if (address <= 0x9FFF && (address & 1)) {
				// bank data
				regs[reg] = value;
				updateOffsets(cartridge);
			}
			else if (address <= 0xBFFF && (address & 1) == 0) {
				switch (value & 1) {
				case 0:
					cartridge->mirror = MirrorVertical;
					break;
				case 1:
					cartridge->mirror = MirrorHorizontal;
					break;
				}
			}
			else if (address <= 0xBFFF && (address & 1)) {
				// TODO
			}
			else if (address <= 0xDFFF && (address & 1) == 0) {
				// IRQ latch
				reload = value;
			}
			else if (address <= 0xDFFF && (address & 1)) {
				// IRQ reload
				counter = 0;
			}
			else if ((address & 1) == 0) {
				// IRQ disable
				IRQ_enable = false;
			}
			else {
				// IRQ enable
				IRQ_enable = true;
			}
		}
		else if (address >= 0x6000) {
			cartridge->SRAM[static_cast<int>(address) - 0x6000] = value;
		}
		else {
			std::cerr << "ERROR: Mapper4 encountered unrecognized write (address 0x" << std::hex << address << std::dec << ')' << std::endl;
		}
	}

	Mapper4() : reg(0), regs{ 0, 0, 0, 0, 0, 0, 0, 0 }, prg_mode(0), chr_mode(0), prg_offsets{ 0, 0, 0, 0 }, chr_offsets{ 0, 0, 0, 0, 0, 0, 0, 0 }, reload(0), counter(0), IRQ_enable(false) {}
};

struct Mapper7 : public Mapper {
	int prg_bank;

	uint8_t read(Cartridge* cartridge, uint16_t address) {
		if (address < 0x2000) {
			return cartridge->CHR[address];
		}
		else if (address >= 0x8000) {
			const int index = (prg_bank << 15) + static_cast<int>(address - 0x8000);
			return cartridge->PRG[index];
		}
		else if (address >= 0x6000) {
			const int index = static_cast<int>(address) - 0x6000;
			return cartridge->SRAM[index];
		}
		else {
			std::cerr << "ERROR: Mapper7 encountered unrecognized read (address 0x" << std::hex << address << std::dec << ')' << std::endl;
			return 0;
		}
	}

	void write(Cartridge* cartridge, uint16_t address, uint8_t value) {
		if (address < 0x2000) {
			cartridge->CHR[address] = value;
		}
		else if (address >= 0x8000) {
			prg_bank = static_cast<int>(value & 7);
			switch (value & 0x10) {
			case 0x00:
				cartridge->mirror = MirrorSingle0;
				break;
			case 0x10:
				cartridge->mirror = MirrorSingle1;
				break;
			}
		}
		else if (address >= 0x6000) {
			int index = static_cast<int>(address) - 0x6000;
			cartridge->SRAM[index] = value;
		}
		else {
			std::cerr << "ERROR: Mapper7 encountered unrecognized write (address 0x" << std::hex << address << std::dec << ')' << std::endl;
		}
	}

	Mapper7() : prg_bank(0) {}
};

#endif //VMOS6502_MAPPER_H

