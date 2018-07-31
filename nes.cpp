#include "nes.h"

// push uint16_t onto stack
void NES::push16(uint16_t value) {
    push(static_cast<byte>(value >> 8));
    push(static_cast<byte>(value));
}

// push byte onto stack
void NES::push(byte value) {
    writeByte(0x100 | static_cast<uint16_t>(cpu->sp), value);
    --cpu->sp;
}

// pop byte from stack
byte NES::pop() {
    ++cpu->sp;
    return readByte(0x100 | static_cast<uint16_t>(cpu->sp));
}

// pop uint16_t onto stack
uint16_t NES::pop16() {
    auto lo = pop();
    auto hi = pop();
    return hi << 8 | lo;
}

NES::NES(const std::string path, const std::string SRAM_path) : initialized(false) {
    std::cout << "Initializing cartridge..." << std::endl;
    auto cartridge = std::make_shared<Cartridge>(path, SRAM_path);

    std::cout << "Initializing controllers..." << std::endl;
    controller1 = new Controller;
    controller2 = new Controller;

    RAM = new byte[2048];
    memset(RAM, 0, 2048);

    std::cout << "Initializing mapper..." << std::endl;
    if (cartridge->mapper == 0) {
        const int prg_banks = cartridge->prg_size >> 14;
        mapper = new Mapper2(cartridge, prg_banks, 0, prg_banks - 1);
    } else if (cartridge->mapper == 1) {
        auto *m = new Mapper1(cartridge);
        m->shift_reg = 0x10;
        m->prg_offsets[1] = m->prgBankOffset(-1);
        mapper = m;
    } else if (cartridge->mapper == 2) {
        const int prg_banks = cartridge->prg_size >> 14;
        mapper = new Mapper2(cartridge, prg_banks, 0, prg_banks - 1);
    } else if (cartridge->mapper == 3) {
        const int prg_banks = cartridge->prg_size >> 14;
        mapper = new Mapper3(cartridge, 0, 0, prg_banks - 1);
    } else if (cartridge->mapper == 4) {
        auto *m = new Mapper4(cartridge);
        m->prg_offsets[0] = m->prgBankOffset(0);
        m->prg_offsets[1] = m->prgBankOffset(1);
        m->prg_offsets[2] = m->prgBankOffset(-2);
        m->prg_offsets[3] = m->prgBankOffset(-1);
        mapper = m;
    } else if (cartridge->mapper == 7) {
        mapper = new Mapper7(cartridge);
    } else {
        std::cerr << "ERROR: cartridge uses Mapper " << static_cast<int>(cartridge->mapper)
                  << ", which isn't currently supported by vmos!" << std::endl;
        return;
    }

    std::cout << "Mapper " << static_cast<int>(cartridge->mapper) << " activated." << std::endl;

    std::cout << "Initializing NES CPU..." << std::endl;
    cpu = new CPU(this);
    cpu->pc = read16(0xFFFC);
    cpu->sp = 0xFD;
    cpu->flags = 0x24;

    std::cout << "Initializing NES APU..." << std::endl;
    apu = std::make_unique<APU>();

    std::cout << "Initializing NES PPU..." << std::endl;
    ppu = std::make_unique<PPU>();

    ppu->writePPUCtrl(0);
    ppu->writePPUMask(0);
    ppu->oam_addr = 0;
    initialized = true;
}

void NES::emulate(double seconds) {
    auto cycles = static_cast<int>(CPU_FREQ * seconds + 0.5);

    while (cycles > 0) {
        int cpuCycles = 0;
        CPU *cpu = this->cpu;

        if (cpu->stall > 0) {
            --cpu->stall;
            cpuCycles = 1;
        } else {
            uint64_t startCycles = cpu->cycles;
            if (cpu->interrupt == interruptNMI) {
                push16(cpu->pc);
                cpu->php();
                cpu->pc = read16(0xFFFA);
                cpu->setI(true);
                cpu->cycles += 7;
            } else if (cpu->interrupt == interruptIRQ) {
                push16(cpu->pc);
                cpu->php();
                cpu->pc = read16(0xFFFE);
                cpu->setI(true);
                cpu->cycles += 7;
            }
            cpu->interrupt = interruptNone;
            byte opcode = readByte(cpu->pc);
            execute(opcode);
            cpuCycles = static_cast<int>(cpu->cycles - startCycles);
        }

        const int ppuCycles = cpuCycles * 3;
        for (int i = 0; i < ppuCycles; ++i) {
            ppu->tickPPU(this->cpu, mapper);
        }

        for (int i = 0; i < cpuCycles; ++i) {
            this->apu->tickAPU(*cpu);
        }
        cycles -= cpuCycles;
    }
}

byte NES::readByte(uint16_t address) {
    if (address < 0x2000) {
        return this->RAM[address & 2047];
    } else if (address < 0x4000) {
        return ppu->readPPURegister(0x2000 + (address & 7), mapper);
    } else if (address == 0x4014) {
        return ppu->readPPURegister(address, mapper);
    } else if (address == 0x4015) {
        // apu reg read
        byte read_status = 0;
        if (apu->pulse1->length_val > 0) {
            read_status |= 1;
        }
        if (apu->pulse2->length_val > 0) {
            read_status |= 2;
        }
        if (apu->triangle.length_val > 0) {
            read_status |= 4;
        }
        if (apu->noise.length_val > 0) {
            read_status |= 8;
        }
        if (apu->dmc.cur_len > 0) {
            read_status |= 16;
        }
        return 0;
    } else if (address == 0x4016) {
        return controller1->readController();
    } else if (address == 0x4017) {
        return controller2->readController();
    } else if (address < 0x6000) {
        return 0;
    } else if (address >= 0x6000) {
        return this->mapper->read(address);
    } else {
        std::cerr << "ERROR: CPU encountered unrecognized read (address 0x" << std::hex << address << std::dec << ')'
                  << std::endl;
        return 0;
    }
}

// 6502 indirect memory bug - only low bytes
uint16_t NES::read16_ff_bug(uint16_t address) {
    const uint16_t a = address;
    const uint16_t b = (a & 0xFF00) | static_cast<uint16_t>(static_cast<byte>(static_cast<byte>(a) + 1));
    const byte lo = readByte(a);
    const byte hi = readByte(b);
    return (static_cast<uint16_t>(hi) << 8) | static_cast<uint16_t>(lo);
}

uint16_t NES::read16(uint16_t address) {
    const byte lo = static_cast<uint16_t>(readByte(address));
    const byte hi = static_cast<uint16_t>(readByte(address + 1));
    return (hi << 8) | lo;
}

void NES::writeByte(uint16_t address, byte value) {
    std::cout << "Writing byte " << unsigned(value) << " to address " << std::hex << address << std::endl;
    if (address < 0x2000) {
        this->RAM[address & 2047] = value;
    } else if (address < 0x4000) {
        ppu->writeRegisterPPU(0x2000 + (address & 7), value, mapper, cpu);
    } else if (address < 0x4014) {
        this->apu->writeRegisterAPU(address, value);
    } else if (address == 0x4014) {
        ppu->writeRegisterPPU(address, value, mapper, cpu);
    } else if (address == 0x4015) {
        this->apu->writeRegisterAPU(address, value);

        class NES;
    } else if (address == 0x4016) {
        controller1->writeController(value);
        controller2->writeController(value);
    } else if (address == 0x4017) {
        this->apu->writeRegisterAPU(address, value);
    } else if (address < 0x6000) {
        // I/O registers
    } else if (address >= 0x6000) {
        this->mapper->write(address, value);
    } else {
        std::cerr << "ERROR: CPU encountered unrecognized write (address 0x" << std::hex << address << std::dec << ')'
                  << std::endl;
    }
}

void NES::execute(byte opcode) {
    const Instruction &instruction = instructions[opcode];

    uint16_t address = 0;
    bool page_crossed = false;
    uint16_t offset;

    switch (instruction.mode) {
        case modeAbsolute:
            address = read16(cpu->pc + 1);
            break;
        case modeAbsoluteX:
            address = read16(cpu->pc + 1) + static_cast<uint16_t>(cpu->x);
            page_crossed = cpu->pagesDiffer(address - static_cast<uint16_t>(cpu->x), address);
            break;
        case modeAbsoluteY:
            address = read16(cpu->pc + 1) + static_cast<uint16_t>(cpu->y);
            page_crossed = cpu->pagesDiffer(address - static_cast<uint16_t>(cpu->y), address);
            break;
        case modeAccumulator:
            address = 0;
            break;
        case modeImmediate:
            address = cpu->pc + 1;
            break;
        case modeImplied:
            address = 0;
            break;
        case modeIndexedIndirect:
            address = read16_ff_bug(static_cast<uint16_t>(static_cast<byte>(readByte(cpu->pc + 1) + cpu->x)));
            break;
        case modeIndirect:
            address = read16_ff_bug(read16(cpu->pc + 1));
            break;
        case modeIndirectIndexed:
            address = read16_ff_bug(static_cast<uint16_t>(readByte(cpu->pc + 1))) + static_cast<uint16_t>(cpu->y);
            page_crossed = cpu->pagesDiffer(address - static_cast<uint16_t>(cpu->y), address);
            break;
        case modeRelative:
            offset = static_cast<uint16_t>(readByte(cpu->pc + 1));
            address = cpu->pc + 2 + offset - ((offset >= 128) << 8);
            break;
        case modeZeroPage:
            address = static_cast<uint16_t>(readByte(cpu->pc + 1));
            break;
        case modeZeroPageX:
            address = static_cast<uint16_t>(static_cast<byte>(readByte(cpu->pc + 1) + cpu->x));
            break;
        case modeZeroPageY:
            address = static_cast<uint16_t>(static_cast<byte>(readByte(cpu->pc + 1) + cpu->y));
            break;
        default:
            break;
    }

    cpu->pc += static_cast<uint16_t>(instruction.size);
    cpu->cycles += static_cast<uint64_t>(instruction.cycles);
    if (page_crossed) {
        cpu->cycles += static_cast<uint64_t>(instruction.page_cross_cycles);
    }

    cpu->executeOpcode(opcode, address, instruction);
}

/*
void NES::printState() {
    std::cout << "CPU status: "
              << "PC=" << cpu->pc <<  std::endl;
    std::cout << "APU status: "
              << "DM=" << apu->dmc.enabled << " P1=" << apu->pulse1.enabled << " P2=" << apu->pulse2.enabled << " TR=" << apu->triangle.enabled<< " NO=" << apu->noise.enabled << std::endl;
    std::cout << "PPU status: "
              << "BG=" << unsigned(ppu->flag_show_background) << " BL=" << unsigned(ppu->flag_show_left_background) << " sp=" << unsigned(ppu->flag_show_sprites) << " SL="<< unsigned(ppu->flag_show_left_sprites) << std::endl;
}
*/

