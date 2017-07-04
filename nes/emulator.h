//
// Created by trent on 6/30/17.
//

#ifndef VMOS6502_EMULATOR_H
#define VMOS6502_EMULATOR_H


class emulator {
public:
    emulator(int memory_size, int eip, int esp);
    void run();
private:
    void execute();
};


#endif //VMOS6502_EMULATOR_H
