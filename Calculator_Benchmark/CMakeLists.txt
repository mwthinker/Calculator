project(Calculator_Benchmark
	DESCRIPTION
		"Test the project Calculator using GTest"
	LANGUAGES
		CXX
)

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

	set_target_properties(Calculator_Benchmark
		PROPERTIES
			CXX_STANDARD 20
			CXX_STANDARD_REQUIRED YES
			CXX_EXTENSIONS NO
	)
else ()
	message(STATUS "benchmark not found, Calculator_Benchmark not created")
endif ()
