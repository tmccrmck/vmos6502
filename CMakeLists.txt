cmake_minimum_required(VERSION 3.7)

project(vmos6502)

set_property (GLOBAL PROPERTY USE_FOLDERS ON)

set(CMAKE_CXX_STANDARD 14)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

if (UNIX)
	find_package(PkgConfig REQUIRED)
	pkg_search_module(GLFW REQUIRED glfw3)
	include_directories(${GLFW_INCLUDE_DIRS})
	#find_package(glfw3 REQUIRED)
endif (UNIX)

set(SOURCE_FILES main.cpp nes/nes.cpp nes/include/nes.h nes/cpu.cpp nes/include/cpu.h nes/include/ines.h nes/rom.cpp nes/include/rom.h nes/ines.cpp nes/include/ines_header.h nes/include/mem.h)
add_executable(vmos6502 ${SOURCE_FILES})
target_link_libraries(vmos6502 ${GLFW_LIBRARIES})
