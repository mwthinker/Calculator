#ifndef CALCULATOR_CALC_SYMBOL_H
#define CALCULATOR_CALC_SYMBOL_H

#include <cstdint>

namespace calc {

	enum class Type : char {
		Operator,
		Float,
		Function,
		Paranthes,
		Comma,
		Variable,
		Nothing
	};

	union Symbol;

	struct Operator {
		static Symbol create(char token, int8_t predence, bool leftAssociative, char index);

		Type type;
		char token;
		int8_t predence;
		bool leftAssociative;
		char index;
	};

	struct Paranthes {
		static Symbol create(bool left);

		Type type;
		bool left;
	};

	struct Float {
		static Symbol create(float value);

		Type type;
		float value;
	};

	struct Function {
		static Symbol create(char index);

		Type type;
		char index;
	};

	struct Comma {
		static Symbol create();

		Type type;
	};

	struct Variable {
		static Symbol create(char index);

		Type type;
		char index;
	};

	struct Nothing {
		static Symbol create();

		Type type;
	};

	union Symbol {
		Type type;
		Operator op;
		Paranthes paranthes;
		Float value;
		Function function;
		Comma comma;
		Variable variable;
		Nothing nothing;
	};

}

#endif
