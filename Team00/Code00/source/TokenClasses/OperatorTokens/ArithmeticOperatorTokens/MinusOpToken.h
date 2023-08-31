#ifndef MINUSOPTOKEN_H
#define MINUSOPTOKEN_H

#include <iostream>
#include <string_view>

#include "ArithmeticOpToken.h"

using namespace std::string_view_literals;

/**
* This class represents a "-" symbol.
*/
namespace Tokens {
	class MinusOpToken : public ArithmeticOpToken {
	public:
		MinusOpToken()
			: ArithmeticOpToken{ "-"sv } {};
	};
}
#endif
