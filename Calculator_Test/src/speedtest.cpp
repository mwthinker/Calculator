#include <catch2/catch_test_macros.hpp>
#include <catch2/benchmark/catch_benchmark.hpp>

#include <calc/calculator.h>
#include <calc/calculatorexception.h>

TEST_CASE("benchmarked", "[!benchmark]") {
	calc::Calculator calculator;
	std::string expression = "2.1+-3.2*5^(3-1)/(2*3.14 - 1) + VAR";
	calculator.addVariable("VAR", 3.14f);

	const int ITERATIONS = 1000;

	BENCHMARK("test with no precalculation") {
		for (int i = 0; i < ITERATIONS; ++i) {
			calculator.updateVariable("VAR", i * 0.0001f);
			calculator.excecute(expression);
		}
	};

	calc::Cache cache = calculator.preCalculate(expression);

	BENCHMARK("test with precalculate") {
		for (int i = 0; i < ITERATIONS; ++i) {
			calculator.updateVariable("VAR", i * 0.0001f);
			calculator.excecute(cache);
		}
	};

	expression = "-1 * (11.2 * 12 / 123 * 10.4^2) * (11.2 * 12 / 123 * 10.4^2)*(11.2 * 12 / 123 * 10.4^2) * (11.2 * 12 / 123 * 10.4^2) - 12";
	cache = calculator.preCalculate(expression);
	BENCHMARK("calculate long expression") {
		for (int i = 0; i < ITERATIONS; ++i) {
			calculator.excecute(cache);
		}
	};
}
