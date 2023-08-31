#ifndef BOOLEANOPTOKEN_H
#define BOOLEANOPTOKEN_H

#include <iostream>
#include <string_view>

#include "../OperatorToken.h"

/**
* This class represents tokens objects that are boolean operators between expressions
*/
class BooleanOpToken : public OperatorToken {
public:
	BooleanOpToken(std::string_view tokenName)
		: OperatorToken{ tokenName } {
	};
};

#endif
