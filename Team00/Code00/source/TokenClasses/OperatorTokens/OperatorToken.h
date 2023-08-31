#ifndef OPERATORTOKEN_H
#define OPERATORTOKEN_H

#include <iostream>
#include <string_view>

#include "../Token.h"

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

#endif
