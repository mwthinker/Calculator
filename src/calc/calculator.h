#ifndef CALC_CALCULATOR_H
#define CALC_CALCULATOR_H

#include "symbol.h"
#include "cache.h"

#include <string>
#include <list>
#include <vector>
#include <array>
#include <functional>
#include <map>
#include <cassert>

namespace calc {

	class Calculator {
	public:
		friend class Cache;
		const char UNARY_MINUS = '~';
		const std::string UNARY_MINUS_S = "~";

		const char MINUS = '-';
		const char PLUS = '+';
		const char MULTIPLICATION = '*';
		const char DIVISION = '/';
		const char POW = '^';

		Calculator();

		Cache preCalculate(std::string infixNotation);
		
		float excecute(Cache cache);
		float excecute(std::string infixNotation);

		void addOperator(char token, char predence, bool leftAssociative,
			const std::function<float(float)>& function);

		void addOperator(char token, char predence, bool leftAssociative,
			const std::function<float(float, float)>& function);

		void addFunction(std::string name, const std::function<float(float)>& function);

		void addFunction(std::string name, const std::function<float(float, float)>& function);
		
		void addVariable(std::string name, float value);

		void updateVariable(std::string name, float value);

		bool hasSymbol(std::string name) const;
		bool hasFunction(std::string name) const;
		bool hasOperator(char token) const;
		bool hasVariable(std::string name) const;

		float extractVariableValue(std::string name) const;

		std::vector<std::string> getVariables() const;

	private:
		void addOperator(char token, char predence, bool leftAssociative,
			char parameters, const std::function<float(float, float)>& function);

		void addFunction(std::string name, char parameters,
			const std::function<float(float, float)>& function);

		std::string addSpaceBetweenSymbols(std::string infixNotation) const;
		std::list<Symbol> toSymbolList(std::string infixNotationWithSpaces);
		std::list<Symbol> handleUnaryPlusMinusSymbol(const std::list<Symbol>& infix);

		// Return a list of all symbols.
		std::list<Symbol> transformToSymbols(std::string infixNotation);

		std::vector<Symbol> shuntingYardAlgorithm(const std::list<Symbol>& infix);

		void initDefaultOperators();

		class ExcecuteFunction {
		public:
			static constexpr int MAX_ARGS = 2;

			ExcecuteFunction(char parameters, const std::function<float(float, float)>& function)
				: function_(function), parameters_(parameters) {
				assert(parameters > 0 && parameters <= 2);
			}

			inline Float excecute(const std::array<float, MAX_ARGS>& args) {
				return Float::create(function_(args[0], args[1]));
			}

			const char parameters_;
		private:
			const std::function<float(float, float)> function_;
		};

		std::map<std::string, Symbol> symbols_;
		std::vector<ExcecuteFunction> functions_;
		std::vector<float> variableValues_;
	};

} // Namespace calc;

#endif	// CALC_CALCULATOR_H
