cmake_minimum_required(VERSION 3.21)
project(Calculator
	DESCRIPTION
		"A C++ Calculator library to evaluate mathematical expressions"
	LANGUAGES
		CXX
	VERSION 0.1.1
)

add_library(Calculator STATIC
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
add_library(Calculator::Calculator ALIAS Calculator)

set_property(GLOBAL PROPERTY USE_FOLDERS On)

option(CODE_COVERAGE "Enable coverage reporting" OFF)
if (CODE_COVERAGE AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
	target_compile_options(Calculator PUBLIC --coverage)
	target_link_options(Calculator PUBLIC --coverage)
endif ()

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
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
		$<INSTALL_INTERFACE:include/calculator>
)
set_target_properties(Calculator
	PROPERTIES
		CXX_STANDARD 23
		CXX_STANDARD_REQUIRED YES
		CXX_EXTENSIONS NO
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

# -------------------------------------------------------------------------
# Install
install(TARGETS Calculator
	EXPORT CalculatorTargets
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
	RUNTIME DESTINATION bin
	INCLUDES DESTINATION include
)

install(DIRECTORY src/calc/
	DESTINATION include/calculator/calc
	FILES_MATCHING PATTERN "*.h"
)

include(CMakePackageConfigHelpers)

# Export the package configuration
install(EXPORT CalculatorTargets
	FILE CalculatorTargets.cmake
	NAMESPACE Calculator::
	DESTINATION share/cmake/calculator
)

# Generate the package configuration file
configure_package_config_file(
	${CMAKE_CURRENT_SOURCE_DIR}/CalculatorConfig.cmake.in
	${CMAKE_CURRENT_BINARY_DIR}/CalculatorConfig.cmake
	INSTALL_DESTINATION share/cmake/calculator
)

# Install the generated package configuration file
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/CalculatorConfig.cmake
	DESTINATION share/cmake/calculator
)

# Install the package configuration file
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
	${CMAKE_CURRENT_BINARY_DIR}/CalculatorConfigVersion.cmake
	VERSION ${PROJECT_VERSION}
	COMPATIBILITY AnyNewerVersion
)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/CalculatorConfigVersion.cmake
	DESTINATION share/cmake/calculator
)

install(FILES LICENSE.txt DESTINATION share/doc/calculator)