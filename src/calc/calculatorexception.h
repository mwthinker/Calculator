#ifndef CALCULATOR_CALC_CALCULATOREXCEPTION_H
#define CALCULATOR_CALC_CALCULATOREXCEPTION_H

#include <stdexcept>

namespace calc {

	class CalculatorException : public std::runtime_error {
	public:
		explicit CalculatorException()
			: std::runtime_error{""} {
		}

		explicit CalculatorException(const char* error)
			: std::runtime_error{error} {
		}
	};

} // Namespace calc.

#endif	// CALCULATOR_CALC_CALCULATOREXCEPTION_H
