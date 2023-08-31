#ifndef EQUALSOPTOKEN_H
#define EQUALSOPTOKEN_H

#include <iostream>
#include <string_view>

#include "OperatorToken.h"

using namespace std::string_view_literals;

/**
* This class represents a "=" symbol.
* 
* Not to be confused with the equality operator "==" or its token.
*/
namespace Tokens {
	class EqualsOpToken : public OperatorToken {
	public:
		EqualsOpToken()
			: OperatorToken{ "="sv } {};
	};
}
#endif
