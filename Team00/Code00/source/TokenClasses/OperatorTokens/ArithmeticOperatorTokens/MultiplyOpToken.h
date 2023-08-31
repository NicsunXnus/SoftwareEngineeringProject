#ifndef MULTIPLYOPTOKEN_H
#define MULTIPLYOPTOKEN_H

#include <iostream>
#include <string_view>

#include "ArithmeticOpToken.h"

using namespace std::string_view_literals;

/**
* This class represents a "*" symbol.
*/
namespace Tokens {
	class MultiplyOpToken : public ArithmeticOpToken {
	public:
		MultiplyOpToken()
			: ArithmeticOpToken{ "*"sv } {};
	};
}
#endif
