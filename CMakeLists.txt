cmake_minimum_required(VERSION 3.1...3.14)

set(VCPKG_ROOT $ENV{VCPKG_ROOT})
if(DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
	set(CMAKE_TOOLCHAIN_FILE "${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
		CACHE STRING ""
	)
	message(STATUS "Use VCPKG CMAKE_TOOLCHAIN_FILE")
endif()

project(Calculator
	DESCRIPTION
		"A C++ Calculator library to evaluate mathematical expressions"
	LANGUAGES
		CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(Calculator
	src/calc/calculatorexception.h
	src/calc/calculator.cpp
	src/calc/calculator.h
	src/calc/cache.cpp
	src/calc/cache.h
	src/calc/symbol.cpp
	src/calc/symbol.h
)

if (MSVC)
	target_compile_options(Calculator
		PRIVATE
			/W3 /WX /permissive-
	)
else()
	target_compile_options(Calculator
		PRIVATE
			-Wall -Wextra -Wnon-virtual-dtor -pedantic -Wcast-align -Woverloaded-virtual -Wno-unused-parameter -Wno-parentheses
	)
endif()

target_include_directories(Calculator
	PUBLIC
		"$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>"
)

message(STATUS "Calculator_Test is available to add: -DCalculator_Test=1")
option(Calculator_Test "Add Calculator_Test project." OFF)
if (Calculator_Test)
	add_subdirectory(Calculator_Test)
endif (Calculator_Test)
