#include <iostream>
#include <portaudio.h>
#include "pa_linux_alsa.h"
#include "io.h"

int main(int argc, char *argv[]) {
    if (!glfwInit()) {
        throw std::runtime_error("GLFW initialization failed");
    }

    if (argc != 2) {
        throw std::invalid_argument("Must provide ROM");
    }

    std::string sram(argv[1]);
    std::string sram_path = sram + ".srm";

    std::cout << "Initializing NES object..." << std::endl;
    std::unique_ptr<NES> nes = std::make_unique<NES>(sram, sram_path);

    std::cout << "Initializing PulseAudio..." << std::endl;
    initalizePulseAudio(nes);

    std::cout << "Initializing GLFW window..." << std::endl;
    GLFWwindow *window;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
    window = glfwCreateWindow(512, 480, "vmos", nullptr, nullptr);

    if (!window) {
        std::cerr << "ERROR: Failed to create window. Aborting." << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "Window created." << std::endl;

    glfwMakeContextCurrent(window);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

    glfwSwapInterval(0);

    int old_w, old_h, w, h;
    glfwGetFramebufferSize(window, &old_w, &old_h);

    std::cout << "Framebuffer reports initial dimensions " << old_w << "x" << old_h << '.' << std::endl;
    std::cout << "Creating display texture..." << std::endl;

    GLuint texture;
    glEnable(GL_TEXTURE_2D);
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

    PaAlsa_EnableRealtimeScheduling(nes->apu->stream, 1);
    std::cout << "Starting audio stream..." << std::endl;
    Pa_StartStream(nes->apu->stream);

    double prevtime = 0.0;
    while (!glfwWindowShouldClose(window)) {
        const double time = glfwGetTime();
        const double dt = time - prevtime < 1.0 ? time - prevtime : 1.0;
        prevtime = time;

        const bool turbo = (nes->ppu->frame % 6) < 3;
        glfwPollEvents();
        nes->controller1->buttons = getKeys(window, turbo) | getJoy(GLFW_JOYSTICK_1, turbo);
        nes->controller2->buttons = getJoy(GLFW_JOYSTICK_2, turbo);

        // step the NES state forward by 'dt' seconds, or more if in fast-forward
        nes->emulate(getKey(window, GLFW_KEY_GRAVE_ACCENT) ? 4.0 * dt : dt);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 256, 240, 0, GL_RGBA, GL_UNSIGNED_BYTE, nes->ppu->front);
        glfwGetFramebufferSize(window, &w, &h);
        if (w != old_w || h != old_h) {
            old_w = w;
            old_h = h;
            std::cout << std::endl << "Framebuffer reports resize to " << w << "x" << h << '.' << std::endl;
        }

        const float s1 = static_cast<float>(w) / 256.0f;
        const float s2 = static_cast<float>(h) / 240.0f;

        const float x = (s1 >= s2) ? s2 / s1 : 1.0f;
        const float y = (s1 >= s2) ? 1.0f : s1 / s2;

        glViewport(0, 0, w, h);
        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 1.0f);
        glVertex2f(-x, -y);
        glTexCoord2f(1.0f, 1.0f);
        glVertex2f(x, -y);
        glTexCoord2f(1.0f, 0.0f);
        glVertex2f(x, y);
        glTexCoord2f(0.0f, 0.0f);
        glVertex2f(-x, y);
        glEnd();

        glfwSwapBuffers(window);

        if (getKey(window, GLFW_KEY_ESCAPE)) glfwSetWindowShouldClose(window, GLFW_TRUE);
    }

    std::cout << std::endl << "Stopping audio stream..." << std::endl;
    Pa_StopStream(nes->apu->stream);

    std::cout << "Closing audio stream..." << std::endl;
    Pa_CloseStream(nes->apu->stream);

    std::cout << "Terminating GLFW..." << std::endl;
    glfwTerminate();

    std::cout << "Terminating PortAudio..." << std::endl;
    Pa_Sleep(500L);
    Pa_Terminate();

    return EXIT_SUCCESS;
}
