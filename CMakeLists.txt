cmake_minimum_required(VERSION 3.7)

project(vmos6502)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Werror -march=native -flto -fpeel-loops -ftracer -ftree-vectorize")

set(SOURCE_FILES main.cpp cpu.cpp nes.h nes.cpp memory.cpp cartridge.h cartridge.cpp mapper.cpp mapper.h cpu.h apu.cpp apu.h ppu.cpp ppu.h)
add_executable(vmos6502 ${SOURCE_FILES})
target_link_libraries(vmos6502 glfw portaudio GL)