#ifndef VMOS6502_INPUT_H
#define VMOS6502_INPUT_H

#include <GLFW/glfw3.h>
#include "nes.h"

constexpr int AUDIO_FRAME_BUFFER_SIZE = 1024;

bool getKey(GLFWwindow *window, int key) {
    return glfwGetKey(window, key) == GLFW_PRESS;
}

byte getKeys(GLFWwindow *window, bool turbo) {
    byte ret = getKey(window, GLFW_KEY_Z) || (turbo && getKey(window, GLFW_KEY_S));
    ret |= (getKey(window, GLFW_KEY_X) || (turbo && getKey(window, GLFW_KEY_D))) << 1;
    ret |= (getKey(window, GLFW_KEY_RIGHT_SHIFT)) << 2;
    ret |= (getKey(window, GLFW_KEY_ENTER)) << 3;
    ret |= (getKey(window, GLFW_KEY_UP)) << 4;
    ret |= (getKey(window, GLFW_KEY_DOWN)) << 5;
    ret |= (getKey(window, GLFW_KEY_LEFT)) << 6;
    ret |= (getKey(window, GLFW_KEY_RIGHT)) << 7;
    return ret;
}

byte getJoy(int joy, bool turbo) {
    if (!glfwJoystickPresent(joy)) {
        return 0;
    }
    int count;
    const float *axes = glfwGetJoystickAxes(joy, &count);
    const unsigned char *buttons = glfwGetJoystickButtons(joy, &count);

    byte ret = buttons[0] == 1 || (turbo && buttons[2] == 1);
    ret |= (buttons[1] == 1 || (turbo && buttons[3] == 1)) << 1;
    ret |= (buttons[6] == 1) << 2;
    ret |= (buttons[7] == 1) << 3;
    ret |= (axes[1] < -0.5f) << 4;
    ret |= (axes[1] > 0.5f) << 5;
    ret |= (axes[0] < -0.5f) << 6;
    ret |= (axes[0] > 0.5f) << 7;

    return ret;
}

void notifyPaError(const PaError err) {
    Pa_Terminate();
    std::cout << std::endl << "PortAudio error:" << std::endl;
    std::cout << "Error code: " << err << std::endl;
    std::cout << "Error message: " << Pa_GetErrorText(err) << std::endl;

    if ((err == paUnanticipatedHostError)) {
        const PaHostErrorInfo *hostErrorInfo = Pa_GetLastHostErrorInfo();
        std::cout << "Host info error code: " << hostErrorInfo->errorCode << std::endl;
        std::cout << "Host info error message: " << hostErrorInfo->errorText << std::endl << std::endl;
    }
}

void initalizePulseAudio(std::unique_ptr<NES>& nes) {

    PaError err = Pa_Initialize();
    if (err != paNoError) {
        notifyPaError(err);
        throw std::runtime_error("PA failed to initialize.");
    }

    PaStreamParameters outputParameters;
    outputParameters.device = Pa_GetDefaultOutputDevice();

    if (outputParameters.device == paNoDevice) {
        std::cerr << "ERROR: no PortAudio device found." << std::endl;
        notifyPaError(err);
        throw std::runtime_error("PA failed to find a device.");
    }

    outputParameters.channelCount = 2;
    outputParameters.sampleFormat = paFloat32;
    outputParameters.suggestedLatency = Pa_GetDeviceInfo(outputParameters.device)->defaultLowOutputLatency;
    outputParameters.hostApiSpecificStreamInfo = nullptr;

    err = Pa_OpenStream(
            &nes->apu->stream,
            nullptr,
            &outputParameters,
            44100,
            AUDIO_FRAME_BUFFER_SIZE,
            paNoFlag,
            nullptr,
            nullptr);

    if (err != paNoError) {
        notifyPaError(err);
        throw std::runtime_error("PA encoutered an error.");
    }
}

#endif //VMOS6502_INPUT_H
