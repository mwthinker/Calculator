// main() provided by Catch in file main.cpp.
#include <catch2/catch.hpp>

#include <calc/calculator.h>
#include <calc/calculatorexception.h>

TEST_CASE("Testing math functions", "[functions][sin][cos][log][exp][pi]") {
	INFO("Testing math functions");

	calc::Calculator calculator;
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
	
	SECTION("test math expressions containing functions") {
		REQUIRE(( calculator.excecute("exp(1.11)") == Approx(3.034358f) ));
		REQUIRE(( calculator.excecute("sin( cos(90*pi / 180))") == Approx(0.0f).margin(0.001f)));
		REQUIRE(( calculator.excecute("34.5*(23+1.5)/2") == Approx(422.625000f) ));
		REQUIRE(( calculator.excecute("5 + ((1 + 2) * 4) - 3") == Approx(14) ));
		REQUIRE(( calculator.excecute("( 1 + 2 ) * ( 3 / 4 ) ^ ( 5 + 6 )") == Approx(0.126705f) ));
		REQUIRE(( calculator.excecute("3/2 + 4*(12+3)") == Approx(61.5f) ));
		REQUIRE(( calculator.excecute("pi*pow(9/2,2)") == Approx(63.617197f) ));
		REQUIRE(( calculator.excecute("((+2*(6-1))/2)*4") == Approx(20) ));
		REQUIRE(( calculator.excecute("ln(2)+3^5") == Approx(243.693147f) ));
		REQUIRE(( calculator.excecute("11 ^ -7") == Approx(5.13158f*std::pow(10.f, -8.f) )));
		REQUIRE(( calculator.excecute("cos ( ( 1.3 + 1 ) ^ ( 1 / 3 ) ) - log ( -2 * 3 / -14 )") == Approx(0.616143f) ));
		REQUIRE(( calculator.excecute("1 * -sin( pi / 2) ") == Approx(-1) ));
		REQUIRE(( calculator.excecute("1*-8 ++ 5") == Approx(-3) ));
		REQUIRE(( calculator.excecute("1 - (-(2^2)) - 1") == Approx(4) ));
	}
}

TEST_CASE("Testing math expressions", "[expressions]") {
	calc::Calculator calculator;

	// Check operator available
	calculator = calc::Calculator();
	sizeof(calc::Calculator);
	
	calc::Calculator calculator2 = calculator; // Test copy constructor, is compilable.


	SECTION("Expression is evaluated") {
		std::string expression = "2.1+-3.2*5^(3-1)/(2*3.14 - 1)";
		const float answer = -13.0515151515151515f;
		float value = calculator.excecute(expression);
		
		REQUIRE(( calculator.excecute(expression) == Approx(-13.0515151515151515f) ));
	}

	SECTION("Expression is evaluated with added constants") {
		calculator.addVariable("PI", 3.14f);
		calculator.addVariable("TWO", 2);
		calculator.addVariable("FIVE", 5);
		std::string expression = "2.1+-3.2*FIVE^(3-1)/(TWO*PI - 1)";
		
		REQUIRE(( calculator.excecute(expression) == Approx(-13.0515151515151515f) ));
	}

	SECTION("Expression is evaluated with added constants and functions") {
		calculator.addVariable("PI", 3.14f);
		calculator.addVariable("TWO", 2);
		calculator.addVariable("FIVE", 5);
		
		calculator.addFunction("addTwo", [] (float a) {
			return a + 2;
		});
		calculator.addFunction("multiply", [](float a, float b) {
			return a  * b;
		});

		std::string expression = "multiply(addTwo(2.1+-3.2*FIVE^(3-1)/(TWO*PI - 1)), 8.1)";
		const float answer = (-13.0515151515151515f + 2) * 8.1f;
		REQUIRE(( calculator.excecute(expression) == Approx(answer) ));
	}

	SECTION("Add variables and change values") {
		calculator.addVariable("a", 1);
		calculator.addVariable("b", 2);
		calculator.addVariable("c", 3);

		REQUIRE(( calculator.getVariables().size() == 3 ));
		auto vars = calculator.getVariables();
		REQUIRE(( vars[0] == "a" ));
		REQUIRE(( vars[1] == "b" ));
		REQUIRE(( vars[2] == "c" ));

		REQUIRE(( calculator.excecute("a + b + c") == 6 ));
		calculator.updateVariable("a", 2);
		calculator.updateVariable("b", 4);
		REQUIRE(( calculator.excecute("a + b + c") == 9 ));
		calculator.updateVariable("c", 6);
		REQUIRE(( calculator.excecute("a + b + c") == 12 ));
		calc::Cache cache = calculator.preCalculate("a + b + c");
		REQUIRE(( calculator.excecute(cache) == 12 ));
		calculator.updateVariable("a", 1);
		calculator.updateVariable("b", 2);
		calculator.updateVariable("c", 3);
		REQUIRE(( calculator.excecute(cache) == 6 ));

		REQUIRE((calculator.extractVariableValue("a") == 1));
		REQUIRE((calculator.extractVariableValue("b") == 2));
		REQUIRE((calculator.extractVariableValue("c") == 3));
	}

	SECTION("Check function, symbol, varible existence") {
		calculator.addVariable("VAR", 1);
		calculator.addFunction("pow", [] (float a, float b) {
			return std::pow(a, b);
		});
		
		REQUIRE(( calculator.hasFunction("pow") ));
		REQUIRE(( calculator.hasSymbol("pow") ));
		REQUIRE(( !calculator.hasVariable("pow") ));

		REQUIRE(( !calculator.hasFunction("NO_POW") ));
		REQUIRE(( !calculator.hasVariable("NO_POW") ));
		REQUIRE(( !calculator.hasSymbol("NO_POW") ));
		REQUIRE(( !calculator.hasFunction("NO_POW") ));

		REQUIRE(( !calculator.hasFunction("{") ));
		REQUIRE(( !calculator.hasVariable("{") ));
		REQUIRE(( !calculator.hasSymbol("{") ) );
		REQUIRE(( !calculator.hasOperator('{') ));

		REQUIRE(( calculator.hasVariable("VAR") ));
		REQUIRE(( calculator.hasSymbol("VAR") ));
		REQUIRE(( !calculator.hasFunction("VAR") ));

		REQUIRE(( calculator.hasOperator('+') ));
		REQUIRE(( calculator.hasSymbol("+") ));
		REQUIRE(( !calculator.hasFunction("+") ));
		REQUIRE(( !calculator.hasVariable("+") ));
	}

}

TEST_CASE("Check symbol functions", "[cache]") {
	calc::Calculator calculator;
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

	SECTION("Variable available in cache") {
		REQUIRE(calculator.hasVariable("PI"));
		REQUIRE(calculator.hasVariable("PI", cache));
	}
	SECTION("Variable available in expression") {
		REQUIRE(calculator.hasVariable("PI"));
		REQUIRE(calculator.hasVariable("PI", expr));
	}
	SECTION("Variable not available in cache") {
		REQUIRE(calculator.hasVariable("EPSILON"));
		REQUIRE(!calculator.hasVariable("EPSILON", cache));
		
		REQUIRE(!calculator.hasVariable("pow", cache));
		REQUIRE(!calculator.hasVariable("+", cache));
	}

	SECTION("Variable not available in expression") {
		REQUIRE(!calculator.hasVariable("EPSILON", expr));
		REQUIRE(!calculator.hasVariable("pow", expr));
		REQUIRE(!calculator.hasVariable("+", expr));
	}

	SECTION("Function available in cache") {
		REQUIRE(calculator.hasFunction("pow"));
		REQUIRE(calculator.hasFunction("pow", cache));
	}
	SECTION("Function available in expression") {
		REQUIRE(calculator.hasFunction("pow"));
		REQUIRE(calculator.hasFunction("pow", expr));
	}
	SECTION("Function not available in cache") {
		REQUIRE(calculator.hasFunction("POWER"));
		REQUIRE(!calculator.hasFunction("POWER", expr));
	}
	SECTION("Function not available in expression") {
		REQUIRE(calculator.hasFunction("POWER"));
		REQUIRE(!calculator.hasFunction("POWER", expr));
	}

	SECTION("Operator available in cache") {
		REQUIRE(calculator.hasOperator('*'));
		REQUIRE(calculator.hasOperator('*', cache));
	}
	SECTION("Operator available in expression") {
		REQUIRE(calculator.hasOperator('*'));
		REQUIRE(calculator.hasOperator('*', expr));
	}
	SECTION("Operator not available in cache") {
		REQUIRE(calculator.hasOperator('-'));
		REQUIRE(!calculator.hasOperator('-', expr));
	}
	SECTION("Operator not available in expression") {
		REQUIRE(calculator.hasOperator('-'));
		REQUIRE(!calculator.hasOperator('-', expr));
	}
}

TEST_CASE("Testing exceptions", "[exceptions]") {
	INFO("Testing exceptions");
	
	// Testing exception available.
	calc::CalculatorException e;
	e.what();

	SECTION("Missing last paranthes") {
		calc::Calculator calculator;
		REQUIRE_NOTHROW(calculator.excecute("2.1+-3.2*5^(3-1)/(2*3.14 - 1)"));
		REQUIRE_THROWS_AS(calculator.excecute("2.1+-3.2*5^(3-1)/(2*3.14 - 1"), calc::CalculatorException);
	}
	
	SECTION("Missing last parameter.") {
		calc::Calculator calculator;
		const std::string EXPR = "2.1+-3.2*5^(3-1)/(2*3.14 - 1) + VAR";
		REQUIRE_THROWS_AS(calculator.excecute(EXPR), calc::CalculatorException);
		calculator.addVariable("VAR", 12);
		REQUIRE_NOTHROW(calculator.excecute(EXPR));
	}
	
	SECTION("Empty expression") {
		calc::Calculator calculator;
		REQUIRE_THROWS_AS(calculator.excecute(""), calc::CalculatorException);		
	}

	SECTION("Add same variable again") {
		calc::Calculator calculator;
		REQUIRE_NOTHROW(calculator.addVariable("VAR", 1.5f));
		REQUIRE_THROWS_AS(calculator.addVariable("VAR", 1.5f), calc::CalculatorException);
	}
	

	SECTION("Update unexisted unknown variable") {
		calc::Calculator calculator;
		REQUIRE_THROWS_AS(calculator.updateVariable("VAR2", 1.5f), calc::CalculatorException);
	}
	
	SECTION("Update used function name") {
		calc::Calculator calculator;
		REQUIRE_NOTHROW(calculator.addFunction("pow", [](float a, float b) {
			return std::pow(a, b);
		}));
		REQUIRE_THROWS_AS(calculator.updateVariable("pow", 1.5f), calc::CalculatorException);
	}

	SECTION("Missing variable in cache") {
		calc::Calculator calculator;
		REQUIRE_NOTHROW(calculator.addVariable("VAR", 1.5f));
		calc::Cache cache = calculator.preCalculate("VAR + 2");
		calc::Calculator calculator2;
		REQUIRE_THROWS_AS(calculator2.excecute(cache), calc::CalculatorException);
	}

	SECTION("No variable found") {
		calc::Calculator calculator;
		REQUIRE_NOTHROW(calculator.addVariable("VAR", 1.5f));
		REQUIRE_THROWS_AS(calculator.extractVariableValue("NO_VARIABLE"), calc::CalculatorException);
	}

	SECTION("No variable found, function with same name exist") {
		calc::Calculator calculator;
		REQUIRE_NOTHROW(calculator.addFunction("pow", [](float a, float b) {
			return std::pow(a, b);
		}));
		REQUIRE_THROWS_AS(calculator.extractVariableValue("pow"), calc::CalculatorException);
	}
}
