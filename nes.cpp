#include "nes.h"

NES::NES(const std::string path, const std::string SRAM_path) : initialized(false) {
	std::cout << "Initializing cartridge..." << std::endl;
	cartridge = new Cartridge(path, SRAM_path);
	if (!cartridge->initialized) return;

	std::cout << "Initializing controllers..." << std::endl;
	controller1 = new Controller;
	controller2 = new Controller;

	RAM = new byte[2048];
	memset(RAM, 0, 2048);

	std::cout << "Initializing mapper..." << std::endl;
	if (cartridge->mapper == 0) {
		const int prg_banks = cartridge->prg_size >> 14;
		mapper = new Mapper2(prg_banks, 0, prg_banks - 1);
	}
	else if (cartridge->mapper == 1) {
		Mapper1* m = new Mapper1();
		m->shift_reg = 0x10;
		m->prg_offsets[1] = m->prgBankOffset(cartridge, -1);
		mapper = m;
	}
	else if (cartridge->mapper == 2) {
		const int prg_banks = cartridge->prg_size >> 14;
		mapper = new Mapper2(prg_banks, 0, prg_banks - 1);
	}
	else if (cartridge->mapper == 3) {
		const int prg_banks = cartridge->prg_size >> 14;
		mapper = new Mapper3(0, 0, prg_banks - 1);
	}
	else if (cartridge->mapper == 4) {
		Mapper4* m = new Mapper4();
		m->prg_offsets[0] = m->prgBankOffset(cartridge, 0);
		m->prg_offsets[1] = m->prgBankOffset(cartridge, 1);
		m->prg_offsets[2] = m->prgBankOffset(cartridge, -2);
		m->prg_offsets[3] = m->prgBankOffset(cartridge, -1);
		mapper = m;
	}
	else if (cartridge->mapper == 7) {
		mapper = new Mapper7();
	}
	else {
		std::cerr << "ERROR: cartridge uses Mapper " << static_cast<int>(cartridge->mapper) << ", which isn't currently supported by vmos!" << std::endl;
		return;
	}

	std::cout << "Mapper " << static_cast<int>(cartridge->mapper) << " activated." << std::endl;

	std::cout << "Initializing NES CPU..." << std::endl;
	cpu = new CPU();

	cpu->PC = read16(0xFFFC);
	cpu->SP = 0xFD;
	cpu->flags = 0x24;

	std::cout << "Initializing NES APU..." << std::endl;
	apu = new APU();
	apu->noise.shift_reg = 1;
	apu->pulse1.channel = 1;
	apu->pulse2.channel = 2;

	std::cout << "Initializing NES PPU..." << std::endl;
	ppu = new PPU();
	ppu->front = new uint32_t[256 * 240];
	ppu->back = new uint32_t[256 * 240];
	ppu->cycle = 340;
	ppu->scanline = 250;
	ppu->frame = 0;

	ppu->writePPUCtrl(0);
	ppu->writePPUMask(0);
	ppu->oam_addr = 0;
	initialized = true;
}

void NES::emulate(double seconds) {
	int cycles = static_cast<int>(CPU_FREQ * seconds + 0.5);
	while (cycles > 0) {
		int cpuCycles = 0;
		CPU* cpu = this->cpu;
		if (cpu->stall > 0) {
			--cpu->stall;
			cpuCycles = 1;
		}
		else {
			uint64_t startCycles = cpu->cycles;

			if (cpu->interrupt == interruptNMI) {
				push16(cpu->PC);
				cpu->php(this, 0, 0);
				cpu->PC = read16(0xFFFA);
				cpu->setI(true);
				cpu->cycles += 7;
			}
			else if (cpu->interrupt == interruptIRQ) {
				push16(cpu->PC);
				cpu->php(this, 0, 0);
				cpu->PC = read16(0xFFFE);
				cpu->setI(true);
				cpu->cycles += 7;
			}
			cpu->interrupt = interruptNone;
			byte opcode = readByte(cpu->PC);
			execute(opcode);
			cpuCycles = static_cast<int>(cpu->cycles - startCycles);
		}

		const int ppuCycles = cpuCycles * 3;
		for (int i = 0; i < ppuCycles; ++i) {
			PPU* ppu = this->ppu;
			ppu->tickPPU(this->cpu, mapper, cartridge);

			if ((ppu->cycle == 280) && (ppu->scanline <= 239 || ppu->scanline >= 261) && (ppu->flag_show_background != 0 || ppu->flag_show_sprites != 0)) {
				this->mapper->updateCounter(this->cpu);
			}
		}

		for (int i = 0; i < cpuCycles; ++i) {
			this->apu->tickAPU(this);
		}
		cycles -= cpuCycles;
	}
}

byte NES::readByte(uint16_t address) {
	if (address < 0x2000) {
		return this->RAM[address & 2047];
	}
	else if (address < 0x4000) {
		return ppu->readPPURegister(0x2000 + (address & 7), mapper, cartridge);
	}
	else if (address == 0x4014) {
		return ppu->readPPURegister(address, mapper, cartridge);
	}
	else if (address == 0x4015) {
		// apu reg read
		byte read_status = 0;
		if (apu->pulse1.length_val > 0) {
			read_status |= 1;
		}
		if (apu->pulse2.length_val > 0) {
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
	}
	else if (address == 0x4016) {
		return controller1->readController();
	}
	else if (address == 0x4017) {
		return controller2->readController();
	}
	else if (address < 0x6000) {
		return 0;
	}
	else if (address >= 0x6000) {
		return this->mapper->read(this->cartridge, address);
	}
	else {
		std::cerr << "ERROR: CPU encountered unrecognized read (address 0x" << std::hex << address << std::dec << ')' << std::endl;
		return 0;
	}
}

// push uint16_t onto stack
void NES::push16(uint16_t value) {
	push(static_cast<byte>(value >> 8));
	push(static_cast<byte>(value));
}

// push byte onto stack
void NES::push(byte value) {
	writeByte(0x100 | static_cast<uint16_t>(cpu->SP), value);
	--cpu->SP;
}

// pop byte from stack
byte NES::pop() {
	++cpu->SP;
	return readByte(0x100 | static_cast<uint16_t>(cpu->SP));
}

// pop uint16_t onto stack
uint16_t NES::pop16() {
	auto lo = pop();
	auto hi = pop();
	return hi << 8 | lo;
}

void dmcRestart(DMC* d) {
	d->cur_addr = d->samp_addr;
	d->cur_len = d->samp_len;
}

// famous 6502 memory indirect jump bug: only the low byte wraps on an xxFF read instead of the whole word incrementing
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
	if (address < 0x2000) {
		this->RAM[address & 2047] = value;
	}
	else if (address < 0x4000) {
        ppu->writeRegisterPPU(0x2000 + (address & 7), value, mapper, cartridge, cpu, this);
	}
	else if (address < 0x4014) {
		this->apu->writeRegisterAPU(address, value);
	}
	else if (address == 0x4014) {
        ppu->writeRegisterPPU(address, value, mapper, cartridge, cpu, this);
	}
	else if (address == 0x4015) {
		this->apu->writeRegisterAPU(address, value);

	class NES;}
	else if (address == 0x4016) {
		controller1->writeController(value);
		controller2->writeController(value);
	}
	else if (address == 0x4017) {
		this->apu->writeRegisterAPU(address, value);
	}
	else if (address < 0x6000) {
		// I/O registers
	}
	else if (address >= 0x6000) {
		this->mapper->write(this->cartridge, address, value);
	}
	else {
		std::cerr << "ERROR: CPU encountered unrecognized write (address 0x" << std::hex << address << std::dec << ')' << std::endl;
	}
}

void NES::execute(byte opcode) {
	const Instruction& instruction = instructions[opcode];

	uint16_t address = 0;
	bool page_crossed = false;
	uint16_t offset;

    switch (instruction.mode) {
        case modeAbsolute:
            address = read16(cpu->PC + 1);
            break;
        case modeAbsoluteX:
            address = read16(cpu->PC + 1) + static_cast<uint16_t>(cpu->X);
            page_crossed = cpu->pagesDiffer(address - static_cast<uint16_t>(cpu->X), address);
            break;
        case modeAbsoluteY:
            address = read16(cpu->PC + 1) + static_cast<uint16_t>(cpu->Y);
            page_crossed = cpu->pagesDiffer(address - static_cast<uint16_t>(cpu->Y), address);
            break;
        case modeAccumulator:
            address = 0;
            break;
        case modeImmediate:
            address = cpu->PC + 1;
            break;
        case modeImplied:
            address = 0;
            break;
        case modeIndexedIndirect:
            address = read16_ff_bug(static_cast<uint16_t>(static_cast<byte>(readByte(cpu->PC + 1) + cpu->X)));
            break;
        case modeIndirect:
            address = read16_ff_bug(read16(cpu->PC + 1));
            break;
        case modeIndirectIndexed:
            address = read16_ff_bug(static_cast<uint16_t>(readByte(cpu->PC + 1))) + static_cast<uint16_t>(cpu->Y);
            page_crossed = cpu->pagesDiffer(address - static_cast<uint16_t>(cpu->Y), address);
            break;
        case modeRelative:
            offset = static_cast<uint16_t>(readByte(cpu->PC + 1));
            address = cpu->PC + 2 + offset - ((offset >= 128) << 8);
            break;
        case modeZeroPage:
            address = static_cast<uint16_t>(readByte(cpu->PC + 1));
            break;
        case modeZeroPageX:
            address = static_cast<uint16_t>(static_cast<byte>(readByte(cpu->PC + 1) + cpu->X));
            break;
        case modeZeroPageY:
            address = static_cast<uint16_t>(static_cast<byte>(readByte(cpu->PC + 1) + cpu->Y));
            break;
        default:
            break;
    }

	cpu->PC += static_cast<uint16_t>(instruction.size);
	cpu->cycles += static_cast<uint64_t>(instruction.cycles);
	if (page_crossed) {
		cpu->cycles += static_cast<uint64_t>(instruction.page_cross_cycles);
	}

	cpu->execute(opcode, this, address, instruction);
}

void NES::printState() {
    std::cout << "CPU status: "
              << "PC=" << cpu->PC <<  std::endl;
    std::cout << "APU status: "
              << "DM=" << apu->dmc.enabled << " P1=" << apu->pulse1.enabled << " P2=" << apu->pulse2.enabled << " TR=" << apu->triangle.enabled<< " NO=" << apu->noise.enabled << std::endl;
    std::cout << "PPU status: "
              << "BG=" << unsigned(ppu->flag_show_background) << " BL=" << unsigned(ppu->flag_show_left_background) << " SP=" << unsigned(ppu->flag_show_sprites) << " SL="<< unsigned(ppu->flag_show_left_sprites) << std::endl;

}


