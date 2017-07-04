#include <iostream>
#include <GLFW/glfw3.h>
#include <unistd.h>

#include "emulator.h"

int main() {

    std::cout << "Hello, World!" << std::endl;
    emulator *emu;
    emu = new emulator(1024, 1, 1);
    emu->run();

    if (!glfwInit())
    {
        std::cout << "Install glfw3"<< std::endl;
        return EXIT_FAILURE;
    }

    GLFWwindow* window = glfwCreateWindow(640, 480, "My Title", NULL, NULL);
    sleep(10);
    glfwTerminate();
    return 0;
}