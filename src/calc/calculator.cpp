#include "calculator.h"
#include "calculatorexception.h"

#include <sstream>
#include <stack>
#include <cmath>
#include <cassert>
#include <algorithm>

namespace calc {

	Calculator::Calculator() {
		initDefaultOperators();
	}

	void Calculator::initDefaultOperators() {
		addOperator(UNARY_MINUS, 5, false, [](float a) {
			return -a;
		});
		addOperator(PLUS, 2, true, [](float a, float b) {
			return a + b;
		});
		addOperator(MINUS, 2, true, [](float a, float b) {
			return a - b;
		});
		addOperator(DIVISION, 3, true, [](float a, float b) {
			return a / b;
		});
		addOperator(MULTIPLICATION, 3, true, [](float a, float b) {
			return a * b;
		});
		addOperator(POW, 4, false, std::pow<float, float>);

		Comma c = Comma::create();
		Symbol symbol;
		symbol.comma_ = c;
		symbols_[","] = symbol;
		Paranthes p = Paranthes::create(true);
		symbol.paranthes_ = p;
		symbols_["("] = symbol;
		p = Paranthes::create(false);
		symbol.paranthes_ = p;
		symbols_[")"] = symbol;
	}

	Cache Calculator::preCalculate(std::string infixNotation) {
		std::list<Symbol> infix = transformToSymbols(infixNotation);
		return Cache(shuntingYardAlgorithm(infix));
	}

	float Calculator::excecute(Cache cache) {
		std::vector<Symbol>& postfix = cache.symbols_;
		const int size = postfix.size();
		if (size <= 0) {
			throw CalculatorException("Empty math expression");
		}
		for (int index = 0; index < size; ++index) {
			Symbol& symbol = postfix[index];
			ExcecuteFunction* f = nullptr;
			switch (symbol.type_) {
				case Type::FUNCTION:
					f = &functions_[symbol.function_.index_];
					// Fall through!
				case Type::OPERATOR:
				{
					if (f == nullptr) {
						f = &functions_[symbol.operator_.index_];
					}
					int nbr = f->parameters_;
					std::array<float, ExcecuteFunction::MAX_ARGS> args;
					for (int j = index - 1; j >= 0 && nbr > 0; --j) { // Find function arguments and set associated symbols as used.
						if (postfix[j].type_ == Type::FLOAT) {
							args[--nbr] = postfix[j].float_.value_;
							postfix[j].type_ = Type::NOTHING; // Used now as argument.
						} else if (postfix[j].type_ == Type::VARIABLE) {
							try {
								args[--nbr] = variableValues_.at(postfix[j].variable_.index_);
								postfix[j].type_ = Type::NOTHING;  // Used now as argument.
							} catch (std::out_of_range ex) {
								throw CalculatorException("Variable does not exist");
							}
						}
					}
					if (nbr > 0) {
						throw CalculatorException("Expression error");
					}
					symbol.float_ = f->excecute(args);
					break;
				}
				default:
					// Continue the iteration!
					break;
			}
		}
		return postfix[size - 1].float_.value_;
	}

	float Calculator::excecute(std::string infixNotation) {
		Cache cache = preCalculate(infixNotation);
		return excecute(cache);
	}

	void Calculator::addVariable(std::string name, float value) {
		// Function name not used?
		if (symbols_.end() == symbols_.find(name)) {
			Variable v = Variable::create((char) variableValues_.size());
			variableValues_.push_back(value);
			Symbol symbol;
			symbol.variable_ = v;
			symbols_[name] = symbol;
		} else {
			throw CalculatorException("Variable could not be added, already exist");
		}
	}

	void Calculator::updateVariable(std::string name, float value) {
		try {
			Variable& var = symbols_.at(name).variable_;
			if (var.type_ != Type::VARIABLE) {
				throw CalculatorException(std::string("Variable ") + name + std::string(" can not be updated, is not a variable"));
			}
			variableValues_[var.index_] = value;
		} catch (std::out_of_range ex) {
			throw CalculatorException("Variable could not be updated, does not exist");
		}
	}

	bool Calculator::hasSymbol(std::string name) const {
		return symbols_.end() != symbols_.find(name);
	}

	bool Calculator::hasFunction(std::string name) const {
		auto it = symbols_.find(name);
		if (symbols_.end() == it) {
			return false;
		} else {
			return it->second.type_ == Type::FUNCTION;
		}
	}

	bool Calculator::hasOperator(char token) const {
		auto it = symbols_.find(std::string(1, token));
		if (symbols_.end() == it) {
			return false;
		} else {
			return it->second.type_ == Type::OPERATOR;
		}
	}

	bool Calculator::hasVariable(std::string name) const {
		auto it = symbols_.find(name);
		if (symbols_.end() == it) {
			return false;
		} else {
			return it->second.type_ == Type::VARIABLE;
		}
	}

	float Calculator::extractVariableValue(std::string name) const {
		try {
			const Variable& var = symbols_.at(name).variable_;
			if (var.type_ != Type::VARIABLE) {
				throw CalculatorException("Variable does not exist");
			}
			return variableValues_[var.index_];
		} catch (std::out_of_range ex) {
			throw CalculatorException("Variable does not exist");
		}
	}

	std::string Calculator::addSpaceBetweenSymbols(std::string infixNotation) const {
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

	std::list<Symbol> Calculator::toSymbolList(std::string infixNotationWithSpaces) {
		std::list<Symbol> infix;
		std::stringstream stream(infixNotationWithSpaces);
		std::string word;
		while (stream >> word) {
			if (symbols_.end() != symbols_.find(word)) {
				// Symbol exists.
				infix.push_back(symbols_[word]);
			} else {
				// Assume unknown symbol is a value.
				float value;
				std::stringstream floatStream(word);

				if (floatStream >> value) {
					Symbol symbol;
					symbol.float_ = Float::create(value);
					infix.push_back(symbol);
				} else {
					throw CalculatorException(std::string("Unrecognized symbol: ") + word);
				}
			}
		}
		return infix;
	}

	std::list<Symbol> Calculator::handleUnaryPlusMinusSymbol(const std::list<Symbol>& infix) {
		Symbol lastSymbol;
		lastSymbol.nothing_ = Nothing::create();
		std::list<Symbol> finalInfix;
		for (const Symbol& symbol : infix) {
			switch (symbol.type_) {
				case Type::OPERATOR:
					if (symbol.operator_.token_ == MINUS) {
						if (lastSymbol.type_ == Type::PARANTHES && lastSymbol.paranthes_.left_) {
							finalInfix.push_back(symbols_[UNARY_MINUS_S]);
						} else if (lastSymbol.type_ == Type::OPERATOR) {
							finalInfix.push_back(symbols_[UNARY_MINUS_S]);
						} else if (lastSymbol.type_ == Type::COMMA) {
							finalInfix.push_back(symbols_[UNARY_MINUS_S]);
						} else if (lastSymbol.type_ == Type::NOTHING) {
							finalInfix.push_back(symbols_[UNARY_MINUS_S]);
						} else {
							finalInfix.push_back(symbol);
						}
					} else if (symbol.operator_.token_ == PLUS) {
						if (lastSymbol.type_ == Type::PARANTHES && lastSymbol.paranthes_.left_) {
							// Skip symbol.
						} else if (lastSymbol.type_ == Type::OPERATOR) {
							// Skip symbol.
						} else if (lastSymbol.type_ == Type::COMMA) {
							// Skip symbol.
						} else if (lastSymbol.type_ == Type::NOTHING) {
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

	// Return a list of all symbols.
	std::list<Symbol> Calculator::transformToSymbols(std::string infixNotation) {
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

		std::stringstream stream;
		stream << token;

		// Function name not used?
		if (symbols_.end() == symbols_.find(stream.str())) {
			Operator op = Operator::create(token, predence, leftAssociative, (char) functions_.size());
			Symbol symbol;
			symbol.operator_ = op;
			symbols_[stream.str()] = symbol;
			functions_.push_back(ExcecuteFunction(parameters, function));
		}
	}

	void Calculator::addFunction(std::string name, const std::function<float(float)>& function) {
		addFunction(name, 1, [=](float a, float b) {
			return function(a);
		});
	}

	void Calculator::addFunction(std::string name, const std::function<float(float, float)>& function) {
		addFunction(name, 2, function);
	}

	void Calculator::addFunction(std::string name, char parameters, const std::function<float(float, float)>& function) {
		// Function name not used?
		if (symbols_.end() == symbols_.find(name)) {
			Function f = Function::create((char) functions_.size());
			Symbol symbol;
			symbol.function_ = f;
			symbols_[name] = symbol;
			functions_.push_back(ExcecuteFunction(parameters, function));
		}
	}

	std::vector<std::string> Calculator::getVariables() const {
		std::vector<std::string> variables;

		for (const auto& pair : symbols_) {
			if (pair.second.type_ == Type::VARIABLE) {
				variables.push_back(pair.first);
			}
		}
		return variables;
	}

	std::vector<Symbol> Calculator::shuntingYardAlgorithm(const std::list<Symbol>& infix) {
		std::stack<Symbol> operatorStack;
		std::vector<Symbol> output;
		for (const Symbol& symbol : infix) {
			switch (symbol.type_) {
				case Type::VARIABLE:
					// Fall through!
				case Type::FLOAT:
					output.push_back(symbol);
					break;
				case Type::FUNCTION:
					operatorStack.push(symbol);
					break;
				case Type::COMMA:
					while (operatorStack.size() > 0) {
						Symbol top = operatorStack.top();
						// Is a left paranthes?
						if (top.type_ == Type::PARANTHES && top.paranthes_.left_) {
							break;
						} else { // Not a left paranthes.
							operatorStack.pop();
							output.push_back(top);
						}
					}
					break;
				case Type::OPERATOR:
					// Empty the operator stack.
					while (operatorStack.size() > 0 && operatorStack.top().type_ == Type::OPERATOR &&
						(((symbol.operator_.leftAssociative_ &&
							symbol.operator_.predence_ == operatorStack.top().operator_.predence_)) ||
							(symbol.operator_.predence_ < operatorStack.top().operator_.predence_))) {

						output.push_back(operatorStack.top());
						operatorStack.pop();
					}
					operatorStack.push(symbol);
					break;
				case Type::PARANTHES:
					// Is left paranthes?
					if (symbol.paranthes_.left_) {
						operatorStack.push(symbol);
					} else { // Is right paranthes.
						if (operatorStack.size() < 1) {
							throw CalculatorException("Missing right parameter '(' in expression");
						}
						bool foundLeftParanthes = false;

						while (operatorStack.size() > 0) {
							Symbol top = operatorStack.top();
							operatorStack.pop();

							// Is a left paranthes?
							if (top.type_ == Type::PARANTHES && top.paranthes_.left_) {
								foundLeftParanthes = true;
								break;
							} else {
								// 'top' is not a left paranthes.
								output.push_back(top);
							}
						}

						if (operatorStack.size() > 0 && operatorStack.top().type_ == Type::FUNCTION) {
							output.push_back(operatorStack.top());
							operatorStack.pop();
						}

						if (!foundLeftParanthes) {
							throw CalculatorException("Error, mismatch of parantheses in expression");
						}
					}
					break;
			}
		}

		if (!operatorStack.empty()) {
			while (operatorStack.size() > 0) {
				Symbol top = operatorStack.top();
				if (top.type_ == Type::PARANTHES) {
					throw CalculatorException("Error, mismatch of parantheses in expression");
				}
				operatorStack.pop();
				output.push_back(top);
			}
		}
		return output;
	}

} // Namespace calc.
