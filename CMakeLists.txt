cmake_minimum_required(VERSION 3.7)

project(vmos6502)

set_property (GLOBAL PROPERTY USE_FOLDERS ON)

set(CMAKE_CXX_STANDARD 14)
set (CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wpedantic -lportaudio -lglfw3 -lGL -lm -ldl -lXinerama -lXrandr -lXi -lXcursor -lX11 -lXxf86vm -lpthread")

find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)
include_directories(${GLFW_INCLUDE_DIRS})
find_package(glfw3 REQUIRED)

set(SOURCE_FILES main.cpp cpu.cpp NES.h NES.cpp memory.cpp)
add_executable(vmos6502 ${SOURCE_FILES})
target_link_libraries(vmos6502 ${GLFW_LIBRARIES})
