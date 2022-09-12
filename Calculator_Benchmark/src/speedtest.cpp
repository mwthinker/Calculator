#include <benchmark/benchmark.h>

#include <calc/calculator.h>
#include <calc/calculatorexception.h>

class MyFixture : public benchmark::Fixture {
public:
	void SetUp(const ::benchmark::State& state) override {
		calculator = calc::Calculator{};
		calculator.addVariable("VAR", 3.14f);
	}

	void TearDown(const ::benchmark::State& state) override {
	
	}

	calc::Calculator calculator;
	const std::string Expression = "2.1+-3.2*5^(3-1)/(2*3.14 - 1) + VAR";
	static constexpr int Iterations = 1000;
};


BENCHMARK_F(MyFixture, noPreCalculation)(benchmark::State& state) {
	for (auto _ : state) {
		for (int i = 0; i < Iterations; ++i) {
			calculator.updateVariable("VAR", i * 0.0001f);
			calculator.excecute(Expression);
		}
	}
}

BENCHMARK_F(MyFixture, preCalculation)(benchmark::State& state) {
	calc::Cache cache = calculator.preCalculate(Expression);
	for (auto _ : state) {
		for (int i = 0; i < Iterations; ++i) {
			calculator.updateVariable("VAR", i * 0.0001f);
			calculator.excecute(cache);
		}
	}
}

BENCHMARK_F(MyFixture, excecutePreCalculatedLongExpression)(benchmark::State& state) {
	std::string expression = "-1 * (11.2 * 12 / 123 * 10.4^2) * (11.2 * 12 / 123 * 10.4^2)*(11.2 * 12 / 123 * 10.4^2) * (11.2 * 12 / 123 * 10.4^2) - 12";
	calc::Cache cache = calculator.preCalculate(expression);
	
	for (auto _ : state) {
		for (int i = 0; i < Iterations; ++i) {
			calculator.excecute(cache);
		}
	}
}
