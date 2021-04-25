#ifndef CALCULATOR_CALC_CACHE_H
#define CALCULATOR_CALC_CACHE_H

#include "symbol.h"

#include <vector>

namespace calc {

	class Cache {
	public:
		friend class Calculator;
		
		Cache() = default;
		
	private:
		explicit Cache(const std::vector<Symbol>& symbols);

		std::vector<Symbol> symbols_;
	};

}

#endif
