#ifndef OPERATORTOKEN_H
#define OPERATORTOKEN_H

#include <iostream>
#include <string_view>

#include "../Token.h"

/**
* This class represents tokens objects that are operators between expressions
*/
namespace Tokens {
	class OperatorToken : public Token {
	public:
		OperatorToken(std::string_view tokenName)
			: Token{ tokenName } {
		};
	};
}
#endif
