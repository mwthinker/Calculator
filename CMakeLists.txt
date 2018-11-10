cmake_minimum_required(VERSION 3.1...3.12)
project(Calculator
	DESCRIPTION
		"A C++ Calculator library to evaluate mathematical expressions"
	LANGUAGES
		CXX
)
project(Calculator)

# In order to find Catch2.
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules/)

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

find_package(CATCH2)
if (CATCH2_FOUND)
	option(CalculatorTest "CalculatorTest project is added" OFF)
	
	if (CalculatorTest)
		set(SOURCES_TEST
			srcTest/main.cpp
			srcTest/speedtest.cpp
			srcTest/tests.cpp
		)
	
		add_executable(CalculatorTest ${SOURCES_TEST})	
		
		target_include_directories(CalculatorTest
			PRIVATE
				${CATCH2_INCLUDE_DIRS}			
		)
		
		target_link_libraries(CalculatorTest
			PRIVATE
				Calculator
		)
	endif (CalculatorTest)
endif (CATCH2_FOUND)
