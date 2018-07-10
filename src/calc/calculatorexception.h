#ifndef CALC_CALCULATOREXCEPTION_H
#define CALC_CALCULATOREXCEPTION_H

#include "symbol.h"

#include <string>

namespace calc {	

	class CalculatorException {
	public:
		CalculatorException() {
		}

		CalculatorException(std::string error) : error_(error) {
		}
		
		std::string what() const {
			return error_;
		}

	private:
		std::string error_;
	};

} // Namespace calc.

#endif	// CALC_CALCULATOREXCEPTION_H
