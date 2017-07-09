#include "rom.h"

class emulator {
public:
    explicit emulator(const rom &r);

    emulator(const emulator &rom);

    void run();
private:
};
