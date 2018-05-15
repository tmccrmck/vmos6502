#ifndef VMOS6502_APU_H
#define VMOS6502_APU_H

#include <cstdint>
#include <portaudio.h>
#include "cpu.h"

typedef uint8_t byte;

class NES;
class CPU;

constexpr double CPU_FREQ = 1789773.0;
constexpr double FRAME_CTR_FREQ = CPU_FREQ / 240.0;
constexpr double SAMPLE_RATE = CPU_FREQ / (44100.0);

class Pulse {
public:
	bool enabled;
	uint8_t channel;
	bool length_enabled;
	uint8_t length_val;
	uint16_t timer_period;
	uint16_t timer_val;
	uint8_t duty_mode;
	uint8_t duty_val;
	bool sweep_reload;
	bool sweep_enabled;
	bool sweep_negate;
	uint8_t sweep_shift;
	uint8_t sweep_period;
	uint8_t sweep_val;
	bool envelope_enabled;
	bool envelope_loop;
	bool envelope_start;
	uint8_t envelope_period;
	uint8_t envelope_val;
	uint8_t envelope_vol;
	uint8_t const_vol;

	Pulse() : enabled(false), channel(0), length_enabled(false), length_val(0), timer_period(0), timer_val(0), duty_mode(0), duty_val(0), sweep_reload(false), sweep_enabled(false), sweep_negate(false), sweep_shift(0), sweep_period(0), sweep_val(0), envelope_enabled(false), envelope_loop(false), envelope_start(false), envelope_period(0), envelope_val(0), envelope_vol(0), const_vol(0) {}
	void tickPulseTimer();
	void sweep();
	void pulseTickSweep();
	byte pulseOutput();
    void pulseWriteControl(byte value);
    void pulseWriteSweep(byte value);
    void pulseWriteTimerHigh(byte value);

};

struct Triangle {
	bool enabled;
	bool length_enabled;
	uint8_t length_val;
	uint16_t timer_period;
	uint16_t timer_val;
	uint8_t duty_val;
	uint8_t counter_period;
	uint8_t counter_val;
	bool counter_reload;

	Triangle() : enabled(false), length_enabled(false), length_val(0), timer_period(0), timer_val(0), duty_val(0), counter_period(0), counter_val(0), counter_reload(false) {}
};

struct Noise {
	bool enabled;
	bool mode;
	uint16_t shift_reg;
	bool length_enabled;
	uint8_t length_val;
	uint16_t timer_period;
	uint16_t timer_val;
	bool envelope_enabled;
	bool envelope_loop;
	bool envelope_start;
	uint8_t envelope_period;
	uint8_t envelope_val;
	uint8_t envelope_vol;
	uint8_t const_vol;

	Noise() : enabled(false), mode(false), shift_reg(0), length_enabled(false), length_val(0), timer_period(0), timer_val(0), envelope_enabled(false), envelope_loop(false), envelope_start(false), envelope_period(0), envelope_val(0), envelope_vol(0), const_vol(0) {}
};

struct DMC {
	bool enabled;
	uint8_t value;
	uint16_t samp_addr;
	uint16_t samp_len;
	uint16_t cur_addr;
	uint16_t cur_len;
	uint8_t shift_reg;
	uint8_t bit_count;
	uint8_t tick_period;
	uint8_t tick_val;
	bool loop;
	bool irq;

	DMC() : enabled(false), value(0), samp_addr(0), samp_len(0), cur_addr(0), cur_len(0), shift_reg(0), bit_count(0), tick_period(0), tick_val(0), loop(false), irq(false) {}
};

class APU {
public:
	PaStream* stream{};
	Pulse pulse1;
	Pulse pulse2;
	Triangle triangle;
	Noise noise;
	DMC dmc;
	uint64_t cycle;
	uint8_t frame_period;
	uint8_t frame_val;
	bool frame_IRQ;

	APU();

	void tickLength();
    void tickEnvelope();
    void tickSweep();
    void writeRegisterAPU(uint16_t address, byte value);

	void tickAPU(CPU* cpu);
};

void dmcRestart(DMC* d);

// TODO: make class method
void tickEnvelope(APU* apu);
void tickSweep(APU* apu);
void tickLength(APU* apu);
#endif //VMOS6502_APU_H
