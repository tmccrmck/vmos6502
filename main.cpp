#include <iostream>
#include <memory>
#include "nes/include/emulator.h"
#include "nes/include/ines.h"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cout << "no rom found" << std::endl;
        return EXIT_FAILURE;
    }

    ines::load(argv[1]);

    std::unique_ptr<emulator> emu (new emulator(1024, 1, 1));
    emu->run();

    /*if (!glfwInit())
    {
        std::cout << "Install glfw3"<< std::endl;
        return EXIT_FAILURE;
    }

    glfwTerminate();*/
    return EXIT_SUCCESS;
}