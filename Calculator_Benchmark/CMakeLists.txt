cmake_minimum_required(VERSION 3.21)
project(Calculator_Benchmark
	DESCRIPTION
		"Test the project Calculator using GTest"
	LANGUAGES
		CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(benchmark CONFIG)
if (benchmark_FOUND)
	add_executable(Calculator_Benchmark
		src/speedtest.cpp
	)
	
	target_link_libraries(Calculator_Benchmark
		PRIVATE
			Calculator
			benchmark::benchmark benchmark::benchmark_main
	)
else ()
	message(STATUS "benchmark not found, Calculator_Benchmark not created")
endif ()
