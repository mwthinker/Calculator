#ifndef CALCULATOR_CALC_CALCULATOR_H
#define CALCULATOR_CALC_CALCULATOR_H

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
		static constexpr char UnaryMinus = '~';
		static const std::string UnaryMinusS; // Defined to UnaryMinus;

		static constexpr char Minus = '-';
		static constexpr char Plus = '+';
		static constexpr char Multiplication = '*';
		static constexpr char Division = '/';
		static constexpr char Pow = '^';

		Calculator();

		Calculator(const Calculator&) = default;
		Calculator& operator=(const Calculator&) = default;

		Calculator(Calculator&&) noexcept;
		Calculator& operator=(Calculator&&) noexcept;

		Cache preCalculate(const std::string& infixNotation) const;
		
		float excecute(Cache cache) const;
		float excecute(std::string infixNotation) const;

		void addOperator(char token, char predence, bool leftAssociative,
			const std::function<float(float)>& function);

		void addOperator(char token, char predence, bool leftAssociative,
			const std::function<float(float, float)>& function);

		void addFunction(const std::string& name, const std::function<float(float)>& function);

		void addFunction(const std::string& name, const std::function<float(float, float)>& function);
		
		void addVariable(const std::string& name, float value);

		void updateVariable(const std::string& name, float value);

		bool hasSymbol(const std::string& name) const;
		bool hasFunction(const std::string& name) const;
		bool hasOperator(char token) const;
		
		bool hasVariable(const std::string& name) const;
		
		bool hasFunction(const std::string& name, const Cache& cache) const;
		bool hasFunction(const std::string& name, const std::string& infix) const;
		
		bool hasOperator(char token, const Cache& cache) const;
		bool hasOperator(char token, const std::string& infix) const;

		bool hasVariable(const std::string& name, const std::string& infix) const;
		bool hasVariable(const std::string& name, const Cache& cache) const;

		float extractVariableValue(const std::string& name) const;

		std::vector<std::string> getVariables() const;

	private:
		void addOperator(char token, char predence, bool leftAssociative,
			char parameters, const std::function<float(float, float)>& function);

		void addFunction(std::string name, char parameters,
			const std::function<float(float, float)>& function);

		std::string addSpaceBetweenSymbols(const std::string& infixNotation) const;
		std::list<Symbol> toSymbolList(const std::string& infixNotationWithSpaces) const;
		std::list<Symbol> handleUnaryPlusMinusSymbol(const std::list<Symbol>& infix) const;

		// Return a list of all symbols.
		std::list<Symbol> transformToSymbols(const std::string& infixNotation) const;

		std::vector<Symbol> shuntingYardAlgorithm(const std::list<Symbol>& infix) const;

		void initDefaultOperators();

		class ExcecuteFunction {
		public:
			static constexpr int MAX_ARGS{2};

			ExcecuteFunction(char parameters, const std::function<float(float, float)>& function)
				: function_(function), parameters_(parameters) {
				assert(parameters > 0 && parameters <= 2);
			}

			Float excecute(const std::array<float, MAX_ARGS>& args) const {
				return Float::create(function_(args[0], args[1]));
			}

			const char parameters_{};
		private:
			const std::function<float(float, float)> function_;
		};

		std::map<std::string, Symbol> symbols_;
		std::vector<ExcecuteFunction> functions_;
		std::vector<float> variableValues_;
	};

}

#endif
