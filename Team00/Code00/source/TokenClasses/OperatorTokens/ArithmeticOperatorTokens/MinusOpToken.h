#ifndef MINUSOPTOKEN_H
#define MINUSOPTOKEN_H

#include <iostream>
#include <string_view>

#include "ArithmeticOpToken.h"

/**
* This class represents a "-" symbol.
*/
class MinusOpToken : public ArithmeticOpToken {
public:
	MinusOpToken()
		: ArithmeticOpToken{ "-"sv } {};
};

#endif
