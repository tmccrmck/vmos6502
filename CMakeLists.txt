cmake_minimum_required(VERSION 3.7)

project(vmos6502)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Werror -march=native -flto -ftree-vectorize -Ofast")

find_package(glfw3 3.2 REQUIRED)
find_package(OpenGL REQUIRED)
#find_package(PORTAUDIO REQUIRED)

set(SOURCE_FILES main.cpp cpu.cpp nes.cpp cartridge.cpp mapper.cpp apu.cpp ppu.cpp controller.cpp)
add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} glfw portaudio GL)
