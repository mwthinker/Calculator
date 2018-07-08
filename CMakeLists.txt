cmake_minimum_required(VERSION 2.8)
project(Calculator)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules/)

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
	src/calc/calculatorexception.h
	src/calc/calculator.cpp
	src/calc/calculator.h
	src/calc/cache.cpp
	src/calc/cache.h
	src/calc/symbol.cpp
	src/calc/symbol.h
)

set(SOURCES_TEST
	srcTest/main.cpp
	srcTest/speedtest.cpp
	srcTest/tests.cpp
)

add_library(Calculator ${SOURCES})

find_package(CATCH2)

if (CATCH2_FOUND)
	if (CalculatorTest)
		add_executable(CalculatorTest ${SOURCES_TEST})	
		
		target_include_directories(CalculatorTest
			PUBLIC ${CATCH2_INCLUDE_DIRS}
			PUBLIC src
		)
		
		target_link_libraries(CalculatorTest Calculator)
	endif ()
endif ()

#if (CalculatorTest)
#	include_directories(src)
#	add_executable(CalculatorTest ${SOURCES_TEST})
#	target_link_libraries(CalculatorTest Calculator)
#endif ()
