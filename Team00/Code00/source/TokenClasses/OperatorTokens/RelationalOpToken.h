#ifndef RELATIONALOPTOKEN_H
#define RELATIONALOPTOKEN_H

#include <iostream>
#include <string_view>

#include "OperatorToken.h"

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
