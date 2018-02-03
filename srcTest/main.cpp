#include <iostream>
#include <string>
#include <functional>
#include <cassert>
#include <cmath>

#include "calc/calculator.h"
#include "calc/calculatorexception.h"

const float DELTA = 0.001f;

bool equal(float a, float b) {
	return (a + DELTA > b) && (a - DELTA < b);
}

float addTwo(float a, float notUsed) {
	return a + 2;
}

float multiply(float a, float b) {
	return a * b;
}

void testExceptions() {
	calc::Calculator calculator;
	try {
		// Missing last paranthes.
		calculator.excecute("2.1+-3.2*5^(3-1)/(2*3.14 - 1");
		assert(false);
	} catch (calc::CalculatorException) {
		// Do nothing, is supposed to fail.
	}

	try {
		// Missing last parameter.
		calculator.excecute("2.1+-3.2*5^(3-1)/(2*3.14 - 1) + VAR");
		assert(false);
	} catch (calc::CalculatorException) {
		// Do nothing, is supposed to fail.
	}

	try {
		// Empty expression.
		calculator.excecute("");
		assert(false);
	} catch (calc::CalculatorException) {
		// Do nothing, is supposed to fail.
	}

	calculator.addVariable("VAR", 0.5f);
	try {
		// Add same variable again.
		calculator.addVariable("VAR", 1.5f);
		assert(false);
	} catch (calc::CalculatorException) {
		// Do nothing, is supposed to fail.
	}

	try {
		// Update unexisted unknown variable.
		calculator.updateVariable("VAR2", 1.5f);
		assert(false);
	} catch (calc::CalculatorException) {
		// Do nothing, is supposed to fail.
	}

	calculator.addFunction("pow", 2, [](float a, float b) {
		return std::pow(a, b);
	});
	try {
		// Update used function name
		calculator.updateVariable("pow", 1.5f);
		assert(false);
	} catch (calc::CalculatorException) {
		// Do nothing, is supposed to fail.
	}

}

int main() {
	// Test 1.
	// Expression is evaluated.
	{
		calc::Calculator calculator;
		std::string expression = "2.1+-3.2*5^(3-1)/(2*3.14 - 1)";
		const float answer = -13.0515151515151515f;
		float value = calculator.excecute(expression);
		std::cout << expression << "=" << value << "\n";
		// Test expression!
		assert(equal(answer, value));
	}

	// Test 2.
	// Expression is evaluated with added constants.
	{
		calc::Calculator calculator;
		calculator.addVariable("PI", 3.14f);
		calculator.addVariable("TWO", 2);
		calculator.addVariable("FIVE", 5);
		std::string expression = "2.1+-3.2*FIVE^(3-1)/(TWO*PI - 1)";
		const float answer = -13.0515151515151515f;
		float value = calculator.excecute(expression);
		std::cout << expression << "=" << value << "\n";

		// Test expression!
		assert(equal(answer, value));
	}

	// Test 2.
	// Expression is evaluated with added constants and functions.
	{
		calc::Calculator calculator;
		calculator.addVariable("PI", 3.14f);
		calculator.addVariable("TWO", 2);
		calculator.addVariable("FIVE", 5);
		calculator.addFunction("addTwo", 1, std::bind(addTwo, std::placeholders::_1, std::placeholders::_2));
		calculator.addFunction("multiply", 2, std::bind(multiply, std::placeholders::_1, std::placeholders::_2));

		std::string expression = "multiply(addTwo(2.1+-3.2*FIVE^(3-1)/(TWO*PI - 1)), 8.1)";
		const float answer = (-13.0515151515151515f + 2)*8.1f;
		float value = calculator.excecute(expression);
		std::cout << expression << "=" << value << "\n";

		// Test expression!
		assert(equal(answer, value));
	}

	// Test 2.
	// Expression is evaluated with added constants and functions.
	{
		calc::Calculator calculator;
		calculator.addVariable("pi", 3.14159265359f);
		calculator.addFunction("exp", 1, [](float a, float b) {
			return std::exp(a);
		});
		calculator.addFunction("sin", 1, [](float a, float b) {
			return std::sin(a);
		});
		calculator.addFunction("cos", 1, [](float a, float b) {
			return std::cos(a);
		});
		calculator.addFunction("ln", 1, [](float a, float b) {
			return std::log(a);
		});
		calculator.addFunction("log", 1, [](float a, float b) {
			return std::log10(a);
		});
		calculator.addFunction("pow", 2, [](float a, float b) {
			return std::pow(a, b);
		});

		assert(equal(calculator.excecute("exp(1.11)"), 3.034358f));
		assert(equal(calculator.excecute("sin( cos(90*pi / 180))"), 0.000001f));
		assert(equal(calculator.excecute("34.5*(23+1.5)/2"), 422.625000f));
		assert(equal(calculator.excecute("5 + ((1 + 2) * 4) - 3"), 14));
		assert(equal(calculator.excecute("( 1 + 2 ) * ( 3 / 4 ) ^ ( 5 + 6 )"), 0.126705f));
		assert(equal(calculator.excecute("3/2 + 4*(12+3)"), 61.5f));
		assert(equal(calculator.excecute("pi*pow(9/2,2)"), 63.617197f));
		assert(equal(calculator.excecute("((+2*(6-1))/2)*4"), 20));
		assert(equal(calculator.excecute("ln(2)+3^5"), 243.693147f));
		assert(equal(calculator.excecute("11 ^ -7"), 5.13158f*std::pow(10.f, -8.f)));
		assert(equal(calculator.excecute("cos ( ( 1.3 + 1 ) ^ ( 1 / 3 ) ) - log ( -2 * 3 / -14 )"), 0.616143f));
		assert(equal(calculator.excecute("1 * -sin( pi / 2) "), -1));
		assert(equal(calculator.excecute("1*-8 ++ 5"), -3));
		assert(equal(calculator.excecute("1 - (-(2^2)) - 1"), 4));
	}
	{
		calc::Calculator calculator;
		calculator.addVariable("a", 1);
		calculator.addVariable("b", 2);
		calculator.addVariable("c", 3);

		assert(calculator.getVariables().size() == 3);
		auto vars = calculator.getVariables();
		assert(vars[0] == "a");
		assert(vars[1] == "b");
		assert(vars[2] == "c");

		assert(equal(calculator.excecute("a + b + c"), 6));
		calculator.updateVariable("a", 2);
		calculator.updateVariable("b", 4);
		assert(equal(calculator.excecute("a + b + c"), 9));
		calculator.updateVariable("c", 6);
		assert(equal(calculator.excecute("a + b + c"), 12));
		calc::Cache cache = calculator.preCalculate("a + b + c");
		assert(equal(calculator.excecute(cache), 12));
		calculator.updateVariable("a", 1);
		calculator.updateVariable("b", 2);
		calculator.updateVariable("c", 3);
		assert(equal(calculator.excecute(cache), 6));

		calc::Calculator calculator2;
		try {
			calculator2.excecute(cache);
			assert(false);
		} catch (calc::CalculatorException) {
			// Do nothing, is supposed to fail.
		}
	}

	testExceptions();

	std::cout << "\nAll tests succeeded!\n";

	return 0;
}
