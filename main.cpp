#include <iostream>
#include "emulator.h"

int main() {
    std::cout << "Hello, World!" << std::endl;
    emulator *emu;
    emu = new emulator(1024, 1, 1);
    emu->run();
    return 0;
}