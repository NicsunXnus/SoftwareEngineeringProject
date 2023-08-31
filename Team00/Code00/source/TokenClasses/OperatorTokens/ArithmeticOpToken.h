#ifndef ARITHMETICOPTOKEN_H
#define ARITHMETICOPTOKEN_H

#include <iostream>
#include <string_view>

#include "OperatorToken.h"

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

#endif
