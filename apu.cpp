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

void pulseTickEnvelope(Pulse* p) {
	if (p->envelope_start) {
		p->envelope_vol = 15;
		p->envelope_val = p->envelope_period;
		p->envelope_start = false;
	}
	else if (p->envelope_val > 0) {
		--p->envelope_val;
	}
	else {
		if (p->envelope_vol > 0) {
			--p->envelope_vol;
		}
		else if (p->envelope_loop) {
			p->envelope_vol = 15;
		}
		p->envelope_val = p->envelope_period;
	}
}

void Pulse::tickPulseTimer() {
	if (this->timer_val == 0) {
		this->timer_val = this->timer_period;
		this->duty_val = (this->duty_val + 1) & 7;
	}
	else {
		--this->timer_val;
	}
}

void Pulse::sweep() {
	const uint16_t delta = this->timer_period >> this->sweep_shift;
	if (this->sweep_negate) {
		this->timer_period -= delta;
		if (this->channel == 1) {
			--this->timer_period;
		}
	}
	else {
		this->timer_period += delta;
	}
}

void Pulse::pulseTickSweep() {
	if (this->sweep_reload) {
		if (this->sweep_enabled && this->sweep_val == 0) {
			sweep();
		}
		this->sweep_val = this->sweep_period;
		this->sweep_reload = false;
	}
	else if (this->sweep_val > 0) {
		--this->sweep_val;
	}
	else {
		if (this->sweep_enabled) {
			sweep();
		}
		this->sweep_val = this->sweep_period;
	}
}

byte Pulse::pulseOutput() {
	if (!this->enabled || this->length_val == 0 || duty_tbl[this->duty_mode][this->duty_val] == 0 || this->timer_period < 8 || this->timer_period > 0x7FF) {
		return 0;
	}
	else if (this->envelope_enabled) {
		return this->envelope_vol;
	}
	else {
		return this->const_vol;
	}
}

void Pulse::pulseWriteControl(byte value) {
	this->duty_mode = (value >> 6) & 3;
	this->length_enabled = ((value >> 5) & 1) == 0;
	this->envelope_loop = ((value >> 5) & 1) == 1;
	this->envelope_enabled = ((value >> 4) & 1) == 0;
	this->envelope_period = value & 15;
	this->const_vol = value & 15;
	this->envelope_start = true;
}

void Pulse::pulseWriteSweep(byte value) {
	this->sweep_enabled = ((value >> 7) & 1) == 1;
	this->sweep_period = ((value >> 4) & 7) + 1;
	this->sweep_negate = ((value >> 3) & 1) == 1;
	this->sweep_shift = value & 7;
	this->sweep_reload = true;
}

void Pulse::pulseWriteTimerHigh(byte value) {
	this->length_val = length_tbl[value >> 3];
	this->timer_period = (this->timer_period & 0x00FF) | (static_cast<uint16_t>(value & 7) << 8);
	this->envelope_start = true;
	this->duty_val = 0;
}

void APU::tickLength() {
	if (this->pulse1.length_enabled && this->pulse1.length_val > 0) {
		--this->pulse1.length_val;
	}
	if (this->pulse2.length_enabled && this->pulse2.length_val > 0) {
		--this->pulse2.length_val;
	}
	if (this->triangle.length_enabled && this->triangle.length_val > 0) {
		--this->triangle.length_val;
	}
	if (this->noise.length_enabled && this->noise.length_val > 0) {
		--this->noise.length_val;
	}
}

void APU::tickSweep() {
	pulse1.pulseTickSweep();
	pulse2.pulseTickSweep();
}

void APU::tickEnvelope() {
	pulseTickEnvelope(&this->pulse1);
	pulseTickEnvelope(&this->pulse2);

	Triangle* t = &this->triangle;
	if (t->counter_reload) {
		t->counter_val = t->counter_period;
	}
	else if (t->counter_val > 0) {
		--t->counter_val;
	}
	if (t->length_enabled) {
		t->counter_reload = false;
	}

	Noise* n = &this->noise;
	if (n->envelope_start) {
		n->envelope_vol = 15;
		n->envelope_val = n->envelope_period;
		n->envelope_start = false;
	}
	else if (n->envelope_val > 0) {
		--n->envelope_val;
	}
	else {
		if (n->envelope_vol > 0) {
			--n->envelope_vol;
		}
		else if (n->envelope_loop) {
			n->envelope_vol = 15;
		}
		n->envelope_val = n->envelope_period;
	}
}

void APU::writeRegisterAPU(uint16_t address, byte value) {
	switch (address) {
		case 0x4000:
			this->pulse1.pulseWriteControl(value);
			break;
		case 0x4001:
			this->pulse1.pulseWriteSweep(value);
			break;
		case 0x4002:
			this->pulse1.timer_period = (this->pulse1.timer_period & 0xFF00) | static_cast<uint16_t>(value);
			break;
		case 0x4003:
			this->pulse1.pulseWriteTimerHigh(value);
			break;
		case 0x4004:
			this->pulse2.pulseWriteControl(value);
			break;
		case 0x4005:
			this->pulse2.pulseWriteSweep(value);
			break;
		case 0x4006:
			this->pulse2.timer_period = (this->pulse2.timer_period & 0xFF00) | static_cast<uint16_t>(value);
			break;
		case 0x4007:
			this->pulse2.pulseWriteTimerHigh(value);
			break;
		case 0x4008:
			this->triangle.length_enabled = ((value >> 7) & 1) == 0;
			this->triangle.counter_period = value & 0x7F;
			break;
		case 0x4009:
		case 0x4010:
			this->dmc.irq = (value & 0x80) == 0x80;
			this->dmc.loop = (value & 0x40) == 0x40;
			this->dmc.tick_period = dmc_tbl[value & 0x0F];
			break;
		case 0x4011:
			this->dmc.value = value & 0x7F;
			break;
		case 0x4012:
			this->dmc.samp_addr = 0xC000 | (static_cast<uint16_t>(value) << 6);
			break;
		case 0x4013:
			this->dmc.samp_len = (static_cast<uint16_t>(value) << 4) | 1;
			break;
		case 0x400A:
			this->triangle.timer_period = (this->triangle.timer_period & 0xFF00) | static_cast<uint16_t>(value);
			break;
		case 0x400B:
			this->triangle.length_val = length_tbl[value >> 3];
			this->triangle.timer_period = (this->triangle.timer_period & 0x00FF) | (static_cast<uint16_t>(value & 7) << 8);
			this->triangle.timer_val = this->triangle.timer_period;
			this->triangle.counter_reload = true;
			break;
		case 0x400C:
			this->noise.length_enabled = ((value >> 5) & 1) == 0;
			this->noise.envelope_loop = ((value >> 5) & 1) == 1;
			this->noise.envelope_enabled = ((value >> 4) & 1) == 0;
			this->noise.envelope_period = value & 15;
			this->noise.const_vol = value & 15;
			this->noise.envelope_start = true;
			break;
		case 0x400D:
		case 0x400E:
			this->noise.mode = (value & 0x80) == 0x80;
			this->noise.timer_period = noise_tbl[value & 0x0F];
			break;
		case 0x400F:
			this->noise.length_val = length_tbl[value >> 3];
			this->noise.envelope_start = true;
			break;
		case 0x4015:
			this->pulse1.enabled = (value & 1) == 1;
			this->pulse2.enabled = (value & 2) == 2;
			this->triangle.enabled = (value & 4) == 4;
			this->noise.enabled = (value & 8) == 8;
			this->dmc.enabled = (value & 16) == 16;
			if (!this->pulse1.enabled) {
				this->pulse1.length_val = 0;
			}
			if (!this->pulse2.enabled) {
				this->pulse2.length_val = 0;
			}
			if (!this->triangle.enabled) {
				this->triangle.length_val = 0;
			}
			if (!this->noise.enabled) {
				this->noise.length_val = 0;
			}
			if (!this->dmc.enabled) {
				this->dmc.cur_len = 0;
			}
			else {
				if (this->dmc.cur_len == 0) {
					dmcRestart(&this->dmc);
				}
			}
			break;
		case 0x4017:
			this->frame_period = 4 + ((value >> 7) & 1);
			this->frame_IRQ = ((value >> 6) & 1) == 0;
			if (this->frame_period == 5) {
				this->tickEnvelope();
				this->tickSweep();
				this->tickLength();
			}
			break;
	}
}
