#include "nes.h"

constexpr byte length_tbl[] = {
	10, 254, 20, 2, 40, 4, 80, 6, 160, 8, 60, 10, 14, 12, 26, 14,
	12, 16, 24, 18, 48, 20, 96, 22, 192, 24, 72, 26, 16, 28, 32, 30
};

constexpr byte dmc_tbl[] = {
	214, 190, 170, 160, 143, 127, 113, 107, 95, 80, 71, 64, 53, 42, 36, 27
};

constexpr uint16_t noise_tbl[] = {
	4, 8, 16, 32, 64, 96, 128, 160, 202, 254, 380, 508, 762, 1016, 2034, 4068
};

constexpr uint16_t mirror_tbl[5][4] = {
	{ 0, 0, 1, 1 },
	{ 0, 1, 0, 1 },
	{ 0, 0, 0, 0 },
	{ 1, 1, 1, 1 },
	{ 0, 1, 2, 3 }
};

byte readController(Controller* c) {
	const byte value = (c->index < 8 && ((c->buttons >> c->index) & 1));
	++c->index;
	if ((c->strobe & 1) == 1 ){
		c->index = 0;
	}
	return value;
}

void writeController(Controller* c, byte value) {
	c->strobe = value;
	if ((c->strobe & 1) == 1) {
		c->index = 0;
	}
}

void pulseWriteControl(Pulse* p, byte value) {
	p->duty_mode = (value >> 6) & 3;
	p->length_enabled = ((value >> 5) & 1) == 0;
	p->envelope_loop = ((value >> 5) & 1) == 1;
	p->envelope_enabled = ((value >> 4) & 1) == 0;
	p->envelope_period = value & 15;
	p->const_vol = value & 15;
	p->envelope_start = true;
}

void pulseWriteSweep(Pulse* p, byte value) {
	p->sweep_enabled = ((value >> 7) & 1) == 1;
	p->sweep_period = ((value >> 4) & 7) + 1;
	p->sweep_negate = ((value >> 3) & 1) == 1;
	p->sweep_shift = value & 7;
	p->sweep_reload = true;
}

void pulseWriteTimerHigh(Pulse* p, byte value) {
	p->length_val = length_tbl[value >> 3];
	p->timer_period = (p->timer_period & 0x00FF) | (static_cast<uint16_t>(value & 7) << 8);
	p->envelope_start = true;
	p->duty_val = 0;
}

void writeRegisterAPU(APU* apu, uint16_t address, byte value) {
	switch (address) {
	case 0x4000:
		pulseWriteControl(&apu->pulse1, value);
		break;
	case 0x4001:
		pulseWriteSweep(&apu->pulse1, value);
		break;
	case 0x4002:
		apu->pulse1.timer_period = (apu->pulse1.timer_period & 0xFF00) | static_cast<uint16_t>(value);
		break;
	case 0x4003:
		pulseWriteTimerHigh(&apu->pulse1, value);
		break;
	case 0x4004:
		pulseWriteControl(&apu->pulse2, value);
		break;
	case 0x4005:
		pulseWriteSweep(&apu->pulse2, value);
		break;
	case 0x4006:
		apu->pulse2.timer_period = (apu->pulse2.timer_period & 0xFF00) | static_cast<uint16_t>(value);
		break;
	case 0x4007:
		pulseWriteTimerHigh(&apu->pulse2, value);
		break;
	case 0x4008:
		apu->triangle.length_enabled = ((value >> 7) & 1) == 0;
		apu->triangle.counter_period = value & 0x7F;
		break;
	case 0x4009:
	case 0x4010:
		apu->dmc.irq = (value & 0x80) == 0x80;
		apu->dmc.loop = (value & 0x40) == 0x40;
		apu->dmc.tick_period = dmc_tbl[value & 0x0F];
		break;
	case 0x4011:
		apu->dmc.value = value & 0x7F;
		break;
	case 0x4012:
		apu->dmc.samp_addr = 0xC000 | (static_cast<uint16_t>(value) << 6);
		break;
	case 0x4013:
		apu->dmc.samp_len = (static_cast<uint16_t>(value) << 4) | 1;
		break;
	case 0x400A:
		apu->triangle.timer_period = (apu->triangle.timer_period & 0xFF00) | static_cast<uint16_t>(value);
		break;
	case 0x400B:
		apu->triangle.length_val = length_tbl[value >> 3];
		apu->triangle.timer_period = (apu->triangle.timer_period & 0x00FF) | (static_cast<uint16_t>(value & 7) << 8);
		apu->triangle.timer_val = apu->triangle.timer_period;
		apu->triangle.counter_reload = true;
		break;
	case 0x400C:
		apu->noise.length_enabled = ((value >> 5) & 1) == 0;
		apu->noise.envelope_loop = ((value >> 5) & 1) == 1;
		apu->noise.envelope_enabled = ((value >> 4) & 1) == 0;
		apu->noise.envelope_period = value & 15;
		apu->noise.const_vol = value & 15;
		apu->noise.envelope_start = true;
		break;
	case 0x400D:
	case 0x400E:
		apu->noise.mode = (value & 0x80) == 0x80;
		apu->noise.timer_period = noise_tbl[value & 0x0F];
		break;
	case 0x400F:
		apu->noise.length_val = length_tbl[value >> 3];
		apu->noise.envelope_start = true;
		break;
	case 0x4015:
		apu->pulse1.enabled = (value & 1) == 1;
		apu->pulse2.enabled = (value & 2) == 2;
		apu->triangle.enabled = (value & 4) == 4;
		apu->noise.enabled = (value & 8) == 8;
		apu->dmc.enabled = (value & 16) == 16;
		if (!apu->pulse1.enabled) {
			apu->pulse1.length_val = 0;
		}
		if (!apu->pulse2.enabled) {
			apu->pulse2.length_val = 0;
		}
		if (!apu->triangle.enabled) {
			apu->triangle.length_val = 0;
		}
		if (!apu->noise.enabled) {
			apu->noise.length_val = 0;
		}
		if (!apu->dmc.enabled) {
			apu->dmc.cur_len = 0;
		}
		else {
			if (apu->dmc.cur_len == 0) {
				dmcRestart(&apu->dmc);
			}
		}
		break;
	case 0x4017:
		apu->frame_period = 4 + ((value >> 7) & 1);
		apu->frame_IRQ = ((value >> 6) & 1) == 0;
		if (apu->frame_period == 5) {
			tickEnvelope(apu);
			tickSweep(apu);
			tickLength(apu);
		}
		break;
	}
}

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

uint16_t mirrorAddress(byte mode, uint16_t address) {
	address = (address - 0x2000) & 4095;
	const uint16_t table = address >> 10;
	const uint16_t offset = address & 1023;
	return 0x2000 + (mirror_tbl[mode][table] << 10) + offset;
}

