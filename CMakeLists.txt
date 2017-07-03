cmake_minimum_required(VERSION 3.7)
project(vmos6502)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES main.cpp emulator.cpp emulator.h)
add_executable(vmos6502 ${SOURCE_FILES})