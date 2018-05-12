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

constexpr float pulse_tbl[] = { 0.0f, 0.01160913892f, 0.02293948084f, 0.03400094807f, 0.04480300099f, 0.05535465851f, 0.0656645298f, 0.07574082166f, 0.08559139818f, 0.09522374719f, 0.1046450436f, 0.1138621494f, 0.1228816435f, 0.1317097992f, 0.1403526366f, 0.1488159597f, 0.1571052521f, 0.1652258784f, 0.1731829196f, 0.1809812635f, 0.188625589f, 0.1961204559f, 0.2034701705f, 0.2106789351f, 0.2177507579f, 0.2246894985f, 0.2314988673f, 0.2381824702f, 0.2447437793f, 0.2511860728f, 0.2575125694f, 0.2637263834f };
constexpr float tnd_tbl[] = { 0.0f, 0.006699823774f, 0.01334501989f, 0.01993625611f, 0.0264741797f, 0.03295944259f, 0.0393926762f, 0.04577450082f, 0.05210553482f, 0.05838638172f, 0.06461763382f, 0.07079987228f, 0.07693368942f, 0.08301962167f, 0.08905825764f, 0.09505013376f, 0.1009957939f, 0.1068957672f, 0.1127505824f, 0.1185607538f, 0.1243267879f, 0.130049184f, 0.1357284486f, 0.1413650513f, 0.1469594985f, 0.1525122225f, 0.1580237001f, 0.1634943932f, 0.1689247638f, 0.174315244f, 0.1796662807f, 0.1849783063f, 0.1902517378f, 0.1954869777f, 0.2006844729f, 0.2058446258f, 0.210967809f, 0.2160544395f, 0.2211049199f, 0.2261195928f, 0.2310988754f, 0.2360431105f, 0.2409527153f, 0.2458280027f, 0.2506693602f, 0.2554771006f, 0.2602516413f, 0.2649932802f, 0.2697023749f, 0.2743792236f, 0.2790241838f, 0.2836375833f, 0.2882197201f, 0.292770952f, 0.2972915173f, 0.3017818034f, 0.3062421083f, 0.3106726706f, 0.3150738478f, 0.3194458783f, 0.3237891197f, 0.3281037807f, 0.3323901892f, 0.3366486132f, 0.3408792913f, 0.3450825512f, 0.3492586315f, 0.3534077704f, 0.357530266f, 0.3616263568f, 0.3656963408f, 0.3697403669f, 0.3737587631f, 0.3777517378f, 0.3817195594f, 0.3856624365f, 0.3895806372f, 0.3934743702f, 0.3973438442f, 0.4011892974f, 0.4050109982f, 0.4088090658f, 0.412583828f, 0.4163354635f, 0.4200641513f, 0.4237701297f, 0.4274536073f, 0.431114763f, 0.4347538352f, 0.4383709729f, 0.4419664443f, 0.4455403984f, 0.449093014f, 0.4526245296f, 0.4561350644f, 0.4596248865f, 0.4630941153f, 0.4665429294f, 0.4699715674f, 0.4733801484f, 0.4767689407f, 0.4801379442f, 0.4834875166f, 0.4868176877f, 0.4901287258f, 0.4934206903f, 0.4966938794f, 0.4999483228f, 0.5031842589f, 0.5064018369f, 0.5096011758f, 0.5127824545f, 0.5159458518f, 0.5190914273f, 0.5222194791f, 0.5253300667f, 0.5284232497f, 0.5314993262f, 0.5345583558f, 0.5376005173f, 0.5406259298f, 0.5436347723f, 0.5466270447f, 0.549603045f, 0.5525628328f, 0.5555064678f, 0.5584343076f, 0.5613462329f, 0.5642424822f, 0.5671232343f, 0.5699884892f, 0.5728384256f, 0.5756732225f, 0.5784929395f, 0.5812976956f, 0.5840876102f, 0.5868628025f, 0.5896234512f, 0.5923695564f, 0.5951013565f, 0.5978189111f, 0.6005222797f, 0.6032115817f, 0.6058869958f, 0.6085486412f, 0.6111965775f, 0.6138308048f, 0.6164515615f, 0.6190590262f, 0.6216531396f, 0.6242340207f, 0.6268018484f, 0.6293566823f, 0.6318986416f, 0.6344277263f, 0.6369441748f, 0.6394480467f, 0.641939342f, 0.6444182396f, 0.6468848586f, 0.6493391991f, 0.6517813802f, 0.6542115211f, 0.6566297412f, 0.6590360403f, 0.6614305973f, 0.6638134122f, 0.6661846638f, 0.6685443521f, 0.6708925962f, 0.6732294559f, 0.6755550504f, 0.6778694391f, 0.6801727414f, 0.6824649572f, 0.6847462058f, 0.6870166063f, 0.6892762184f, 0.6915250421f, 0.6937633157f, 0.6959909201f, 0.698208034f, 0.7004147768f, 0.7026110888f, 0.7047972083f, 0.7069730759f, 0.7091388106f, 0.7112944722f, 0.7134401202f, 0.7155758739f, 0.7177017927f, 0.7198178768f, 0.7219242454f, 0.7240209579f, 0.7261080146f, 0.7281856537f, 0.7302538157f, 0.7323125601f, 0.7343619466f, 0.7364020944f, 0.7384331226f, 0.7404549122f, 0.7424675822f };

constexpr byte tri_tbl[] = {
        15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0,
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
};

constexpr byte duty_tbl[4][8] = {
	{ 0, 1, 0, 0, 0, 0, 0, 0 },
	{ 0, 1, 1, 0, 0, 0, 0, 0 },
	{ 0, 1, 1, 1, 1, 0, 0, 0 },
	{ 1, 0, 0, 1, 1, 1, 1, 1 },
};

// note use getI
void triggerIRQ(CPU<NES>* cpu) {
	if ((cpu->flags & 4) >> 2 == 0) {
		cpu->interrupt = interruptIRQ;
	}
}

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

void APU::tickAPU(NES* nes) {
	uint64_t cycle1 = this->cycle;
	++this->cycle;
	uint64_t cycle2 = this->cycle;

	// tick timers
	if ((this->cycle & 1) == 0) {
		this->pulse1.tickPulseTimer();
		this->pulse2.tickPulseTimer();

		Noise* n = &this->noise;
		if (n->timer_val == 0) {
			n->timer_val = n->timer_period;
			byte shift = n->mode ? 6 : 1;
			uint16_t b1 = n->shift_reg & 1;
			uint16_t b2 = (n->shift_reg >> shift) & 1;
			n->shift_reg >>= 1;
			n->shift_reg |= (b1 ^ b2) << 14;
		}
		else {
			--n->timer_val;
		}

		DMC* d = &this->dmc;
		if (d->enabled) {
			// tick reader
			if (d->cur_len > 0 && d->bit_count == 0) {
				nes->cpu->stall += 4;
				d->shift_reg = nes->readByte(d->cur_addr);
				d->bit_count = 8;
				++d->cur_addr;
				if (d->cur_addr == 0) {
					d->cur_addr = 0x8000;
				}
				--d->cur_len;
				if (d->cur_len == 0 && d->loop) {
					dmcRestart(d);
				}
			}

			if (d->tick_val == 0) {
				d->tick_val = d->tick_period;

				// tick shifter
				if (d->bit_count != 0) {
					if ((d->shift_reg & 1) == 1) {
						if (d->value <= 125) {
							d->value += 2;
						}
					}
					else {
						if (d->value >= 2) {
							d->value -= 2;
						}
					}
					d->shift_reg >>= 1;
					--d->bit_count;
				}
			}
			else {
				--d->tick_val;
			}
		}
	}

	Triangle* t = &this->triangle;
	if (t->timer_val == 0) {
		t->timer_val = t->timer_period;
			if (t->length_val > 0 && t->counter_val > 0) {
				t->duty_val = (t->duty_val + 1) & 31;
			}
		}
		else {
			--t->timer_val;
		}

	const int f1 = static_cast<int>(static_cast<double>(cycle1) / FRAME_CTR_FREQ);
	const int f2 = static_cast<int>(static_cast<double>(cycle2) / FRAME_CTR_FREQ);
	if (f1 != f2) {
		const byte fp = this->frame_period;
		if (fp == 4) {
			this->frame_val = (this->frame_val + 1) & 3;
				switch (this->frame_val) {
				case 0:
				case 2:
					this->tickEnvelope();
					break;
				case 1:
					this->tickEnvelope();
					this->tickSweep();
					this->tickLength();
					break;
				case 3:
					this->tickEnvelope();
					this->tickSweep();
					this->tickLength();
					if (this->frame_IRQ) {
						triggerIRQ(nes->cpu);
					}
					break;
				}
		}
		else if (fp == 5) {
			this->frame_val = (this->frame_val + 1) % 5;
			switch (this->frame_val) {
			case 1:
			case 3:
				this->tickEnvelope();
				break;
			case 0:
			case 2:
				this->tickEnvelope();
				this->tickSweep();
				this->tickLength();
				break;
			}
		}
	}

	const int s1 = static_cast<int>(static_cast<double>(cycle1) / SAMPLE_RATE);
	const int s2 = static_cast<int>(static_cast<double>(cycle2) / SAMPLE_RATE);

	if (s1 != s2) {
		const byte p1_output = this->pulse1.pulseOutput();
		const byte p2_output = this->pulse2.pulseOutput();

		const byte tri_output = (!t->enabled || t->length_val == 0 || t->counter_val == 0) ? 0 : tri_tbl[t->duty_val];

		Noise* n = &this->noise;
		byte noise_out;
		if (!n->enabled || n->length_val == 0 || (n->shift_reg & 1) == 1) {
			noise_out = 0;
		}
		else if (n->envelope_enabled) {
			noise_out = n->envelope_vol;
		}
		else {
			noise_out = n->const_vol;
		}

		const byte dOut = this->dmc.value;

		// combined outputs
		float output[2];
		output[1] = output[0] = tnd_tbl[(3 * tri_output) + (2 * noise_out) + dOut] + pulse_tbl[p1_output + p2_output];

		if (Pa_GetStreamWriteAvailable(this->stream)) Pa_WriteStream(this->stream, output, 1);
	}
}

APU::APU() : cycle(0), frame_period(0), frame_val(0), frame_IRQ(false) {}

