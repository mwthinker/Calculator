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
		// Unary minus sign operator.
		addOperator('~', 5, false, 1, [](float a, float b) {
			return -a;
		});
		addOperator('+', 2, true, 2, [](float a, float b) {
			return a + b;
		});
		addOperator('-', 2, true, 2, [](float a, float b) {
			return a - b;
		});
		addOperator('/', 3, true, 2, [](float a, float b) {
			return a / b;
		});
		addOperator('*', 3, true, 2, [](float a, float b) {
			return a * b;
		});
		addOperator('^', 4, false, 2, [](float a, float b) {
			return std::pow(a, b);
		});

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
		std::vector<Symbol>& prefix = cache.symbols_;
		int size = prefix.size();
		for (int index = 0; index < size; ++index) {
			Symbol& symbol = prefix[index];
			ExcecuteFunction* f = nullptr;
			switch (symbol.type_) {
				case Type::FUNCTION:
					f = &functions_[symbol.function_.index_];
					// Fall through!
				case Type::OPERATOR:
				{
					if (f == nullptr) {
						// Is an operator, not a function!
						f = &functions_[symbol.operator_.index_];
					}
					int nbr = f->parameters_;

					// Symbols not already used?
					for (int j = index - 1; j >= 0 && nbr > 0; --j) {
						if (prefix[j].type_ == Type::FLOAT) {
							// Set the parameter value.
							f->param_[--nbr] = prefix[j].float_.value_;
							prefix[j].type_ = Type::NOTHING;
						} else if (prefix[j].type_ == Type::VARIABLE) {
							try {
								// Set the parameter value.
								f->param_[--nbr] = variableValues_.at(prefix[j].variable_.index_);
								prefix[j].type_ = Type::NOTHING;
							} catch (std::out_of_range ex) {
								throw CalculatorException("Variable does not exist");
							}
						}
					}
					if (nbr > 0) {
						auto it = std::find_if(symbols_.begin(), symbols_.end(), [&](const std::pair<std::string, Symbol>& pair) {
							if (symbol.type_ == Type::FUNCTION) {
								return pair.second.function_.index_ == symbol.function_.index_;
							} else { // Is a operator!
								return pair.second.operator_.token_ == symbol.operator_.index_;
							}
						});
						if (it != symbols_.end()) {
							throw CalculatorException(std::string("Variable does not exist, ")
								+ it->first + std::string(" missing enough parameters"));
						} else {
							throw CalculatorException("Unrecognized symbol");
						}
						return 0;
					}
					float value = f->excecute();
					symbol.float_ = Float::create(value);
					break;
				}
				default:
					// Continue the iteration!
					break;
			}
		}

		if (size > 0) {
			return prefix[size - 1].float_.value_;
		} else {
			throw CalculatorException("Empty math expression");
		}
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
			variableValues_.at(symbols_[name].variable_.index_) = value;
		} catch (std::out_of_range ex) {
			throw CalculatorException("Variable could not be updated, does not exist");
		}
	}

	// Add space between all "symbols" 
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
				// Is a available symbol?
				infix.push_back(symbols_[word]);
			} else {
				float value;
				std::stringstream floatStream(word);

				if (floatStream >> value) { // Is a number?
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

	std::list<Symbol> Calculator::handleUnaryPlusMinusSymbol(std::list<Symbol>& infix) {
		Symbol lastSymbol;
		lastSymbol.nothing_ = Nothing::create();
		std::list<Symbol> finalInfix;
		for (Symbol& symbol : infix) {
			switch (symbol.type_) {
				case Type::OPERATOR:
					if (symbol.operator_.token_ == '-') {
						if (lastSymbol.type_ == Type::PARANTHES && lastSymbol.paranthes_.left_) {
							finalInfix.push_back(symbols_["~"]);
						} else if (lastSymbol.type_ == Type::OPERATOR) {
							finalInfix.push_back(symbols_["~"]);
						} else if (lastSymbol.type_ == Type::COMMA) {
							finalInfix.push_back(symbols_["~"]);
						} else if (lastSymbol.type_ == Type::NOTHING) {
							finalInfix.push_back(symbols_["~"]);
						} else {
							finalInfix.push_back(symbol);
						}
					} else if (symbol.operator_.token_ == '+') {
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

	Calculator::ExcecuteFunction::ExcecuteFunction(char parameters, const std::function<float(float, float)>& function)
		: function_(function), parameters_(parameters) {
	}

	float Calculator::ExcecuteFunction::excecute() {
		return function_(param_[0], param_[1]);
	}

} // Namespace calc;
