cmake_minimum_required(VERSION 3.21)
project(Calculator_Test
	DESCRIPTION
		"Test the project Calculator using Catch2"
	LANGUAGES
		CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Catch2 CONFIG)
if (Catch2_FOUND)
	enable_testing()
	
	include(Catch)

	add_executable(Calculator_Test
		src/speedtest.cpp
		src/tests.cpp
	)
	
	target_compile_definitions(Calculator_Test
		PRIVATE
			CATCH_CONFIG_ENABLE_BENCHMARKING
	)
	
	target_link_libraries(Calculator_Test
		PRIVATE
			Calculator
			Catch2::Catch2 Catch2::Catch2WithMain
	)

	catch_discover_tests(Calculator_Test)
else (Catch2_FOUND)
	message(STATUS "Catch2 not found, Calculator_Test not created")
endif (Catch2_FOUND)
