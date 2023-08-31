#ifndef BOOLEANOPTOKEN_H
#define BOOLEANOPTOKEN_H

#include <iostream>
#include <string_view>

#include "OperatorToken.h"

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

#endif
