#include "mapper.h"

int Mapper1::prgBankOffset(int index) {
    if (index >= 0x80) {
        index -= 0x100;
    }
    index %= cartridge->prg_size >> 14;
    int offset = index << 14;
    if (offset < 0) {
        offset += cartridge->prg_size;
    }
    return offset;
}

int Mapper1::chrBankOffset(int index) {
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
void Mapper1::updateOffsets() {
    switch (prg_mode) {
        case 0:
        case 1:
            prg_offsets[0] = prgBankOffset(static_cast<int>(prg_bank & 0xFE));
            prg_offsets[1] = prgBankOffset(static_cast<int>(prg_bank | 0x01));
            break;
        case 2:
            prg_offsets[0] = 0;
            prg_offsets[1] = prgBankOffset(static_cast<int>(prg_bank));
            break;
        case 3:
            prg_offsets[0] = prgBankOffset(static_cast<int>(prg_bank));
            prg_offsets[1] = prgBankOffset(-1);
            break;
    }

    switch (chr_mode) {
        case 0:
            chr_offsets[0] = chrBankOffset(static_cast<int>(chr_bank0 & 0xFE));
            chr_offsets[1] = chrBankOffset(static_cast<int>(chr_bank0 | 0x01));
            break;
        case 1:
            chr_offsets[0] = chrBankOffset(static_cast<int>(chr_bank0));
            chr_offsets[1] = chrBankOffset(static_cast<int>(chr_bank1));
            break;
    }
}

// Control ($8000-$9FFF)
void Mapper1::writeCtrl(byte value) {
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

uint8_t Mapper1::read(uint16_t address) {
    if (address < 0x2000) {
        const uint16_t bank = address >> 12;
        const uint16_t offset = address & 4095;
        return cartridge->CHR[chr_offsets[bank] + static_cast<int>(offset)];
    } else if (address >= 0x8000) {
        address -= 0x8000;
        const uint16_t bank = address >> 14;
        const uint16_t offset = address & 16383;
        return cartridge->PRG[prg_offsets[bank] + static_cast<int>(offset)];
    } else if (address >= 0x6000) {
        return cartridge->SRAM[static_cast<int>(address) - 0x6000];
    } else {
        std::cerr << "ERROR: Mapper1 encountered unrecognized read (address 0x" << std::hex << address << std::dec
                  << ')' << std::endl;
        return 0;
    }
}

void Mapper1::write(uint16_t address, uint8_t value) {
    if (address < 0x2000) {
        const uint16_t bank = address >> 12;
        const uint16_t offset = address & 4095;
        cartridge->CHR[chr_offsets[bank] + static_cast<int>(offset)] = value;
    } else if (address >= 0x8000) {
        if ((value & 0x80) == 0x80) {
            shift_reg = 0x10;
            writeCtrl(control | 0x0C);
            updateOffsets();
        } else {
            const bool complete = (shift_reg & 1) == 1;
            shift_reg >>= 1;
            shift_reg |= (value & 1) << 4;
            if (complete) {
                if (address <= 0x9FFF) {
                    writeCtrl(shift_reg);
                } else if (address <= 0xBFFF) {
                    // CHRbank 0 ($A000-$BFFF)
                    chr_bank0 = shift_reg;
                } else if (address <= 0xDFFF) {
                    // CHRbank 1 ($C000-$DFFF)
                    chr_bank1 = shift_reg;
                } else {
                    // PRGbank ($E000-$FFFF)
                    prg_bank = shift_reg & 0x0F;
                }
                updateOffsets();
                shift_reg = 0x10;
            }
        }
    } else if (address >= 0x6000) {
        cartridge->SRAM[static_cast<int>(address) - 0x6000] = value;
    } else {
        std::cerr << "ERROR: Mapper1 encountered unrecognized write (address 0x" << std::hex << address << std::dec
                  << ')' << std::endl;
    }
}

int Mapper4::prgBankOffset(int index) {
    if (index >= 0x80) {
        index -= 0x100;
    }
    index %= cartridge->prg_size >> 13;
    int offset = index << 13;
    if (offset < 0) {
        offset += cartridge->prg_size;
    }
    return offset;
}

int Mapper4::chrBankOffset(int index) {
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

void Mapper4::updateOffsets() {
    switch (prg_mode) {
        case 0:
            prg_offsets[0] = prgBankOffset(static_cast<int>(regs[6]));
            prg_offsets[1] = prgBankOffset(static_cast<int>(regs[7]));
            prg_offsets[2] = prgBankOffset(-2);
            prg_offsets[3] = prgBankOffset(-1);
            break;
        case 1:
            prg_offsets[0] = prgBankOffset(-2);
            prg_offsets[1] = prgBankOffset(static_cast<int>(regs[7]));
            prg_offsets[2] = prgBankOffset(static_cast<int>(regs[6]));
            prg_offsets[3] = prgBankOffset(-1);
            break;
        default:break;
    }

    switch (chr_mode) {
        case 0:
            chr_offsets[0] = chrBankOffset(static_cast<int>(regs[0] & 0xFE));
            chr_offsets[1] = chrBankOffset(static_cast<int>(regs[0] | 0x01));
            chr_offsets[2] = chrBankOffset(static_cast<int>(regs[1] & 0xFE));
            chr_offsets[3] = chrBankOffset(static_cast<int>(regs[1] | 0x01));
            chr_offsets[4] = chrBankOffset(static_cast<int>(regs[2]));
            chr_offsets[5] = chrBankOffset(static_cast<int>(regs[3]));
            chr_offsets[6] = chrBankOffset(static_cast<int>(regs[4]));
            chr_offsets[7] = chrBankOffset(static_cast<int>(regs[5]));
            break;
        case 1:
            chr_offsets[0] = chrBankOffset(static_cast<int>(regs[2]));
            chr_offsets[1] = chrBankOffset(static_cast<int>(regs[3]));
            chr_offsets[2] = chrBankOffset(static_cast<int>(regs[4]));
            chr_offsets[3] = chrBankOffset(static_cast<int>(regs[5]));
            chr_offsets[4] = chrBankOffset(static_cast<int>(regs[0] & 0xFE));
            chr_offsets[5] = chrBankOffset(static_cast<int>(regs[0] | 0x01));
            chr_offsets[6] = chrBankOffset(static_cast<int>(regs[1] & 0xFE));
            chr_offsets[7] = chrBankOffset(static_cast<int>(regs[1] | 0x01));
            break;
        default:break;
    }
}

uint8_t Mapper4::read(uint16_t address) {
    if (address < 0x2000) {
        const uint16_t bank = address >> 10;
        const uint16_t offset = address & 1023;
        return cartridge->CHR[chr_offsets[bank] + static_cast<int>(offset)];
    } else if (address >= 0x8000) {
        address -= 0x8000;
        const uint16_t bank = address >> 13;
        const uint16_t offset = address & 8191;
        return cartridge->PRG[prg_offsets[bank] + static_cast<int>(offset)];
    } else if (address >= 0x6000) {
        return cartridge->SRAM[static_cast<int>(address) - 0x6000];
    } else {
        std::cerr << "ERROR: Mapper4 encountered unrecognized read (address 0x" << std::hex << address << std::dec
                  << ')' << std::endl;
        return 0;
    }
}

void Mapper4::write(uint16_t address, uint8_t value) {
    if (address < 0x2000) {
        const uint16_t bank = address >> 10;
        const uint16_t offset = address & 1023;
        cartridge->CHR[chr_offsets[bank] + static_cast<int>(offset)] = value;
    } else if (address >= 0x8000) {
        if (address <= 0x9FFF && (address & 1) == 0) {
            // bank select
            prg_mode = (value >> 6) & 1;
            chr_mode = (value >> 7) & 1;
            reg = value & 7;
            updateOffsets();
        } else if (address <= 0x9FFF && (address & 1)) {
            // bank data
            regs[reg] = value;
            updateOffsets();
        } else if (address <= 0xBFFF && (address & 1) == 0) {
            switch (value & 1) {
                case 0:
                    cartridge->mirror = MirrorVertical;
                    break;
                case 1:
                    cartridge->mirror = MirrorHorizontal;
                    break;
            }
        } else if (address <= 0xBFFF && (address & 1)) {
            // TODO
        } else if (address <= 0xDFFF && (address & 1) == 0) {
            // IRQ latch
            reload = value;
        } else if (address <= 0xDFFF && (address & 1)) {
            // IRQ reload
            counter = 0;
        } else if ((address & 1) == 0) {
            // IRQ disable
            IRQ_enable = false;
        } else {
            // IRQ enable
            IRQ_enable = true;
        }
    } else if (address >= 0x6000) {
        cartridge->SRAM[static_cast<int>(address) - 0x6000] = value;
    } else {
        std::cerr << "ERROR: Mapper4 encountered unrecognized write (address 0x" << std::hex << address << std::dec
                  << ')' << std::endl;
    }
}

uint8_t Mapper2::read(uint16_t address) {
    if (address < 0x2000) {
        return cartridge->CHR[address];
    } else if (address >= 0xC000) {
        const int index = (prg_bank2 << 14) + static_cast<int>(address - 0xC000);
        return cartridge->PRG[index];
    } else if (address >= 0x8000) {
        const int index = (prg_bank1 << 14) + static_cast<int>(address - 0x8000);
        return cartridge->PRG[index];
    } else if (address >= 0x6000) {
        const int index = static_cast<int>(address) - 0x6000;
        return cartridge->SRAM[index];
    } else {
        std::cerr << "ERROR: Mapper2 encountered unrecognized read (address 0x" << std::hex << address << std::dec
                  << ')' << std::endl;
        return 0;
    }
}

void Mapper2::write(uint16_t address, uint8_t value) {
    if (address < 0x2000) {
        cartridge->CHR[address] = value;
    } else if (address >= 0x8000) {
        prg_bank1 = static_cast<int>(value) % prg_banks;
    } else if (address >= 0x6000) {
        const int index = static_cast<int>(address) - 0x6000;
        cartridge->SRAM[index] = value;
    } else {
        std::cerr << "ERROR: Mapper2 encountered unrecognized write (address 0x" << std::hex << address << std::dec
                  << ')' << std::endl;
    }
}

uint8_t Mapper3::read(uint16_t address) {
    if (address < 0x2000) {
        const int index = chr_bank * 0x2000 + static_cast<int>(address);
        return cartridge->CHR[index];
    } else if (address >= 0xC000) {
        const int index = prg_bank2 * 0x4000 + static_cast<int>(address - 0xC000);
        return cartridge->PRG[index];
    } else if (address >= 0x8000) {
        const int index = prg_bank1 * 0x4000 + static_cast<int>(address - 0x8000);
        return cartridge->PRG[index];
    } else if (address >= 0x6000) {
        const int index = int(address) - 0x6000;
        return cartridge->SRAM[index];
    } else {
        std::cerr << "ERROR: Mapper3 encountered unrecognized read (address 0x" << std::hex << address << std::dec
                  << ')' << std::endl;
        return 0;
    }
}

void Mapper3::write(uint16_t address, uint8_t value) {
    if (address < 0x2000) {
        const int index = chr_bank * 0x2000 + static_cast<int>(address);
        cartridge->CHR[index] = value;
    } else if (address >= 0x8000) {
        chr_bank = static_cast<int>(value & 3);
    } else if (address >= 0x6000) {
        const int index = static_cast<int>(address) - 0x6000;
        cartridge->SRAM[index] = value;
    } else {
        std::cerr << "ERROR: Mapper3 encountered unrecognized write (address 0x" << std::hex << address << std::dec
                  << ')' << std::endl;
    }
}

uint8_t Mapper7::read(uint16_t address) {
    if (address < 0x2000) {
        return cartridge->CHR[address];
    } else if (address >= 0x8000) {
        const int index = (prg_bank << 15) + static_cast<int>(address - 0x8000);
        return cartridge->PRG[index];
    } else if (address >= 0x6000) {
        const int index = static_cast<int>(address) - 0x6000;
        return cartridge->SRAM[index];
    } else {
        std::cerr << "ERROR: Mapper7 encountered unrecognized read (address 0x" << std::hex << address << std::dec
                  << ')' << std::endl;
        return 0;
    }
}

void Mapper7::write(uint16_t address, uint8_t value) {
    if (address < 0x2000) {
        cartridge->CHR[address] = value;
    } else if (address >= 0x8000) {
        prg_bank = static_cast<int>(value & 7);
        switch (value & 0x10) {
            case 0x00:
                cartridge->mirror = MirrorSingle0;
                break;
            case 0x10:
                cartridge->mirror = MirrorSingle1;
                break;
            default:
                break;
        }
    } else if (address >= 0x6000) {
        int index = static_cast<int>(address) - 0x6000;
        cartridge->SRAM[index] = value;
    } else {
        std::cerr << "ERROR: Mapper7 encountered unrecognized write (address 0x" << std::hex << address << std::dec
                  << ')' << std::endl;
    }
}
