CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT(Calculator)

if (MSVC)
	# Nothing.
else ()
	SET(CMAKE_CXX_FLAGS "-std=c++1y")
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
