#ifndef ARITHMETICOPTOKEN_H
#define ARITHMETICOPTOKEN_H

#include <iostream>
#include <string_view>

#include "../OperatorToken.h"

/**
* This class represents tokens objects that are arithmetic operators between expressions
*/
class ArithmeticOpToken : public OperatorToken {
public:
	ArithmeticOpToken(std::string_view tokenName)
		: OperatorToken{ tokenName } {
	};
};

#endif
