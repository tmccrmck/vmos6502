#include "ppu.h"

uint16_t mirrorAddress(byte mode, uint16_t address) {
	address = (address - 0x2000) & 4095;
	const uint16_t table = address >> 10;
	const uint16_t offset = address & 1023;
	return 0x2000 + (mirror_tbl[mode][table] << 10) + offset;
}

void PPU::spritePixel(byte& i, byte& sprite) {
	i = sprite = 0;
	if (this->flag_show_sprites == 0) return;
	for (i = 0; i < this->sprite_cnt; ++i) {
		int offset = (this->cycle - 1) - static_cast<int>(this->sprite_pos[i]);
		if (offset < 0 || offset > 7) continue;
		offset = 7 - offset;
		sprite = static_cast<byte>((this->sprite_patterns[i] >> static_cast<byte>(offset << 2)) & 0x0F);
		if ((sprite & 3)) return;
	}
	i = sprite = 0;
	return;
}

void PPU::writePPUCtrl(byte value) {
	this->flag_name_tbl = value & 3;
	this->flag_increment = (value >> 2) & 1;
	this->flag_sprite_tbl = (value >> 3) & 1;
	this->flag_background_tbl = (value >> 4) & 1;
	this->flag_sprite_size = (value >> 5) & 1;
	this->flag_rw = (value >> 6) & 1;
	this->nmi_out = ((value >> 7) & 1) == 1;
	this->PPUnmiShift();
	this->t = (this->t & 0xF3FF) | ((static_cast<uint16_t>(value) & 3) << 10);
}

// $2001: PPUMASK
void PPU::writePPUMask(byte value) {
	this->flag_gray = value & 1;
	this->flag_show_left_background = (value >> 1) & 1;
	this->flag_show_left_sprites = (value >> 2) & 1;
	this->flag_show_background = (value >> 3) & 1;
	this->flag_show_sprites = (value >> 4) & 1;
	this->flag_red_tint = (value >> 5) & 1;
	this->flag_green_tint = (value >> 6) & 1;
	this->flag_blue_tint = (value >> 7) & 1;
}

void PPU::PPUnmiShift() {
	const bool nmi = this->nmi_out && this->nmi_occurred;
	if (nmi && !this->nmi_last) {
		// TODO: I think this is supposed to be 8...
		this->nmi_delay = 15;
	}
	this->nmi_last = nmi;
}

byte PPU::readPalette(uint16_t address) {
	if (address >= 16 && (address & 3) == 0) {
		address -= 16;
	}
	return this->palette_tbl[address];
}

byte PPU::readPPU(uint16_t address, Mapper* mapper, Cartridge* cartridge) {
	address &= 16383;
	if (address < 0x2000) {
		return mapper->read(cartridge, address);
	}
	else if (address < 0x3F00) {
		byte mode = cartridge->mirror;
		return this->name_tbl[mirrorAddress(mode, address) & 2047];
	}
	else if (address < 0x4000) {
		return this->readPalette(address & 31);
	}
	else {
		std::cerr << "ERROR: PPU encountered unrecognized read (address 0x" << std::hex << address << std::dec << ')' << std::endl;
	}
	return 0;
}

void PPU::tickPPU(CPU<NES>* cpu, Mapper* mapper, Cartridge* cartridge) {
	if (this->nmi_delay > 0) {
		this->nmi_delay--;
		if (this->nmi_delay == 0 && this->nmi_out && this->nmi_occurred) {
			cpu->interrupt = interruptNMI;
		}
	}
	if ((this->flag_show_background != 0 || this->flag_show_sprites != 0) &&
		this->f == 1 && this->scanline == 261 && this->cycle == 339) {
		this->cycle = 0;
		this->scanline = 0;
		++this->frame;
		this->f ^= 1;
	}
	else {
		++this->cycle;
		if (this->cycle > 340) {
			this->cycle = 0;
			++this->scanline;
			if (this->scanline > 261) {
				this->scanline = 0;
				++this->frame;
				this->f ^= 1;
			}
		}
	}

	const bool do_render = this->flag_show_background != 0 || this->flag_show_sprites != 0;
	const bool preline = this->scanline == 261;
	const bool line_visible = this->scanline < 240;
	const bool do_line_render = preline || line_visible;
	const bool prefetch_cycle = this->cycle >= 321 && this->cycle <= 336;
	const bool cycle_visible = this->cycle >= 1 && this->cycle <= 256;
	const bool fetch_cycle = prefetch_cycle || cycle_visible;

	if (do_render) {
		if (line_visible && cycle_visible) {
			int x = this->cycle - 1;
			int y = this->scanline;

			byte background = 0;
			if (this->flag_show_background != 0) {
				uint32_t data = static_cast<uint32_t>(this->tile_data >> 32) >> ((7 - this->x) * 4);
				background = static_cast<byte>(data & 0x0F);
			}

			byte i, sprite;
			spritePixel(i, sprite);

			if (x < 8 && this->flag_show_left_background == 0) {
				background = 0;
			}
			if (x < 8 && this->flag_show_left_sprites == 0) {
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
				if (this->sprite_idx[i] == 0 && x < 255) {
					this->flag_sprite_zero_hit = 1;
				}
				if (this->sprite_priorities[i] == 0) {
					color = sprite | 0x10;
				}
				else {
					color = background;
				}
			}

			this->back[(y << 8) + x] = palette[this->readPalette(static_cast<uint16_t>(color)) & 63];
		}
		if (do_line_render && fetch_cycle) {
			this->tile_data <<= 4;
			int pcm8 = this->cycle & 7;
			if (pcm8 == 1) {
				const uint16_t v = this->v;
				const uint16_t address = 0x2000 | (v & 0x0FFF);
				this->name_tbl_u8 = readPPU(address, mapper, cartridge);
			}
			else if (pcm8 == 3) {
				const uint16_t v = this->v;
				const uint16_t address = 0x23C0 | (v & 0x0C00) | ((v >> 4) & 0x38) | ((v >> 2) & 0x07);
				const int shift = ((v >> 4) & 4) | (v & 2);
				this->attrib_tbl_u8 = ((readPPU(address, mapper, cartridge) >> shift) & 3) << 2;
			}
			else if (pcm8 == 5) {
				const uint16_t fineY = (this->v >> 12) & 7;
				const byte table = this->flag_background_tbl;
				const byte tile = this->name_tbl_u8;
				const uint16_t address = (static_cast<uint16_t>(table) << 12) + (static_cast<uint16_t>(tile) << 4) + fineY;
				this->low_tile_u8 = readPPU(address, mapper, cartridge);
			}
			else if (pcm8 == 7) {
				const uint16_t fineY = (this->v >> 12) & 7;
				const byte table = this->flag_background_tbl;
				const byte tile = this->name_tbl_u8;
				const uint16_t address = (static_cast<uint16_t>(table) << 12) + (static_cast<uint16_t>(tile) << 4) + fineY;
				this->high_tile_u8 = readPPU(address + 8, mapper, cartridge);
			}
			else if (pcm8 == 0) {
				uint32_t data = 0;
				for (int i = 0; i < 8; ++i) {
					const byte a = this->attrib_tbl_u8;
					const byte p1 = (this->low_tile_u8 & 0x80) >> 7;
					const byte p2 = (this->high_tile_u8 & 0x80) >> 6;
					this->low_tile_u8 <<= 1;
					this->high_tile_u8 <<= 1;
					data <<= 4;
					data |= static_cast<uint32_t>(a | p1 | p2);
				}
				this->tile_data |= static_cast<uint64_t>(data);
			}
		}
		if (preline && this->cycle >= 280 && this->cycle <= 304) {
			this->v = (this->v & 0x841F) | (this->t & 0x7BE0);
		}
		if (do_line_render) {
			if (fetch_cycle && (this->cycle & 7) == 0) {
				if ((this->v & 0x001F) == 31) {
					// coarse X = 0
					this->v &= 0xFFE0;
					// switch horizontal nametable
					this->v ^= 0x0400;
				}
				else {
					// increment coarse X
					++this->v;
				}
			}
			if (this->cycle == 256) {
				if ((this->v & 0x7000) != 0x7000) {
					this->v += 0x1000;
				}
				else {
					this->v &= 0x8FFF;
					uint16_t y = (this->v & 0x03E0) >> 5;
					if (y == 29) {
						y = 0;
						this->v ^= 0x0800;
					}
					else if (y == 31) {
						y = 0;
					}
					else {
						++y;
					}
					this->v = (this->v & 0xFC1F) | (y << 5);
				}
			}
			if (this->cycle == 257) {
				this->v = (this->v & 0xFBE0) | (this->t & 0x041F);
			}
		}
	}

	// sprites
	if (do_render && this->cycle == 257) {
		if (line_visible) {
			int h = this->flag_sprite_size != 0 ? 16 : 8;
			int count = 0;
			for (int i = 0; i < 64; ++i) {
				const byte y = this->oam_tbl[4 * i + 0];
				const byte a = this->oam_tbl[4 * i + 2];
				const byte x = this->oam_tbl[4 * i + 3];
				int row = this->scanline - static_cast<int>(y);
				if (row < 0 || row >= h) continue;
				if (count < 8) {
					uint32_t sprite_pattern = 0;
					byte tile = this->oam_tbl[4 * i + 1];
					const byte attributes = this->oam_tbl[4 * i + 2];
					uint16_t address = 0;
					if (this->flag_sprite_size == 0) {
						if ((attributes & 0x80) == 0x80) {
							row = 7 - row;
						}
						byte table = this->flag_sprite_tbl;
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
					byte low_tile_u8 = readPPU(address, mapper, cartridge);
					byte high_tile_u8 = readPPU(address + 8, mapper, cartridge);

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
					this->sprite_patterns[count] = sprite_pattern;
					this->sprite_pos[count] = x;
					this->sprite_priorities[count] = (a >> 5) & 1;
					this->sprite_idx[count] = static_cast<byte>(i);
				}
				++count;
			}
			if (count > 8) {
				count = 8;
				this->flag_sprite_overflow = 1;
			}
			this->sprite_cnt = count;
		}
		else {
			this->sprite_cnt = 0;
		}
	}

	// v_blank logic
	if (this->scanline == 241 && this->cycle == 1) {
		// set v_blank
		std::swap(this->front, this->back);
		this->nmi_occurred = true;
		this->PPUnmiShift();
	}
	if (preline && this->cycle == 1) {
		// clear v_blank
		this->nmi_occurred = false;
		this->PPUnmiShift();

		this->flag_sprite_zero_hit = 0;
		this->flag_sprite_overflow = 0;
	}
}

void PPU::writePPU(uint16_t address, byte value, Mapper* mapper, Cartridge* cartridge) {
	address &= 16383;
	if (address < 0x2000) {
		mapper->write(cartridge, address, value);
	}
	else if (address < 0x3F00) {
		const byte mode = cartridge->mirror;
		this->name_tbl[mirrorAddress(mode, address) & 2047] = value;
	}
	else if (address < 0x4000) {
		// palette
		address &= 31;
		if (address >= 16 && (address & 3) == 0) {
			address -= 16;
		}
		this->palette_tbl[address] = value;
	}
	else {
		std::cerr << "ERROR: PPU encountered unrecognized write (address 0x" << std::hex << address << std::dec << ')' << std::endl;
	}
}

byte PPU::readPPURegister(uint16_t address, Mapper* mapper, Cartridge* cartridge) {
	if (address == 0x2002) {
		byte status = reg & 0x1F;
		status |= this->flag_sprite_overflow << 5;
		status |= this->flag_sprite_zero_hit << 6;
		if (this->nmi_occurred) {
			status |= 1 << 7;
		}
		this->nmi_occurred = false;
		this->PPUnmiShift();
		this->w = 0;
		return status;
	}
	else if (address == 0x2004) {
		return this->oam_tbl[this->oam_addr];
	}
	else if (address == 0x2007) {
		byte value = this->readPPU(this->v, mapper, cartridge);
		// buffered read
		if ((this->v & 16383) < 0x3F00) {
			byte buffered = this->buffered_data;
			this->buffered_data = value;
			value = buffered;
		}
		else {
			this->buffered_data = readPPU(this->v - 0x1000, mapper, cartridge);
		}

		this->v += this->flag_increment == 0 ? 1 : 32;
		return value;
	}
	return 0;
}

void PPU::writeRegisterPPU(uint16_t address, byte value, Mapper *mapper, Cartridge *cartridge, CPU<NES> *cpu, NES* nes) {
	this->reg = value;
	switch (address) {
        case 0x2000:
            this->writePPUCtrl(value);
            break;
        case 0x2001:
            this->writePPUMask(value);
            break;
        case 0x2003:
            this->oam_addr = value;
            break;
        case 0x2004:
            this->oam_tbl[this->oam_addr] = value;
            ++this->oam_addr;
            break;
        case 0x2005:
            // scroll
            if (this->w == 0) {
                this->t = (this->t & 0xFFE0) | (static_cast<uint16_t>(value) >> 3);
                this->x = value & 7;
                this->w = 1;
            }
            else {
                this->t = (this->t & 0x8FFF) | ((static_cast<uint16_t>(value) & 0x07) << 12);
                this->t = (this->t & 0xFC1F) | ((static_cast<uint16_t>(value) & 0xF8) << 2);
                this->w = 0;
            }
            break;
        case 0x2006:
            if (this->w == 0) {
                this->t = (this->t & 0x80FF) | ((static_cast<uint16_t>(value) & 0x3F) << 8);
                this->w = 1;
            }
            else {
                this->t = (this->t & 0xFF00) | static_cast<uint16_t>(value);
                this->v = this->t;
                this->w = 0;
            }
            break;
        case 0x2007:
            this->writePPU(this->v, value, mapper, cartridge);
            this->v += this->flag_increment == 0 ? 1 : 32;
            break;
        case 0x4014:
            // DMA
            address = static_cast<uint16_t>(value) << 8;
            for (int i = 0; i < 256; ++i) {
                this->oam_tbl[this->oam_addr] = nes->readByte(address);
                ++this->oam_addr;
                ++address;
            }
            cpu->stall += 513;
            if (cpu->cycles & 1) {
                ++cpu->stall;
            }
    }
}

