cmake_minimum_required(VERSION 2.8)
project(Calculator)

if (NOT MSVC)
	SET(CMAKE_CXX_FLAGS "-std=c++1y")
	if (Debug)
		set(CMAKE_BUILD_TYPE "Debug" CACHE INTERNAL "Build type.")
		add_definitions(-D_DEBUG)
	else ()
		set(CMAKE_BUILD_TYPE "Release" CACHE INTERNAL "Build type.")
	endif ()
endif ()

option(CalculatorTest "CalculatorTest project is added" OFF)

set(SOURCES
	src/calc/calculator.cpp
	src/calc/calculator.h
	src/calc/cache.cpp
	src/calc/cache.h
	src/calc/symbol.cpp
	src/calc/symbol.h
)

set(SOURCES_TEST
	srcTest/main.cpp
)

add_library(Calculator ${SOURCES})

if (CalculatorTest)
	include_directories(src)
	add_executable(CalculatorTest ${SOURCES_TEST})
	target_link_libraries(CalculatorTest Calculator)
endif ()
