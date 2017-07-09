#include <iostream>
#include <memory>
#include "nes/include/emulator.h"
#include "nes/include/ines.h"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cout << "no rom found" << std::endl;
        return EXIT_FAILURE;
    }

    rom nes_rom = load_nes(argv[1]);

    std::unique_ptr<emulator> emu (new emulator(nes_rom));
    emu->run();

    /*if (!glfwInit())
    {
        std::cout << "Install glfw3"<< std::endl;
        return EXIT_FAILURE;
    }

    glfwTerminate();*/
    return EXIT_SUCCESS;
}