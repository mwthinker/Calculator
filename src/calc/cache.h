#ifndef CALC_CACHE_H
#define CALC_CACHE_H

#include "symbol.h"

#include <vector>

namespace calc {	

	class Cache {
	public:
		friend class Calculator;
		
		Cache() = default;
		
	private:
		Cache(const std::vector<Symbol>& symbols);

		std::vector<Symbol> symbols_;
	};

} // Namespace calc.

#endif	// CALC_CACHE_H
