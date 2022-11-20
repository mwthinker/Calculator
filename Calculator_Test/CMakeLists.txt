project(Calculator_Test
	DESCRIPTION
		"Test the project Calculator using GTest"
	LANGUAGES
		CXX
)

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

	set_target_properties(Calculator_Test
		PROPERTIES
			CXX_STANDARD 20
			CXX_STANDARD_REQUIRED YES
			CXX_EXTENSIONS NO
	)
	
	include(GoogleTest)
	gtest_discover_tests(Calculator_Test)
else ()
	message(STATUS "GTest not found, Calculator_Test not created")
endif ()
