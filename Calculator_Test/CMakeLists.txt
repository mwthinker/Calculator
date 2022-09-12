cmake_minimum_required(VERSION 3.21)
project(Calculator_Test
	DESCRIPTION
		"Test the project Calculator using GTest"
	LANGUAGES
		CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(GTest CONFIG)
if (GTest_FOUND)
	enable_testing()

	add_executable(Calculator_Test
		src/tests.cpp
	)
	
	target_link_libraries(Calculator_Test
		PRIVATE
			Calculator
			GTest::gtest GTest::gtest_main
	)
	
	include(GoogleTest)
	gtest_discover_tests(Calculator_Test)
else ()
	message(STATUS "GTest not found, Calculator_Test not created")
endif ()
