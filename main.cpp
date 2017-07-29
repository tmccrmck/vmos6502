#include <iostream>
#include <memory>
#include "nes/include/ines.h"
#include "nes/include/cpu.h"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cout << "no rom found" << std::endl;
        return EXIT_FAILURE;
    }

    auto nes_rom = load_nes(argv[1]);
    //auto cpu = std::make_unique<Cpu<>>();

    /*if (!glfwInit())
    {
        std::cout << "Install glfw3"<< std::endl;
        return EXIT_FAILURE;
    }

    glfwTerminate();*/
    return EXIT_SUCCESS;
}