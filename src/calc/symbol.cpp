#include "symbol.h"

namespace calc {

	Symbol Operator::create(char token, int8_t predence, bool leftAssociative, char index) {
		Symbol s;
		s.op.type = Type::Operator;
		s.op.token = token;
		s.op.predence = predence;
		s.op.leftAssociative = leftAssociative;
		s.op.index = index;
		return s;
	}

	Symbol Paranthes::create(bool left) {
		Symbol s;
		s.paranthes.type = Type::Paranthes;
		s.paranthes.left = left;
		return s;
	}

	Symbol Float::create(float value) {
		Symbol s;
		s.value.type = Type::Float;
		s.value.value = value;
		return s;
	}

	Symbol Function::create(char index) {
		Symbol s;
		s.function.type = Type::Function;
		s.function.index = index;
		return s;
	}

	Symbol Comma::create() {
		Symbol s;
		s.comma.type = Type::Comma;
		return s;
	}

	Symbol Variable::create(char index) {
		Symbol s;
		s.variable.type = Type::Variable;
		s.variable.index = index;
		return s;
	}

	Symbol Nothing::create() {
		Symbol s;
		s.type = Type::Nothing;
		return s;
	}

}
