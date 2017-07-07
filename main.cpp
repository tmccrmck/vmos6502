#include <iostream>
#include "nes/emulator.h"
#include "nes/INes.h"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cout << "Give rom" << std::endl;
        return EXIT_FAILURE;
    }

    loadNesFile(argv[1]);

    emulator *emu;
    emu = new emulator(1024, 1, 1);
    emu->run();

    /*if (!glfwInit())
    {
        std::cout << "Install glfw3"<< std::endl;
        return EXIT_FAILURE;
    }

    glfwTerminate();*/
    return EXIT_SUCCESS;
}