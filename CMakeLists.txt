cmake_minimum_required(VERSION 3.7)
project(vmos6502)

set(CMAKE_CXX_STANDARD 14)

find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)
include_directories(${GLFW_INCLUDE_DIRS})
#find_package(glfw3 REQUIRED)

set(SOURCE_FILES main.cpp emulator.cpp emulator.h)
add_executable(vmos6502 ${SOURCE_FILES})
target_link_libraries(vmos6502 ${GLFW_LIBRARIES})
