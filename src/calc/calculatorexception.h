#ifndef CALCULATOR_CALC_CALCULATOREXCEPTION_H
#define CALCULATOR_CALC_CALCULATOREXCEPTION_H

#include <stdexcept>

namespace calc {

	class CalculatorException : public std::runtime_error {
	public:
		explicit CalculatorException(const std::string& error)
			: std::runtime_error{error} {
		}
	};

}

#endif
