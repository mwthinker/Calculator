cmake_minimum_required(VERSION 3.1...3.14)
project(Calculator_Test
	DESCRIPTION
		"Test the project Calculator using Catch2"
	LANGUAGES
		CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Catch2 CONFIG)
if (Catch2_FOUND)
	add_executable(Calculator_Test
		src/main.cpp
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
			Catch2::Catch2
	)
else (Catch2_FOUND)
	message(STATUS "Catch2 not found, Calculator_Test not created")
endif (Catch2_FOUND)
