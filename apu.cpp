#include "nes.h"

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
