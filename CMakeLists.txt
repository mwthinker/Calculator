cmake_minimum_required(VERSION 4.2)
project(Calculator
	DESCRIPTION
		"A C++ Calculator library to evaluate mathematical expressions"
	LANGUAGES
		CXX
	VERSION 0.2.0
)

source_group(".github/workflows" FILES
	"${CMAKE_CURRENT_SOURCE_DIR}/.github/workflows/ci.yml"
)
source_group(".vscode" FILES
	"${CMAKE_CURRENT_SOURCE_DIR}/.vscode/settings.json"
)

set_property(DIRECTORY APPEND PROPERTY VS_SOLUTION_ITEMS
	"${CMAKE_CURRENT_SOURCE_DIR}/.gitattributes"
	"${CMAKE_CURRENT_SOURCE_DIR}/.github/workflows/ci.yml"
	"${CMAKE_CURRENT_SOURCE_DIR}/.gitignore"
	"${CMAKE_CURRENT_SOURCE_DIR}/.vscode/settings.json"
	"${CMAKE_CURRENT_SOURCE_DIR}/CalculatorConfig.cmake.in"
	"${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt"
	"${CMAKE_CURRENT_SOURCE_DIR}/CMakePresets.json"
	"${CMAKE_CURRENT_SOURCE_DIR}/LICENSE.txt"
	"${CMAKE_CURRENT_SOURCE_DIR}/README.md"
	"${CMAKE_CURRENT_SOURCE_DIR}/vcpkg-configuration.json"
	"${CMAKE_CURRENT_SOURCE_DIR}/vcpkg.json"
)

add_library(Calculator STATIC
	src/calc/calculator.cpp
	src/calc/cache.cpp
	src/calc/symbol.cpp
)
add_library(Calculator::Calculator ALIAS Calculator)

include(GNUInstallDirs)

target_sources(Calculator
	PUBLIC
		FILE_SET HEADERS
		BASE_DIRS src
		FILES
			src/calc/calculatorexception.h
			src/calc/calculator.h
			src/calc/cache.h
			src/calc/symbol.h
)

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
		$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/calculator>
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
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
	ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
	RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
	INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/calculator
	FILE_SET HEADERS DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/calculator
)

include(CMakePackageConfigHelpers)

# Export the package configuration
install(EXPORT CalculatorTargets
	FILE CalculatorTargets.cmake
	NAMESPACE Calculator::
	DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/cmake/calculator
)

# Install PDB files for debugging support on Windows
if (MSVC)
	install(
		FILES $<TARGET_FILE_DIR:Calculator>/Calculator.pdb
		DESTINATION ${CMAKE_INSTALL_LIBDIR}
		CONFIGURATIONS Debug RelWithDebInfo
		OPTIONAL
	)
endif()

# Generate the package configuration file
configure_package_config_file(
	${CMAKE_CURRENT_SOURCE_DIR}/CalculatorConfig.cmake.in
	${CMAKE_CURRENT_BINARY_DIR}/CalculatorConfig.cmake
	INSTALL_DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/cmake/calculator
)

# Install the generated package configuration file
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/CalculatorConfig.cmake
	DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/cmake/calculator
)

# Install the package configuration file
write_basic_package_version_file(
	${CMAKE_CURRENT_BINARY_DIR}/CalculatorConfigVersion.cmake
	VERSION ${PROJECT_VERSION}
	COMPATIBILITY AnyNewerVersion
)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/CalculatorConfigVersion.cmake
	DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/cmake/calculator
)

install(FILES LICENSE.txt DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/doc/calculator)
