#include "ppu.h"

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

