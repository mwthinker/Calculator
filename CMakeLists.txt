cmake_minimum_required(VERSION 3.21)
project(Calculator
	DESCRIPTION
		"A C++ Calculator library to evaluate mathematical expressions"
	LANGUAGES
		CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(Calculator
	src/calc/calculatorexception.h
	src/calc/calculator.cpp
	src/calc/calculator.h
	src/calc/cache.cpp
	src/calc/cache.h
	src/calc/symbol.cpp
	src/calc/symbol.h
	vcpkg.json
	CMakeLists.txt
	CMakePresets.json
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
endif ()

message(STATUS "Calculator_Benchmark is available to add: -DCalculator_Benchmark=1")
option(Calculator_Benchmark "Add Calculator_Benchmark project." OFF)
if (Calculator_Benchmark)
	add_subdirectory(Calculator_Benchmark)
endif ()
