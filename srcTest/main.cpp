#include <iostream>
#include <string>
#include <functional>
#include <cassert>

#include "calc\calculator.h"

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

int main() {
	// Test 1.
	// Expression is evaluated.
	{
		calc::Calculator calculator;
		std::string expression = "2.1+~3.2*5^(3-1)/(2*3.14 - 1)";
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
		std::string expression = "2.1+~3.2*FIVE^(3-1)/(TWO*PI - 1)";
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

		std::string expression = "multiply(addTwo(2.1+~3.2*FIVE^(3-1)/(TWO*PI - 1)), 8.1)";
		const float answer = (-13.0515151515151515f + 2)*8.1f;
		float value = calculator.excecute(expression);
		std::cout << expression << "=" << value << "\n";
		// Test expression!
		assert(equal(answer, value));
	}

	std::cout << "\nAll tests succeeded!\n";

	return 0;
}