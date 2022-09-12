#include <calc/calculator.h>
#include <calc/calculatorexception.h>

#include <gtest/gtest.h>

#include <cmath>

constexpr float ErrorPrecision = 0.001f;

class CalculatorTest : public ::testing::Test {
protected:

	CalculatorTest() {
		// Check operator available
		auto calculator = calc::Calculator();
		sizeof(calc::Calculator);

		calc::Calculator calculator2 = calculator; // Test copy constructor, is compilable.

		// Testing exception available.
		calc::CalculatorException e{"Test"};
		const auto what = e.what();
	}

	~CalculatorTest() override {}

	void SetUp() override {}

	void TearDown() override {}
};

TEST_F(CalculatorTest, addNewMathFunction) {
	// Given.
	calc::Calculator calculator;

	// When
	calculator.addVariable("pi", 3.14159265359f);
	calculator.addFunction("exp", [](float a) {
		return std::exp(a);
	});
	calculator.addFunction("sin", [](float a) {
		return std::sin(a);
	});
	calculator.addFunction("cos", [](float a) {
		return std::cos(a);
	});
	calculator.addFunction("ln", [](float a) {
		return std::log(a);
	});
	calculator.addFunction("log", [](float a) {
		return std::log10(a);
	});
	calculator.addFunction("pow", [](float a, float b) {
		return std::pow(a, b);
	});

	// Then.
	EXPECT_NEAR(3.034358, calculator.excecute("exp(1.11)"), ErrorPrecision);
	EXPECT_NEAR(0.f, calculator.excecute("sin( cos(90*pi / 180))"), ErrorPrecision);
	EXPECT_NEAR(422.625000, calculator.excecute("34.5*(23+1.5)/2"), ErrorPrecision);
	EXPECT_NEAR(14.0, calculator.excecute("5 + ((1 + 2) * 4) - 3"), ErrorPrecision);
	EXPECT_NEAR(0.126705, calculator.excecute("( 1 + 2 ) * ( 3 / 4 ) ^ ( 5 + 6 )"), ErrorPrecision);
	EXPECT_NEAR(61.5, calculator.excecute("3/2 + 4*(12+3)"), ErrorPrecision);
	EXPECT_NEAR(63.617197, calculator.excecute("pi*pow(9/2,2)"), ErrorPrecision);
	EXPECT_NEAR(20.0, calculator.excecute("((+2*(6-1))/2)*4"), ErrorPrecision);
	EXPECT_NEAR(243.693147, calculator.excecute("ln(2)+3^5"), ErrorPrecision);
	EXPECT_NEAR(5.13158f * std::pow(10.f, -8.f), calculator.excecute("11 ^ -7"), ErrorPrecision);
	EXPECT_NEAR(0.616143, calculator.excecute("cos ( ( 1.3 + 1 ) ^ ( 1 / 3 ) ) - log ( -2 * 3 / -14 )"), ErrorPrecision);
	EXPECT_NEAR(-1.0, calculator.excecute("1 * -sin( pi / 2) "), ErrorPrecision);
	EXPECT_NEAR(-3, calculator.excecute("1*-8 ++ 5"), ErrorPrecision);
	EXPECT_NEAR(4.0, calculator.excecute("1 - (-(2^2)) - 1"), ErrorPrecision);
}

TEST_F(CalculatorTest, testMathExpression) {
	// Given
	calc::Calculator calculator;
	std::string expression = "2.1+-3.2*5^(3-1)/(2*3.14 - 1)";
	const float answer = -13.0515151515151515f;

	// When
	float value = calculator.excecute(expression);

	// Then
	EXPECT_NEAR(answer, value, ErrorPrecision);
}

TEST_F(CalculatorTest, testConstantsInExpression) {
	// Given
	calc::Calculator calculator;
	calculator.addVariable("PI", 3.14f);
	calculator.addVariable("TWO", 2);
	calculator.addVariable("FIVE", 5);
	std::string expression = "2.1+-3.2*FIVE^(3-1)/(TWO*PI - 1)";
	const float answer = -13.05151515f;

	// When
	float value = calculator.excecute(expression);

	// Then
	EXPECT_NEAR(answer, value, ErrorPrecision);
}

TEST_F(CalculatorTest, testConstantsAndFunctionsInExpression) {
	// Given
	calc::Calculator calculator;
	calculator.addVariable("PI", 3.14f);
	calculator.addVariable("TWO", 2);
	calculator.addVariable("FIVE", 5);
	calculator.addFunction("addTwo", [](float a) {
		return a + 2;
	});
	calculator.addFunction("multiply", [](float a, float b) {
		return a * b;
	});

	std::string expression = "multiply(addTwo(2.1+-3.2*FIVE^(3-1)/(TWO*PI - 1)), 8.1)";
	const float answer = (-13.0515151515151515f + 2) * 8.1f;

	// When
	float value = calculator.excecute(expression);

	// Then
	EXPECT_NEAR(answer, value, ErrorPrecision);
}

TEST_F(CalculatorTest, testVariablesAndChangeValuesInExpression) {
	// Given
	calc::Calculator calculator;
	calculator.addVariable("a", 1);
	calculator.addVariable("b", 2);
	calculator.addVariable("c", 3);

	// When/Then
	auto vars = calculator.getVariables();
	EXPECT_EQ(3, calculator.getVariables().size());
	EXPECT_EQ("a", vars[0]);
	EXPECT_EQ("b", vars[1]);
	EXPECT_EQ("c", vars[2]);
	EXPECT_NEAR(6.0f, calculator.excecute("a + b + c"), ErrorPrecision);
	
	calculator.updateVariable("a", 2);
	calculator.updateVariable("b", 4);
	EXPECT_NEAR(9.0f, calculator.excecute("a + b + c"), ErrorPrecision);

	calculator.updateVariable("c", 6);
	EXPECT_NEAR(12.0f, calculator.excecute("a + b + c"), ErrorPrecision);


	calc::Cache cache = calculator.preCalculate("a + b + c");
	EXPECT_NEAR(12.0f, calculator.excecute("a + b + c"), ErrorPrecision);

	calculator.updateVariable("a", 1);
	calculator.updateVariable("b", 2);
	calculator.updateVariable("c", 3);
	EXPECT_NEAR(6.0f, calculator.excecute(cache), ErrorPrecision);

	EXPECT_NEAR(1.0f, calculator.extractVariableValue("a"), ErrorPrecision);
	EXPECT_NEAR(2.0f, calculator.extractVariableValue("b"), ErrorPrecision);
	EXPECT_NEAR(3.0f, calculator.extractVariableValue("c"), ErrorPrecision);
}

TEST_F(CalculatorTest, checkFunctionAndSymbolAndVariable) {
	// Given
	calc::Calculator calculator;
	calculator.addVariable("VAR", 1);
	calculator.addFunction("pow", [](float a, float b) {
		return std::pow(a, b);
	});

	// When/Then
	EXPECT_TRUE(calculator.hasFunction("pow"));
	EXPECT_TRUE(calculator.hasSymbol("pow"));
	EXPECT_FALSE(calculator.hasVariable("pow"));

	EXPECT_FALSE(calculator.hasFunction("NO_POW"));
	EXPECT_FALSE(calculator.hasSymbol("NO_POW"));
	EXPECT_FALSE(calculator.hasVariable("NO_POW"));

	EXPECT_FALSE(calculator.hasFunction("{"));
	EXPECT_FALSE(calculator.hasSymbol("{"));
	EXPECT_FALSE(calculator.hasVariable("{"));
	EXPECT_FALSE(calculator.hasOperator('{'));

	EXPECT_FALSE(calculator.hasFunction("VAR"));
	EXPECT_TRUE(calculator.hasSymbol("VAR"));
	EXPECT_TRUE(calculator.hasVariable("VAR"));

	EXPECT_FALSE(calculator.hasFunction("+"));
	EXPECT_TRUE(calculator.hasSymbol("+"));
	EXPECT_FALSE(calculator.hasVariable("+"));
	EXPECT_TRUE(calculator.hasOperator('+'));
}

TEST_F(CalculatorTest, testSymbolFunctionUsingCache) {
	// Given
	calc::Calculator calculator;

	// When/then
	calculator.addVariable("PI", 3.14f);
	calculator.addVariable("EPSILON", 1234.f);
	calculator.addFunction("pow", [](float a, float b) {
		return std::pow(a, b);
	});
	calculator.addFunction("POWER", [](float a, float b) {
		return std::pow(a, b);
	});

	const std::string expr = "5 * 3 + 2.1^PI + 1 + pow(0.5, 2)";
	const calc::Cache cache = calculator.preCalculate(expr);

	EXPECT_TRUE(calculator.hasVariable("PI"));
	EXPECT_TRUE(calculator.hasVariable("PI", cache));
	EXPECT_TRUE(calculator.hasVariable("PI", expr));	

	EXPECT_FALSE(calculator.hasVariable("pow"));
	EXPECT_FALSE(calculator.hasVariable("pow", cache));
	EXPECT_FALSE(calculator.hasVariable("pow", expr));
	EXPECT_FALSE(calculator.hasVariable("+"));
	EXPECT_FALSE(calculator.hasVariable("+", cache));
	EXPECT_FALSE(calculator.hasVariable("+", expr));

	EXPECT_TRUE(calculator.hasFunction("POWER"));
	EXPECT_FALSE(calculator.hasFunction("POWER", cache));
	EXPECT_FALSE(calculator.hasFunction("POWER", expr));

	EXPECT_TRUE(calculator.hasOperator('*'));
	EXPECT_TRUE(calculator.hasOperator('*', cache));
	EXPECT_TRUE(calculator.hasOperator('*', expr));

	EXPECT_TRUE(calculator.hasOperator('-'));
	EXPECT_FALSE(calculator.hasOperator('-', cache));
	EXPECT_FALSE(calculator.hasOperator('-', expr));
}

TEST_F(CalculatorTest, missingParanthes) {
	calc::Calculator calculator;
	
	EXPECT_THROW({
		calculator.excecute("2.1+-3.2*5^(3-1)/(2*3.14 - 1");
	}, calc::CalculatorException);
}

TEST_F(CalculatorTest, missingParameter) {
	calc::Calculator calculator;
	const auto expr = "2.1+-3.2*5^(3-1)/(2*3.14 - 1) + VAR";
	
	EXPECT_THROW({
		calculator.excecute(expr);
	}, calc::CalculatorException);

	EXPECT_NO_THROW({
		calculator.addVariable("VAR", 10);
		calculator.excecute(expr);
	});
}

TEST_F(CalculatorTest, addSameVariableMultipleTimes) {
	calc::Calculator calculator;

	EXPECT_NO_THROW({
		calculator.addVariable("VAR", 1.5f);
	});

	EXPECT_THROW({
		calculator.addVariable("VAR", 1.5f);
	}, calc::CalculatorException);
}

TEST_F(CalculatorTest, emptyExpression) {
	calc::Calculator calculator;

	EXPECT_THROW({
		calculator.excecute("");
	}, calc::CalculatorException);
}

TEST_F(CalculatorTest, updateNonExistingVariable) {
	calc::Calculator calculator;

	EXPECT_THROW({
		calculator.updateVariable("VAR2", 1.5f);
	}, calc::CalculatorException);
}

TEST_F(CalculatorTest, updateUsedFunctionAsVariable) {
	calc::Calculator calculator;

	EXPECT_NO_THROW({
		calculator.addFunction("pow", [](float a, float b) {
			return std::pow(a, b);
		});
	});

	EXPECT_THROW({
		calculator.updateVariable("pow", 1.5f);
	}, calc::CalculatorException);
}

TEST_F(CalculatorTest, missingVariableInCache) {
	calc::Calculator calculator;

	calc::Cache cache;
	EXPECT_NO_THROW({
		calculator.addVariable("VAR", 1.5f);
		cache = calculator.preCalculate("VAR + 2");
	});

	calc::Calculator calculator2;
	EXPECT_THROW({
		calculator2.excecute(cache);
	}, calc::CalculatorException);
}

TEST_F(CalculatorTest, noVariableFound) {
	calc::Calculator calculator;

	EXPECT_NO_THROW({
		calculator.addVariable("VAR", 1.5f);
	});

	EXPECT_THROW({
		calculator.extractVariableValue("NO_VARIABLE");
	}, calc::CalculatorException);
}

TEST_F(CalculatorTest, tryExtractVariableFromFunction) {
	calc::Calculator calculator;

	EXPECT_NO_THROW({
		calculator.addFunction("pow", [](float a, float b) {
			return std::pow(a, b);
		});
	});

	EXPECT_THROW({
		calculator.extractVariableValue("pow");
	}, calc::CalculatorException);
}
