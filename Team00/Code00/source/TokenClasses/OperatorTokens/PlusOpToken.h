#ifndef MINUSOPTOKEN_H
#define MINUSOPTOKEN_H

#include <iostream>
#include <string_view>

#include "OperatorToken.h"

/**
* This class represents a "-" symbol.
*/
class MinusOpToken : public OperatorToken {
private:
public:
	MinusOpToken()
		: OperatorToken { "-"sv } {};
};

#endif
