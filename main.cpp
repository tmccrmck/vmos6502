#include <memory>
#include <iostream>
#include <portaudio.h>
#include <GLFW/glfw3.h>
#include "pa_linux_alsa.h"
#include "nes.h"

constexpr int AUDIO_FRAME_BUFFER_SIZE = 1024;

bool getKey(GLFWwindow* window, int key) {
	return glfwGetKey(window, key) == GLFW_PRESS;
}

byte getKeys(GLFWwindow* window, bool turbo) {
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
	const float* axes = glfwGetJoystickAxes(joy, &count);
	const unsigned char* buttons = glfwGetJoystickButtons(joy, &count);

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
		const PaHostErrorInfo* hostErrorInfo = Pa_GetLastHostErrorInfo();
		std::cout << "Host info error code: " << hostErrorInfo->errorCode << std::endl;
		std::cout << "Host info error message: " << hostErrorInfo->errorText << std::endl << std::endl;
	}
}

int main(int argc, char* argv[]) {
    if (!glfwInit()) {
        throw std::runtime_error("GLFW initialization failed");
    }

	if (argc != 2) {
		throw std::invalid_argument("Must provide ROM");
	}

	std::string sram (argv[1]);
    std::string sram_path = sram + ".srm";

	std::unique_ptr<NES> nes = std::make_unique<NES>(sram, sram_path);
	if (!nes->initialized) return EXIT_FAILURE;

	PaError err = Pa_Initialize();
	if (err != paNoError) {
		notifyPaError(err);
		return EXIT_FAILURE;
	}

	PaStreamParameters outputParameters;
	outputParameters.device = Pa_GetDefaultOutputDevice();

	if (outputParameters.device == paNoDevice) {
		std::cerr << "ERROR: no PortAudio device found." << std::endl;
		notifyPaError(err);
		return EXIT_FAILURE;
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
		return EXIT_FAILURE;
	}

	std::cout << "Initializing GLFW..." << std::endl;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

	std::cout << "Initializing GLFW window..." << std::endl;
	GLFWwindow* window;
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
	window = glfwCreateWindow(512, 480, "vmos", nullptr, nullptr);
	std::cout << "Window created." << std::endl;

	if (!window) {
		std::cerr << "ERROR: Failed to create window. Aborting." << std::endl;
		return EXIT_FAILURE;
	}

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

		if ((nes->ppu->frame & 3) == 0) nes->printState();

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

	// save SRAM back to file
	if (nes->cartridge->battery_present) {
		std::cout << std::endl << "Writing SRAM..." << std::endl;
		FILE* fp = fopen(sram_path.c_str(), "wb");
		if (fp == nullptr || (fwrite(nes->cartridge->SRAM, 8192, 1, fp) != 1)) {
			std::cout << "WARN: failed to save SRAM file!" << std::endl;
		}
		else {
			fclose(fp);
		}
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
