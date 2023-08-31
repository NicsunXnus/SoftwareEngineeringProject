#ifndef DIVIDEOPTOKEN_H
#define DIVIDEOPTOKEN_H

#include <iostream>
#include <string_view>

#include "ArithmeticOpToken.h"

using namespace std::string_view_literals;

/**
* This class represents a "/" symbol.
*/
namespace Tokens {
	class DivideOpToken : public ArithmeticOpToken {
	public:
		DivideOpToken()
			: ArithmeticOpToken{ "/"sv } {};
	};
}
#endif
