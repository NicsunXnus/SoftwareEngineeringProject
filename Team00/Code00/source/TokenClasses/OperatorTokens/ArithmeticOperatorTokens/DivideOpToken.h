#ifndef DIVIDEOPTOKEN_H
#define DIVIDEOPTOKEN_H

#include <iostream>
#include <string_view>

#include "ArithmeticOpToken.h"

/**
* This class represents a "/" symbol.
*/
class DivideOpToken : public ArithmeticOpToken {
public:
	DivideOpToken()
		: ArithmeticOpToken{ "/"sv } {};
};

#endif
