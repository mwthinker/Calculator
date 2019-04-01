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

set(SOURCES
	src/calc/calculatorexception.h
	src/calc/calculator.cpp
	src/calc/calculator.h
	src/calc/cache.cpp
	src/calc/cache.h
	src/calc/symbol.cpp
	src/calc/symbol.h
)

add_library(Calculator ${SOURCES})

target_include_directories(Calculator
	PUBLIC
		"$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>"
)

find_package(Catch2 CONFIG REQUIRED)
if (Catch2_FOUND)
	message(STATUS "CalculatorTest is available to add")
	option(CalculatorTest "CalculatorTest project is added" OFF)
	if (CalculatorTest)
		set(SOURCES_TEST
			srcTest/main.cpp
			srcTest/speedtest.cpp
			srcTest/tests.cpp
		)
	
		add_executable(CalculatorTest ${SOURCES_TEST})
		
		target_link_libraries(CalculatorTest
			PRIVATE
				Calculator
				Catch2::Catch2
		)
	endif (CalculatorTest)
else (Catch2_FOUND)
	message(STATUS "Catch2 not found, not CalculatorTest created")
endif (Catch2_FOUND)
