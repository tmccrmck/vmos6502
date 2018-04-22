#include "nes.h"

constexpr float pulse_tbl[] = { 0.0f, 0.01160913892f, 0.02293948084f, 0.03400094807f, 0.04480300099f, 0.05535465851f, 0.0656645298f, 0.07574082166f, 0.08559139818f, 0.09522374719f, 0.1046450436f, 0.1138621494f, 0.1228816435f, 0.1317097992f, 0.1403526366f, 0.1488159597f, 0.1571052521f, 0.1652258784f, 0.1731829196f, 0.1809812635f, 0.188625589f, 0.1961204559f, 0.2034701705f, 0.2106789351f, 0.2177507579f, 0.2246894985f, 0.2314988673f, 0.2381824702f, 0.2447437793f, 0.2511860728f, 0.2575125694f, 0.2637263834f };
constexpr float tnd_tbl[] = { 0.0f, 0.006699823774f, 0.01334501989f, 0.01993625611f, 0.0264741797f, 0.03295944259f, 0.0393926762f, 0.04577450082f, 0.05210553482f, 0.05838638172f, 0.06461763382f, 0.07079987228f, 0.07693368942f, 0.08301962167f, 0.08905825764f, 0.09505013376f, 0.1009957939f, 0.1068957672f, 0.1127505824f, 0.1185607538f, 0.1243267879f, 0.130049184f, 0.1357284486f, 0.1413650513f, 0.1469594985f, 0.1525122225f, 0.1580237001f, 0.1634943932f, 0.1689247638f, 0.174315244f, 0.1796662807f, 0.1849783063f, 0.1902517378f, 0.1954869777f, 0.2006844729f, 0.2058446258f, 0.210967809f, 0.2160544395f, 0.2211049199f, 0.2261195928f, 0.2310988754f, 0.2360431105f, 0.2409527153f, 0.2458280027f, 0.2506693602f, 0.2554771006f, 0.2602516413f, 0.2649932802f, 0.2697023749f, 0.2743792236f, 0.2790241838f, 0.2836375833f, 0.2882197201f, 0.292770952f, 0.2972915173f, 0.3017818034f, 0.3062421083f, 0.3106726706f, 0.3150738478f, 0.3194458783f, 0.3237891197f, 0.3281037807f, 0.3323901892f, 0.3366486132f, 0.3408792913f, 0.3450825512f, 0.3492586315f, 0.3534077704f, 0.357530266f, 0.3616263568f, 0.3656963408f, 0.3697403669f, 0.3737587631f, 0.3777517378f, 0.3817195594f, 0.3856624365f, 0.3895806372f, 0.3934743702f, 0.3973438442f, 0.4011892974f, 0.4050109982f, 0.4088090658f, 0.412583828f, 0.4163354635f, 0.4200641513f, 0.4237701297f, 0.4274536073f, 0.431114763f, 0.4347538352f, 0.4383709729f, 0.4419664443f, 0.4455403984f, 0.449093014f, 0.4526245296f, 0.4561350644f, 0.4596248865f, 0.4630941153f, 0.4665429294f, 0.4699715674f, 0.4733801484f, 0.4767689407f, 0.4801379442f, 0.4834875166f, 0.4868176877f, 0.4901287258f, 0.4934206903f, 0.4966938794f, 0.4999483228f, 0.5031842589f, 0.5064018369f, 0.5096011758f, 0.5127824545f, 0.5159458518f, 0.5190914273f, 0.5222194791f, 0.5253300667f, 0.5284232497f, 0.5314993262f, 0.5345583558f, 0.5376005173f, 0.5406259298f, 0.5436347723f, 0.5466270447f, 0.549603045f, 0.5525628328f, 0.5555064678f, 0.5584343076f, 0.5613462329f, 0.5642424822f, 0.5671232343f, 0.5699884892f, 0.5728384256f, 0.5756732225f, 0.5784929395f, 0.5812976956f, 0.5840876102f, 0.5868628025f, 0.5896234512f, 0.5923695564f, 0.5951013565f, 0.5978189111f, 0.6005222797f, 0.6032115817f, 0.6058869958f, 0.6085486412f, 0.6111965775f, 0.6138308048f, 0.6164515615f, 0.6190590262f, 0.6216531396f, 0.6242340207f, 0.6268018484f, 0.6293566823f, 0.6318986416f, 0.6344277263f, 0.6369441748f, 0.6394480467f, 0.641939342f, 0.6444182396f, 0.6468848586f, 0.6493391991f, 0.6517813802f, 0.6542115211f, 0.6566297412f, 0.6590360403f, 0.6614305973f, 0.6638134122f, 0.6661846638f, 0.6685443521f, 0.6708925962f, 0.6732294559f, 0.6755550504f, 0.6778694391f, 0.6801727414f, 0.6824649572f, 0.6847462058f, 0.6870166063f, 0.6892762184f, 0.6915250421f, 0.6937633157f, 0.6959909201f, 0.698208034f, 0.7004147768f, 0.7026110888f, 0.7047972083f, 0.7069730759f, 0.7091388106f, 0.7112944722f, 0.7134401202f, 0.7155758739f, 0.7177017927f, 0.7198178768f, 0.7219242454f, 0.7240209579f, 0.7261080146f, 0.7281856537f, 0.7302538157f, 0.7323125601f, 0.7343619466f, 0.7364020944f, 0.7384331226f, 0.7404549122f, 0.7424675822f };
constexpr uint32_t palette[] = { 0xff666666, 0xff882a00, 0xffa71214, 0xffa4003b, 0xff7e005c, 0xff40006e, 0xff00066c, 0xff001d56, 0xff003533, 0xff00480b, 0xff005200, 0xff084f00, 0xff4d4000, 0xff000000, 0xff000000, 0xff000000, 0xffadadad, 0xffd95f15, 0xffff4042, 0xfffe2775, 0xffcc1aa0, 0xff7b1eb7, 0xff2031b5, 0xff004e99, 0xff006d6b, 0xff008738, 0xff00930c, 0xff328f00, 0xff8d7c00, 0xff000000, 0xff000000, 0xff000000, 0xfffffeff, 0xffffb064, 0xffff9092, 0xffff76c6, 0xffff6af3, 0xffcc6efe, 0xff7081fe, 0xff229eea, 0xff00bebc, 0xff00d888, 0xff30e45c, 0xff82e045, 0xffdecd48, 0xff4f4f4f, 0xff000000, 0xff000000, 0xfffffeff, 0xffffdfc0, 0xffffd2d3, 0xffffc8e8, 0xffffc2fb, 0xffeac4fe, 0xffc5ccfe, 0xffa5d8f7, 0xff94e5e4, 0xff96efcf, 0xffabf4bd, 0xffccf3b3, 0xfff2ebb5, 0xffb8b8b8, 0xff000000, 0xff000000 };

constexpr byte duty_tbl[4][8] = {
	{ 0, 1, 0, 0, 0, 0, 0, 0 },
	{ 0, 1, 1, 0, 0, 0, 0, 0 },
	{ 0, 1, 1, 1, 1, 0, 0, 0 },
	{ 1, 0, 0, 1, 1, 1, 1, 1 },
};

constexpr byte tri_tbl[] = {
	15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0,
	0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
};

void spritePixel(PPU* ppu, byte& i, byte& sprite) {
	i = sprite = 0;
	if (ppu->flag_show_sprites == 0) return;
	for (i = 0; i < ppu->sprite_cnt; ++i) {
		int offset = (ppu->cycle - 1) - static_cast<int>(ppu->sprite_pos[i]);
		if (offset < 0 || offset > 7) continue;
		offset = 7 - offset;
		sprite = static_cast<byte>((ppu->sprite_patterns[i] >> static_cast<byte>(offset << 2)) & 0x0F);
		if ((sprite & 3)) return;
	}
	i = sprite = 0;
	return;
}

void tickPPU(NES* nes, CPU* cpu, PPU* ppu) {
	if (ppu->nmi_delay > 0) {
		ppu->nmi_delay--;
		if (ppu->nmi_delay == 0 && ppu->nmi_out && ppu->nmi_occurred) {
			cpu->interrupt = interruptNMI;
		}
	}
	if ((ppu->flag_show_background != 0 || ppu->flag_show_sprites != 0) &&
		ppu->f == 1 && ppu->scanline == 261 && ppu->cycle == 339) {
		ppu->cycle = 0;
		ppu->scanline = 0;
		++ppu->frame;
		ppu->f ^= 1;
	}
	else {
		++ppu->cycle;
		if (ppu->cycle > 340) {
			ppu->cycle = 0;
			++ppu->scanline;
			if (ppu->scanline > 261) {
				ppu->scanline = 0;
				++ppu->frame;
				ppu->f ^= 1;
			}
		}
	}

	const bool do_render = ppu->flag_show_background != 0 || ppu->flag_show_sprites != 0;
	const bool preline = ppu->scanline == 261;
	const bool line_visible = ppu->scanline < 240;
	const bool do_line_render = preline || line_visible;
	const bool prefetch_cycle = ppu->cycle >= 321 && ppu->cycle <= 336;
	const bool cycle_visible = ppu->cycle >= 1 && ppu->cycle <= 256;
	const bool fetch_cycle = prefetch_cycle || cycle_visible;

	if (do_render) {
		if (line_visible && cycle_visible) {
			int x = ppu->cycle - 1;
			int y = ppu->scanline;

			byte background = 0;
			if (ppu->flag_show_background != 0) {
				uint32_t data = static_cast<uint32_t>(ppu->tile_data >> 32) >> ((7 - ppu->x) * 4);
				background = static_cast<byte>(data & 0x0F);
			}

			byte i, sprite;
			spritePixel(ppu, i, sprite);

			if (x < 8 && ppu->flag_show_left_background == 0) {
				background = 0;
			}
			if (x < 8 && ppu->flag_show_left_sprites == 0) {
				sprite = 0;
			}

			const bool b = (background & 3) != 0;
			const bool s = (sprite & 3) != 0;

			byte color = 0;
			if (!b && !s) {
				color = 0;
			}
			else if (!b && s) {
				color = sprite | 0x10;
			}
			else if (b && !s) {
				color = background;
			}
			else {
				if (ppu->sprite_idx[i] == 0 && x < 255) {
					ppu->flag_sprite_zero_hit = 1;
				}
				if (ppu->sprite_priorities[i] == 0) {
					color = sprite | 0x10;
				}
				else {
					color = background;
				}
			}

			ppu->back[(y << 8) + x] = palette[readPalette(ppu, static_cast<uint16_t>(color)) & 63];
		}
		if (do_line_render && fetch_cycle) {
			ppu->tile_data <<= 4;
			int pcm8 = ppu->cycle & 7;
			if (pcm8 == 1) {
				const uint16_t v = ppu->v;
				const uint16_t address = 0x2000 | (v & 0x0FFF);
				ppu->name_tbl_u8 = readPPU(nes, address);
			}
			else if (pcm8 == 3) {
				const uint16_t v = ppu->v;
				const uint16_t address = 0x23C0 | (v & 0x0C00) | ((v >> 4) & 0x38) | ((v >> 2) & 0x07);
				const int shift = ((v >> 4) & 4) | (v & 2);
				ppu->attrib_tbl_u8 = ((readPPU(nes, address) >> shift) & 3) << 2;
			}
			else if (pcm8 == 5) {
				const uint16_t fineY = (ppu->v >> 12) & 7;
				const byte table = ppu->flag_background_tbl;
				const byte tile = ppu->name_tbl_u8;
				const uint16_t address = (static_cast<uint16_t>(table) << 12) + (static_cast<uint16_t>(tile) << 4) + fineY;
				ppu->low_tile_u8 = readPPU(nes, address);
			}
			else if (pcm8 == 7) {
				const uint16_t fineY = (ppu->v >> 12) & 7;
				const byte table = ppu->flag_background_tbl;
				const byte tile = ppu->name_tbl_u8;
				const uint16_t address = (static_cast<uint16_t>(table) << 12) + (static_cast<uint16_t>(tile) << 4) + fineY;
				ppu->high_tile_u8 = readPPU(nes, address + 8);
			}
			else if (pcm8 == 0) {
				uint32_t data = 0;
				for (int i = 0; i < 8; ++i) {
					const byte a = ppu->attrib_tbl_u8;
					const byte p1 = (ppu->low_tile_u8 & 0x80) >> 7;
					const byte p2 = (ppu->high_tile_u8 & 0x80) >> 6;
					ppu->low_tile_u8 <<= 1;
					ppu->high_tile_u8 <<= 1;
					data <<= 4;
					data |= static_cast<uint32_t>(a | p1 | p2);
				}
				ppu->tile_data |= static_cast<uint64_t>(data);
			}
		}
		if (preline && ppu->cycle >= 280 && ppu->cycle <= 304) {
			ppu->v = (ppu->v & 0x841F) | (ppu->t & 0x7BE0);
		}
		if (do_line_render) {
			if (fetch_cycle && (ppu->cycle & 7) == 0) {
				if ((ppu->v & 0x001F) == 31) {
					// coarse X = 0
					ppu->v &= 0xFFE0;
					// switch horizontal nametable
					ppu->v ^= 0x0400;
				}
				else {
					// increment coarse X
					++ppu->v;
				}
			}
			if (ppu->cycle == 256) {
				if ((ppu->v & 0x7000) != 0x7000) {
					ppu->v += 0x1000;
				}
				else {
					ppu->v &= 0x8FFF;
					uint16_t y = (ppu->v & 0x03E0) >> 5;
					if (y == 29) {
						y = 0;
						ppu->v ^= 0x0800;
					}
					else if (y == 31) {
						y = 0;
					}
					else {
						++y;
					}
					ppu->v = (ppu->v & 0xFC1F) | (y << 5);
				}
			}
			if (ppu->cycle == 257) {
				ppu->v = (ppu->v & 0xFBE0) | (ppu->t & 0x041F);
			}
		}
	}

	// sprites
	if (do_render && ppu->cycle == 257) {
		if (line_visible) {
			int h = ppu->flag_sprite_size != 0 ? 16 : 8;
			int count = 0;
			for (int i = 0; i < 64; ++i) {
				const byte y = ppu->oam_tbl[4 * i + 0];
				const byte a = ppu->oam_tbl[4 * i + 2];
				const byte x = ppu->oam_tbl[4 * i + 3];
				int row = ppu->scanline - static_cast<int>(y);
				if (row < 0 || row >= h) continue;
				if (count < 8) {
					uint32_t sprite_pattern = 0;
					byte tile = ppu->oam_tbl[4 * i + 1];
					const byte attributes = ppu->oam_tbl[4 * i + 2];
					uint16_t address = 0;
					if (ppu->flag_sprite_size == 0) {
						if ((attributes & 0x80) == 0x80) {
							row = 7 - row;
						}
						byte table = ppu->flag_sprite_tbl;
						address = (static_cast<uint16_t>(table) << 12) + (static_cast<uint16_t>(tile) << 4) + static_cast<uint16_t>(row);
					}
					else {
						if ((attributes & 0x80) == 0x80) {
							row = 15 - row;
						}
						byte table = tile & 1;
						tile &= 0xFE;
						if (row > 7) {
							++tile;
							row -= 8;
						}
						address = (static_cast<uint16_t>(table) << 12) + (static_cast<uint16_t>(tile) << 4) + static_cast<uint16_t>(row);
					}
					byte atts = (attributes & 3) << 2;
					byte low_tile_u8 = readPPU(nes, address);
					byte high_tile_u8 = readPPU(nes, address + 8);

					for (int j = 0; j < 8; ++j) {
						byte p1, p2;
						if ((attributes & 0x40) == 0x40) {
							p1 = low_tile_u8 & 1;
							p2 = (high_tile_u8 & 1) << 1;
							low_tile_u8 >>= 1;
							high_tile_u8 >>= 1;
						}
						else {
							p1 = (low_tile_u8 & 0x80) >> 7;
							p2 = (high_tile_u8 & 0x80) >> 6;
							low_tile_u8 <<= 1;
							high_tile_u8 <<= 1;
						}
						sprite_pattern <<= 4;
						sprite_pattern |= static_cast<uint32_t>(atts | p1 | p2);
					}
					ppu->sprite_patterns[count] = sprite_pattern;
					ppu->sprite_pos[count] = x;
					ppu->sprite_priorities[count] = (a >> 5) & 1;
					ppu->sprite_idx[count] = static_cast<byte>(i);
				}
				++count;
			}
			if (count > 8) {
				count = 8;
				ppu->flag_sprite_overflow = 1;
			}
			ppu->sprite_cnt = count;
		}
		else {
			ppu->sprite_cnt = 0;
		}
	}

	// v_blank logic
	if (ppu->scanline == 241 && ppu->cycle == 1) {
		// set v_blank
		std::swap(ppu->front, ppu->back);
		ppu->nmi_occurred = true;
		PPUnmiShift(ppu);
	}
	if (preline && ppu->cycle == 1) {
		// clear v_blank
		ppu->nmi_occurred = false;
		PPUnmiShift(ppu);

		ppu->flag_sprite_zero_hit = 0;
		ppu->flag_sprite_overflow = 0;
	}
}

void tickLength(APU* apu) {
	if (apu->pulse1.length_enabled && apu->pulse1.length_val > 0) {
		--apu->pulse1.length_val;
	}
	if (apu->pulse2.length_enabled && apu->pulse2.length_val > 0) {
		--apu->pulse2.length_val;
	}
	if (apu->triangle.length_enabled && apu->triangle.length_val > 0) {
		--apu->triangle.length_val;
	}
	if (apu->noise.length_enabled && apu->noise.length_val > 0) {
		--apu->noise.length_val;
	}
}

void tickPulseTimer(Pulse* p) {
	if (p->timer_val == 0) {
		p->timer_val = p->timer_period;
		p->duty_val = (p->duty_val + 1) & 7;
	}
	else {
		--p->timer_val;
	}
}

void sweep(Pulse* p) {
	const uint16_t delta = p->timer_period >> p->sweep_shift;
	if (p->sweep_negate) {
		p->timer_period -= delta;
		if (p->channel == 1) {
			--p->timer_period;
		}
	}
	else {
		p->timer_period += delta;
	}
}

void pulseTickSweep(Pulse* p) {
	if (p->sweep_reload) {
		if (p->sweep_enabled && p->sweep_val == 0) {
			sweep(p);
		}
		p->sweep_val = p->sweep_period;
		p->sweep_reload = false;
	}
	else if (p->sweep_val > 0) {
		--p->sweep_val;
	}
	else {
		if (p->sweep_enabled) {
			sweep(p);
		}
		p->sweep_val = p->sweep_period;
	}
}

void tickSweep(APU* apu) {
	pulseTickSweep(&apu->pulse1);
	pulseTickSweep(&apu->pulse2);
}

byte pulseOutput(Pulse* p) {
	if (!p->enabled || p->length_val == 0 || duty_tbl[p->duty_mode][p->duty_val] == 0 || p->timer_period < 8 || p->timer_period > 0x7FF) {
		return 0;
	}
	else if (p->envelope_enabled) {
		return p->envelope_vol;
	}
	else {
		return p->const_vol;
	}
}

void triggerIRQ(CPU* cpu) {
	if (getI(cpu) == 0) {
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
			triggerIRQ(cpu);
		}
	}
}

void tickAPU(NES* nes, APU* apu) {
	uint64_t cycle1 = apu->cycle;
	++apu->cycle;
	uint64_t cycle2 = apu->cycle;

	// tick timers
	if ((apu->cycle & 1) == 0) {
		tickPulseTimer(&apu->pulse1);
		tickPulseTimer(&apu->pulse2);

		Noise* n = &apu->noise;
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

		DMC* d = &apu->dmc;
		if (d->enabled) {
			// tick reader
			if (d->cur_len > 0 && d->bit_count == 0) {
				nes->cpu->stall += 4;
				d->shift_reg = readByte(nes, d->cur_addr);
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

	Triangle* t = &apu->triangle;
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
		const byte fp = apu->frame_period;
		if (fp == 4) {
			apu->frame_val = (apu->frame_val + 1) & 3;
				switch (apu->frame_val) {
				case 0:
				case 2:
					tickEnvelope(apu);
					break;
				case 1:
					tickEnvelope(apu);
					tickSweep(apu);
					tickLength(apu);
					break;
				case 3:
					tickEnvelope(apu);
					tickSweep(apu);
					tickLength(apu);
					if (apu->frame_IRQ) {
						triggerIRQ(nes->cpu);
					}
					break;
				}
		}
		else if (fp == 5) {
			apu->frame_val = (apu->frame_val + 1) % 5;
			switch (apu->frame_val) {
			case 1:
			case 3:
				tickEnvelope(apu);
				break;
			case 0:
			case 2:
				tickEnvelope(apu);
				tickSweep(apu);
				tickLength(apu);
				break;
			}
		}
	}

	const int s1 = static_cast<int>(static_cast<double>(cycle1) / SAMPLE_RATE);
	const int s2 = static_cast<int>(static_cast<double>(cycle2) / SAMPLE_RATE);

	if (s1 != s2) {
		const byte p1_output = pulseOutput(&apu->pulse1);
		const byte p2_output = pulseOutput(&apu->pulse2);

		const byte tri_output = (!t->enabled || t->length_val == 0 || t->counter_val == 0) ? 0 : tri_tbl[t->duty_val];

		Noise* n = &apu->noise;
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

		const byte dOut = apu->dmc.value;

		// combined outputs
		float output[2];
		output[1] = output[0] = tnd_tbl[(3 * tri_output) + (2 * noise_out) + dOut] + pulse_tbl[p1_output + p2_output];

		if (Pa_GetStreamWriteAvailable(apu->stream)) Pa_WriteStream(apu->stream, output, 1);
	}
}

void emulate(NES* nes, double seconds) {
	int cycles = static_cast<int>(CPU_FREQ * seconds + 0.5);
	while (cycles > 0) {
		int cpuCycles = 0;
		CPU* cpu = nes->cpu;
		if (cpu->stall > 0) {
			--cpu->stall;
			cpuCycles = 1;
		}
		else {
			uint64_t startCycles = cpu->cycles;

			if (cpu->interrupt == interruptNMI) {
				push16(nes, cpu->PC);
				php(cpu, nes, 0, 0);
				cpu->PC = read16(nes, 0xFFFA);
				setI(cpu, true);
				cpu->cycles += 7;
			}
			else if (cpu->interrupt == interruptIRQ) {
				push16(nes, cpu->PC);
				php(cpu, nes, 0, 0);
				cpu->PC = read16(nes, 0xFFFE);
				setI(cpu, true);
				cpu->cycles += 7;
			}
			cpu->interrupt = interruptNone;
			byte opcode = readByte(nes, cpu->PC);
			execute(nes, opcode);
			cpuCycles = static_cast<int>(cpu->cycles - startCycles);
		}

		const int ppuCycles = cpuCycles * 3;
		for (int i = 0; i < ppuCycles; ++i) {
			PPU* ppu = nes->ppu;
			tickPPU(nes, nes->cpu, ppu);

			if ((ppu->cycle == 280) && (ppu->scanline <= 239 || ppu->scanline >= 261) && (ppu->flag_show_background != 0 || ppu->flag_show_sprites != 0)) {
				nes->mapper->updateCounter(nes->cpu);
			}
		}

		for (int i = 0; i < cpuCycles; ++i) {
			tickAPU(nes, nes->apu);
		}
		cycles -= cpuCycles;
	}
}

void PPUnmiShift(PPU* ppu) {
	const bool nmi = ppu->nmi_out && ppu->nmi_occurred;
	if (nmi && !ppu->nmi_last) {
		// TODO: I think this is supposed to be 8...
		ppu->nmi_delay = 15;
	}
	ppu->nmi_last = nmi;
}

void dmcRestart(DMC* d) {
	d->cur_addr = d->samp_addr;
	d->cur_len = d->samp_len;
}
