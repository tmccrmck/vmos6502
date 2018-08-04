#ifndef VMOS6502_CONTROLLER_H
#define VMOS6502_CONTROLLER_H

#include <cstdint>

typedef uint8_t byte;

class Controller {
public:
    byte buttons;
    byte index;
    byte strobe;

    Controller() : buttons(0), index(0), strobe(0) {}
    ~Controller() = default;

    byte readController();

    void writeController(byte value);

};

#endif //VMOS6502_CONTROLLER_H
