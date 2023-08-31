#ifndef PLUSOPTOKEN_H
#define PLUSOPTOKEN_H

#include <iostream>
#include <string_view>

#include "ArithmeticOpToken.h"

using namespace std::string_view_literals;

/**
* This class represents a "+" symbol.
*/
class PlusOpToken : public ArithmeticOpToken {
public:
	PlusOpToken()
		: ArithmeticOpToken{ "+"sv } {};
};

#endif
