#include "calculator.h"
#include "calculatorexception.h"

#include <sstream>
#include <stack>
#include <cmath>
#include <cassert>
#include <algorithm>

namespace {

	template <class... Args>
	std::string concatToString(Args&&... args) {
		std::stringstream stream;
		((stream << std::forward<Args>(args)), ...);
		return stream.str();
	}

	std::string charToString(char token) {
		return std::string(1, token);
	}

}

namespace calc {

	Calculator::Calculator() {
		initDefaultOperators();
	}

	Calculator::Calculator(Calculator&& other) noexcept
		: symbols_{std::move(other.symbols_)}
		, functions_{std::move(other.functions_)}
		, variableValues_{std::move(other.variableValues_)} {
		
		other.initDefaultOperators();
	}

	Calculator& Calculator::operator=(Calculator&& other) noexcept {
		symbols_ = std::move(other.symbols_);
		functions_ = std::move(other.functions_);
		variableValues_ = std::move(other.variableValues_);
		
		other.initDefaultOperators();
		return *this;
	}

	void Calculator::initDefaultOperators() {
		addOperator(UnaryMinus, 5, false, [](float a) {
			return -a;
		});
		addOperator(Plus, 2, true, [](float a, float b) {
			return a + b;
		});
		addOperator(Minus, 2, true, [](float a, float b) {
			return a - b;
		});
		addOperator(Division, 3, true, [](float a, float b) {
			return a / b;
		});
		addOperator(Multiplication, 3, true, [](float a, float b) {
			return a * b;
		});
		addOperator(Pow, 4, false, std::pow<float, float>);
		
		symbols_[","] = Comma::create();
		symbols_["("] = Paranthes::create(true);
		symbols_[")"] = Paranthes::create(false);
	}

	Cache Calculator::preCalculate(const std::string& infixNotation) const {
		std::list<Symbol> infix = transformToSymbols(infixNotation);
		return Cache{shuntingYardAlgorithm(infix)};
	}

	float Calculator::excecute(Cache cache) const {
		auto& postfix = cache.symbols_;
		const size_t size = postfix.size();
		if (size <= 0) {
			throw CalculatorException{"Empty math expression"};
		}
		for (unsigned int index = 0; index < size; ++index) {
			auto& symbol = postfix[index];
			const ExcecuteFunction* f = nullptr;
			switch (symbol.type) {
				case Type::Function:
					f = &functions_[symbol.function.index];
					[[fallthrough]];
				case Type::Operator:
				{
					if (f == nullptr) {
						f = &functions_[symbol.op.index];
					}
					int nbr = f->parameters_;
					std::array<float, ExcecuteFunction::MaxArgs> args;
					for (int j = index - 1; j >= 0 && nbr > 0; --j) { // Find function arguments and set associated symbols as used.
						if (postfix[j].type == Type::Float) {
							args[--nbr] = postfix[j].value.value;
							postfix[j].type = Type::Nothing; // Used now as argument.
						} else if (postfix[j].type == Type::Variable) {
							try {
								args[--nbr] = variableValues_.at(postfix[j].variable.index);
								postfix[j] = Nothing::create();  // Used now as argument.
							} catch (const std::out_of_range&) {
								throw CalculatorException{"Variable does not exist"};
							}
						}
					}
					if (nbr > 0) {
						throw CalculatorException{"Expression error"};
					}
					symbol.value = f->excecute(args);
					break;
				}
				default:
					// Continue the iteration!
					break;
			}
		}
		return postfix[size - 1].value.value;
	}

	float Calculator::excecute(const std::string& infixNotation) const {
		return excecute(preCalculate(infixNotation));
	}

	void Calculator::addVariable(const std::string& name, float value) {
		if (symbols_.end() != symbols_.find(name)) {
			throw CalculatorException{"Variable could not be added, already exist"};
		}
		symbols_[name] = Variable::create((static_cast<int8_t>(variableValues_.size())));
		variableValues_.push_back(value);
	}

	void Calculator::updateVariable(const std::string& name, float value) {
		try {
			Variable& var = symbols_.at(name).variable;
			if (var.type != Type::Variable) {
				throw CalculatorException{concatToString("Variable ", name, " can not be updated, is not a variable")};
			}
			variableValues_[var.index] = value;
		} catch (const std::out_of_range&) {
			throw CalculatorException{"Variable could not be updated, does not exist"};
		}
	}

	bool Calculator::hasSymbol(const std::string& name) const {
		return symbols_.end() != symbols_.find(name);
	}

	bool Calculator::hasFunction(const std::string& name) const {
		auto it = symbols_.find(name);
		if (symbols_.end() == it) {
			return false;
		}
		return it->second.type == Type::Function;
	}

	bool Calculator::hasOperator(char token) const {
		auto it = symbols_.find(std::string(1, token));
		if (symbols_.end() == it) {
			return false;
		}
		return it->second.type == Type::Operator;
	}

	bool Calculator::hasVariable(const std::string& name) const {
		auto it = symbols_.find(name);
		if (symbols_.end() == it) {
			return false;
		}
		return it->second.type == Type::Variable;
	}

	bool Calculator::hasFunction(const std::string& name, const std::string& infixNotation) const {
		Cache cache = preCalculate(infixNotation);
		return hasFunction(name, cache);
	}

	bool Calculator::hasFunction(const std::string& name, const Cache& cache) const {
		try {
			const Function& func = symbols_.at(name).function;
			if (func.type != Type::Function) {
				return false;
			}
			for (const Symbol& symbol : cache.symbols_) {
				if (symbol.type == Type::Function && symbol.function.index == func.index) {
					return true;
				}
			}
		} catch (const std::out_of_range&) {
			return false;
		}
		return false;
	}

	bool Calculator::hasOperator(char token, const std::string& infixNotation) const {
		return hasOperator(token, preCalculate(infixNotation));
	}

	bool Calculator::hasOperator(char token, const Cache& cache) const {
		try {
			const Operator& op = symbols_.at(std::string(1, token)).op;
			if (op.type != Type::Operator) {
				return false;
			}
			for (const Symbol& symbol : cache.symbols_) {
				if (symbol.type == Type::Operator && symbol.op.token == op.token) {
					return true;
				}
			}
		} catch (const std::out_of_range&) {
			return false;
		}
		return false;
	}

	bool Calculator::hasVariable(const std::string& name, const std::string& infixNotation) const {
		return hasVariable(name, preCalculate(infixNotation));
	}

	bool Calculator::hasVariable(const std::string& name, const Cache& cache) const {
		try {
			const auto& var = symbols_.at(name).variable;
			if (var.type != Type::Variable) {
				return false;
			}
			for (const auto& symbol : cache.symbols_) {
				if (symbol.type == Type::Variable && symbol.variable.index == var.index) {
					return true;
				}
			}
		} catch (const std::out_of_range&) {
			return false;
		}
		return false;
	}

	float Calculator::extractVariableValue(const std::string& name) const {
		try {
			const Variable& var = symbols_.at(name).variable;
			if (var.type != Type::Variable) {
				throw CalculatorException{"Variable does not exist"};
			}
			return variableValues_[var.index];
		} catch (const std::out_of_range&) {
			throw CalculatorException{"Variable does not exist"};
		}
	}

	std::string Calculator::addSpaceBetweenSymbols(const std::string& infixNotation) const {
		std::string text;
		for (char key : infixNotation) {
			std::string word;
			word += key;
			if (symbols_.end() != symbols_.find(word)) {
				text = text + " " + key + " ";
			} else {
				text += key;
			}
		}
		return text;
	}

	std::list<Symbol> Calculator::toSymbolList(const std::string& infixNotationWithSpaces) const {
		std::list<Symbol> infix;
		std::stringstream stream(infixNotationWithSpaces);
		std::string word;
		while (stream >> word) {
			auto it = symbols_.find(word);
			if (symbols_.end() != it) {
				// Symbol exists.
				infix.push_back(it->second);
			} else {
				// Assume unknown symbol is a value.
				float value;
				std::stringstream floatStream(word);

				if (floatStream >> value) {
					infix.push_back(Float::create(value));
				} else {
					throw CalculatorException{concatToString("Unrecognized symbol: ", word)};
				}
			}
		}
		return infix;
	}

	std::list<Symbol> Calculator::handleUnaryPlusMinusSymbol(const std::list<Symbol>& infix) const {
		Symbol lastSymbol = Nothing::create();
		std::list<Symbol> finalInfix;
		for (const Symbol& symbol : infix) {
			switch (symbol.type) {
				case Type::Operator:
					if (symbol.op.token == Minus) {
						if (lastSymbol.type == Type::Paranthes && lastSymbol.paranthes.left ||
							lastSymbol.type == Type::Operator ||
							lastSymbol.type == Type::Comma ||
							lastSymbol.type == Type::Nothing) {
							
							finalInfix.push_back(symbols_.find(UnaryMinusS)->second);
						} else {
							finalInfix.push_back(symbol);
						}
					} else if (symbol.op.token == Plus) {
						if (lastSymbol.type == Type::Paranthes && lastSymbol.paranthes.left ||
							lastSymbol.type == Type::Operator ||
							lastSymbol.type == Type::Comma ||
							lastSymbol.type == Type::Nothing) {
							// Skip symbol.
						} else {
							finalInfix.push_back(symbol);
						}
					} else {
						finalInfix.push_back(symbol);
					}
					break;
				default:
					finalInfix.push_back(symbol);
					break;
			}
			lastSymbol = symbol;
		}
		return finalInfix;
	}

	std::list<Symbol> Calculator::transformToSymbols(const std::string& infixNotation) const {
		std::string text = addSpaceBetweenSymbols(infixNotation);
		std::list<Symbol> infix = toSymbolList(text);
		return handleUnaryPlusMinusSymbol(infix);
	}

	void Calculator::addOperator(char token, char predence, bool leftAssociative,
		const std::function<float(float)>& function) {
		
		addOperator(token, predence, leftAssociative, 1, [=](float a, float b) {
			return function(a);
		});
	}

	void Calculator::addOperator(char token, char predence, bool leftAssociative,
		const std::function<float(float, float)>& function) {

		addOperator(token, predence, leftAssociative, 2, function);
	}

	void Calculator::addOperator(char token, char predence, bool leftAssociative,
		char parameters, const std::function<float(float, float)>& function) {

		auto str = charToString(token);
		
		if (symbols_.end() == symbols_.find(str)) {
			symbols_[str] = Operator::create(token, predence, leftAssociative, static_cast<uint8_t>(functions_.size()));
			functions_.push_back(ExcecuteFunction{parameters, function});
		}
	}

	void Calculator::addFunction(const std::string& name, const std::function<float(float)>& function) {
		addFunction(name, 1, [=](float a, float b) {
			return function(a);
		});
	}

	void Calculator::addFunction(const std::string& name, const std::function<float(float, float)>& function) {
		addFunction(name, 2, function);
	}

	void Calculator::addFunction(const std::string& name, char parameters, const std::function<float(float, float)>& function) {
		if (symbols_.end() == symbols_.find(name)) {
			symbols_[name] = Function::create(static_cast<uint8_t>(functions_.size()));
			functions_.push_back(ExcecuteFunction{parameters, function});
		}
	}

	std::vector<std::string> Calculator::getVariables() const {
		std::vector<std::string> variables;

		for (const auto& [name, symbol] : symbols_) {
			if (symbol.type == Type::Variable) {
				variables.push_back(name);
			}
		}
		return variables;
	}

	std::vector<Symbol> Calculator::shuntingYardAlgorithm(const std::list<Symbol>& infix) const {
		std::stack<Symbol> operatorStack;
		std::vector<Symbol> output;
		for (const Symbol& symbol : infix) {
			switch (symbol.type) {
				case Type::Variable:
					[[fallthrough]];
				case Type::Float:
					output.push_back(symbol);
					break;
				case Type::Function:
					operatorStack.push(symbol);
					break;
				case Type::Comma:
					while (operatorStack.size() > 0) {
						Symbol top = operatorStack.top();
						// Is a left paranthes?
						if (top.type == Type::Paranthes && top.paranthes.left) {
							break;
						} else { // Not a left paranthes.
							operatorStack.pop();
							output.push_back(top);
						}
					}
					break;
				case Type::Operator:
					// Empty the operator stack.
					while (operatorStack.size() > 0 && operatorStack.top().type == Type::Operator &&
						(((symbol.op.leftAssociative &&
							symbol.op.predence == operatorStack.top().op.predence)) ||
							(symbol.op.predence < operatorStack.top().op.predence))) {

						output.push_back(operatorStack.top());
						operatorStack.pop();
					}
					operatorStack.push(symbol);
					break;
				case Type::Paranthes:
					// Is left paranthes?
					if (symbol.paranthes.left) {
						operatorStack.push(symbol);
					} else { // Is right paranthes.
						if (operatorStack.size() < 1) {
							throw CalculatorException{"Missing right parameter '(' in expression"};
						}
						bool foundLeftParanthes = false;

						while (operatorStack.size() > 0) {
							auto topSymbol = operatorStack.top();
							operatorStack.pop();

							// Is a left paranthes?
							if (topSymbol.type == Type::Paranthes && topSymbol.paranthes.left) {
								foundLeftParanthes = true;
								break;
							} else {
								// 'top' is not a left paranthes.
								output.push_back(topSymbol);
							}
						}

						if (operatorStack.size() > 0 && operatorStack.top().type == Type::Function) {
							output.push_back(operatorStack.top());
							operatorStack.pop();
						}

						if (!foundLeftParanthes) {
							throw CalculatorException{"Error, mismatch of parantheses in expression"};
						}
					}
					break;
			}
		}

		if (!operatorStack.empty()) {
			while (operatorStack.size() > 0) {
				Symbol top = operatorStack.top();
				if (top.type == Type::Paranthes) {
					throw CalculatorException{"Error, mismatch of parantheses in expression"};
				}
				operatorStack.pop();
				output.push_back(top);
			}
		}
		return output;
	}

}
