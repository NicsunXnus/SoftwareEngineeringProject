#ifndef MULTIPLYOPTOKEN_H
#define MULTIPLYOPTOKEN_H

#include <iostream>
#include <string_view>

#include "ArithmeticOpToken.h"

/**
* This class represents a "*" symbol.
*/
class MultiplyOpToken : public ArithmeticOpToken {
public:
	MultiplyOpToken()
		: ArithmeticOpToken{ "*"sv } {};
};

#endif
