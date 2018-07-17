#ifndef CALC_CALCULATOREXCEPTION_H
#define CALC_CALCULATOREXCEPTION_H

#include "symbol.h"

#include <string>

namespace calc {	

	class CalculatorException : std::exception {
	public:
		explicit CalculatorException() = default;

		explicit CalculatorException(std::string error) : error_(error) {
		}

		virtual const char* what() const override {
			return error_.c_str();
		}
	private:
		std::string error_;
	};

} // Namespace calc.

#endif	// CALC_CALCULATOREXCEPTION_H
