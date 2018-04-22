cmake_minimum_required(VERSION 3.7)

project(vmos6502)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Werror -Wshadow -pedantic -Ofast -std=gnu++14 -fomit-frame-pointer -march=native -flto -fpeel-loops -ftracer -ftree-vectorize")

set(SOURCE_FILES main.cpp cpu.cpp NES.h NES.cpp memory.cpp)
add_executable(vmos6502 ${SOURCE_FILES})
target_link_libraries(vmos6502 glfw portaudio GL)