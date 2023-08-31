#ifndef OPERATORTOKEN_H
#define OPERATORTOKEN_H

#include <iostream>
#include <string_view>

#include "Token.h"

using namespace std::string_view_literals;

/**
* This class represents tokens objects that are operators between expressions
*/
class OperatorToken : public Token {
public:
	OperatorToken(std::string_view tokenName)
		: Token{ tokenName } {
	};
};

/**
* This class represents a "=" symbol.
*
* Not to be confused with the equality operator "==" or its token.
*/
class EqualsOpToken : public OperatorToken {
public:
	EqualsOpToken()
		: OperatorToken{ "="sv } {};
};

// === ARITHMETIC OPERATORS ===

/**
* This class represents tokens objects that are arithmetic operators between expressions
*/
class ArithmeticOpToken : public OperatorToken {
public:
	ArithmeticOpToken(std::string_view tokenName)
		: OperatorToken{ tokenName } {
	};
};

/**
* This class represents a "/" symbol.
*/
class DivideOpToken : public ArithmeticOpToken {
public:
	DivideOpToken()
		: ArithmeticOpToken{ "/"sv } {};
};

/**
* This class represents a "-" symbol.
*/
class MinusOpToken : public ArithmeticOpToken {
public:
	MinusOpToken()
		: ArithmeticOpToken{ "-"sv } {};
};

/**
* This class represents a "%" symbol.
*/
class ModuloOpToken : public ArithmeticOpToken {
public:
	ModuloOpToken()
		: ArithmeticOpToken{ "%"sv } {};
};

/**
* This class represents a "*" symbol.
*/
class MultiplyOpToken : public ArithmeticOpToken {
public:
	MultiplyOpToken()
		: ArithmeticOpToken{ "*"sv } {};
};

/**
* This class represents a "+" symbol.
*/
class PlusOpToken : public ArithmeticOpToken {
public:
	PlusOpToken()
		: ArithmeticOpToken{ "+"sv } {};
};

// === BOOlEAN OPERATORS ===

/**
* This class represents tokens objects that are boolean operators between expressions
*/
class BooleanOpToken : public OperatorToken {
public:
	BooleanOpToken(std::string_view tokenName)
		: OperatorToken{ tokenName } {
	};
};

/**
* This class represents a "&&" symbol.
*/
class AndOpToken : public BooleanOpToken {
public:
	AndOpToken()
		: BooleanOpToken{ "&&"sv } {};
};

/**
* This class represents a "!" symbol when used to invert boolean values
*/
class NotOpToken : public BooleanOpToken {
public:
	NotOpToken()
		: BooleanOpToken{ "!"sv } {};
};

/**
* This class represents a "||" symbol.
*/
class OrOpToken : public BooleanOpToken {
public:
	OrOpToken()
		: BooleanOpToken{ "||"sv } {};
};

// === RELATIONAL OPERATORS ===

/**
* This class represents tokens objects that are relational operators between expressions
*/
class RelationalOpToken : public OperatorToken {
public:
	RelationalOpToken(std::string_view tokenName)
		: OperatorToken{ tokenName } {
	};
};

/**
* This class represents a ">" symbol.
*/
class MoreThanOpToken : public RelationalOpToken {
public:
	MoreThanOpToken()
		: RelationalOpToken{ ">"sv } {};
};

/**
* This class represents a ">=" symbol.
*/
class MoreThanEqualOpToken : public RelationalOpToken {
public:
	MoreThanEqualOpToken()
		: RelationalOpToken{ ">="sv } {};
};

/**
* This class represents a "<" symbol.
*/
class LessThanOpToken : public RelationalOpToken {
public:
	LessThanOpToken()
		: RelationalOpToken{ "<"sv } {};
};

/**
* This class represents a "<=" symbol.
*/
class LessThanEqualOpToken : public RelationalOpToken {
public:
	LessThanEqualOpToken()
		: RelationalOpToken{ "<="sv } {};
};

/**
* This class represents a "!=" symbol.
*/
class InequalityOpToken : public RelationalOpToken {
public:
	InequalityOpToken()
		: RelationalOpToken{ "!="sv } {};
};

/**
* This class represents a "==" symbol.
*/
class EqualityOpToken : public RelationalOpToken {
public:
	EqualityOpToken()
		: RelationalOpToken{ "=="sv } {};
};

#endif
