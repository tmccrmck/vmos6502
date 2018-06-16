//
// Created by trent on 4/22/18.
//

#include "controller.h"

byte Controller::readController() {
    const byte value = (this->index < 8 && ((this->buttons >> this->index) & 1));
    ++this->index;
    if ((this->strobe & 1) == 1) {
        this->index = 0;
    }
    return value;
}

void Controller::writeController(byte value) {
    this->strobe = value;
    if ((this->strobe & 1) == 1) {
        this->index = 0;
    }
}
