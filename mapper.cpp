#include "mapper.h"

int Mapper1::prgBankOffset(Cartridge* c, int index) {
	if (index >= 0x80) {
		index -= 0x100;
	}
	index %= c->prg_size >> 14;
	int offset = index << 14;
	if (offset < 0) {
		offset += c->prg_size;
	}
	return offset;
}

int Mapper1::chrBankOffset(Cartridge* cartridge, int index) {
	if (index >= 0x80) {
		index -= 0x100;
	}
	index %= cartridge->chr_size >> 12;
	int offset = index << 12;
	if (offset < 0) {
		offset += cartridge->chr_size;
	}
	return offset;
}

// PRG ROM bank mode  0-1: switch 32k at $8000,     ignore low bit of bank number
//                    2  : fix first bank at $8000, switch 16k bank at $C000
//                    3  : fix last bank at $C000,  switch 16k bank at $8000
//
// CHR ROM bank mode  0  : switch 8k
//                    1  : switch two 4k banks
void Mapper1::updateOffsets(Cartridge* cartridge) {
	switch (prg_mode) {
	case 0:
	case 1:
		prg_offsets[0] = prgBankOffset(cartridge, static_cast<int>(prg_bank & 0xFE));
		prg_offsets[1] = prgBankOffset(cartridge, static_cast<int>(prg_bank | 0x01));
		break;
	case 2:
		prg_offsets[0] = 0;
		prg_offsets[1] = prgBankOffset(cartridge, static_cast<int>(prg_bank));
		break;
	case 3:
		prg_offsets[0] = prgBankOffset(cartridge, static_cast<int>(prg_bank));
		prg_offsets[1] = prgBankOffset(cartridge, -1);
		break;
	}

	switch (chr_mode) {
	case 0:
		chr_offsets[0] = chrBankOffset(cartridge, static_cast<int>(chr_bank0 & 0xFE));
		chr_offsets[1] = chrBankOffset(cartridge, static_cast<int>(chr_bank0 | 0x01));
		break;
	case 1:
		chr_offsets[0] = chrBankOffset(cartridge, static_cast<int>(chr_bank0));
		chr_offsets[1] = chrBankOffset(cartridge, static_cast<int>(chr_bank1));
		break;
	}
}

// Control ($8000-$9FFF)
void Mapper1::writeCtrl(Cartridge* cartridge, byte value) {
	control = value;
	chr_mode = (value >> 4) & 1;
	prg_mode = (value >> 2) & 3;
	byte mirror = value & 3;
	switch (mirror) {
	case 0:
		cartridge->mirror = MirrorSingle0;
		break;
	case 1:
		cartridge->mirror = MirrorSingle1;
		break;
	case 2:
		cartridge->mirror = MirrorVertical;
		break;
	case 3:
		cartridge->mirror = MirrorHorizontal;
		break;
	}
}

uint8_t Mapper1::read(Cartridge *cartridge, uint16_t address) {
	if (address < 0x2000) {
		const uint16_t bank = address >> 12;
		const uint16_t offset = address & 4095;
		return cartridge->CHR[chr_offsets[bank] + static_cast<int>(offset)];
	}
	else if (address >= 0x8000) {
		address -= 0x8000;
		const uint16_t bank = address >> 14;
		const uint16_t offset = address & 16383;
		return cartridge->PRG[prg_offsets[bank] + static_cast<int>(offset)];
	}
	else if (address >= 0x6000) {
		return cartridge->SRAM[static_cast<int>(address) - 0x6000];
	}
	else {
		std::cerr << "ERROR: Mapper1 encountered unrecognized read (address 0x" << std::hex << address << std::dec << ')' << std::endl;
		return 0;
	}
}

int Mapper4::prgBankOffset(Cartridge* c, int index) {
	if (index >= 0x80) {
		index -= 0x100;
	}
	index %= c->prg_size >> 13;
	int offset = index << 13;
	if (offset < 0) {
		offset += c->prg_size;
	}
	return offset;
}

void triggerIRQ2(CPU* cpu) {
    auto i = (cpu->flags & 4) >> 2;
	if (i == 0) {
		cpu->interrupt = interruptIRQ;
	}
}

void Mapper4::updateCounter(CPU* cpu) {
	if (counter == 0) {
		counter = reload;
	}
	else {
		--counter;
		if (counter == 0 && IRQ_enable) {
			triggerIRQ2(cpu);
		}
	}
}

int Mapper4::chrBankOffset(Cartridge* cartridge, int index) {
	if (index >= 0x80) {
		index -= 0x100;
	}
	index %= cartridge->chr_size >> 10;
	int offset = index << 10;
	if (offset < 0) {
		offset += cartridge->chr_size;
	}
	return offset;
}

void Mapper4::updateOffsets(Cartridge* cartridge) {
	switch (prg_mode) {
	case 0:
		prg_offsets[0] = prgBankOffset(cartridge, static_cast<int>(regs[6]));
		prg_offsets[1] = prgBankOffset(cartridge, static_cast<int>(regs[7]));
		prg_offsets[2] = prgBankOffset(cartridge, -2);
		prg_offsets[3] = prgBankOffset(cartridge, -1);
		break;
	case 1:
		prg_offsets[0] = prgBankOffset(cartridge, -2);
		prg_offsets[1] = prgBankOffset(cartridge, static_cast<int>(regs[7]));
		prg_offsets[2] = prgBankOffset(cartridge, static_cast<int>(regs[6]));
		prg_offsets[3] = prgBankOffset(cartridge, -1);
		break;
	}

	switch (chr_mode) {
	case 0:
		chr_offsets[0] = chrBankOffset(cartridge, static_cast<int>(regs[0] & 0xFE));
		chr_offsets[1] = chrBankOffset(cartridge, static_cast<int>(regs[0] | 0x01));
		chr_offsets[2] = chrBankOffset(cartridge, static_cast<int>(regs[1] & 0xFE));
		chr_offsets[3] = chrBankOffset(cartridge, static_cast<int>(regs[1] | 0x01));
		chr_offsets[4] = chrBankOffset(cartridge, static_cast<int>(regs[2]));
		chr_offsets[5] = chrBankOffset(cartridge, static_cast<int>(regs[3]));
		chr_offsets[6] = chrBankOffset(cartridge, static_cast<int>(regs[4]));
		chr_offsets[7] = chrBankOffset(cartridge, static_cast<int>(regs[5]));
		break;
	case 1:
		chr_offsets[0] = chrBankOffset(cartridge, static_cast<int>(regs[2]));
		chr_offsets[1] = chrBankOffset(cartridge, static_cast<int>(regs[3]));
		chr_offsets[2] = chrBankOffset(cartridge, static_cast<int>(regs[4]));
		chr_offsets[3] = chrBankOffset(cartridge, static_cast<int>(regs[5]));
		chr_offsets[4] = chrBankOffset(cartridge, static_cast<int>(regs[0] & 0xFE));
		chr_offsets[5] = chrBankOffset(cartridge, static_cast<int>(regs[0] | 0x01));
		chr_offsets[6] = chrBankOffset(cartridge, static_cast<int>(regs[1] & 0xFE));
		chr_offsets[7] = chrBankOffset(cartridge, static_cast<int>(regs[1] | 0x01));
		break;
	}
}

