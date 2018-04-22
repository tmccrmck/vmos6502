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

void tickEnvelope(APU* apu) {
	pulseTickEnvelope(&apu->pulse1);
	pulseTickEnvelope(&apu->pulse2);

	Triangle* t = &apu->triangle;
	if (t->counter_reload) {
		t->counter_val = t->counter_period;
	}
	else if (t->counter_val > 0) {
		--t->counter_val;
	}
	if (t->length_enabled) {
		t->counter_reload = false;
	}

	Noise* n = &apu->noise;
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

