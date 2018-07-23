#ifndef CALC_CALCULATOREXCEPTION_H
#define CALC_CALCULATOREXCEPTION_H

namespace calc {	

	class CalculatorException : public std::runtime_error {
	public:
		explicit CalculatorException() : std::runtime_error("") {
		}

		explicit CalculatorException(const char* error) : std::runtime_error(error) {
		}
	};

} // Namespace calc.

#endif	// CALC_CALCULATOREXCEPTION_H
