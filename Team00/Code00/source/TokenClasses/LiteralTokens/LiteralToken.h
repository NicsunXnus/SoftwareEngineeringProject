#ifndef LITERALTOKEN_H
#define LITERALTOKEN_H

#include <iostream>
#include <string_view>

#include "../Token.h"

/**
* This class represents tokens objects that are constants or literals
*/
namespace Tokens {
	class LiteralToken : public Token {
	private:
	public:
		LiteralToken(std::string_view tokenName)
			: Token{ tokenName } {
		};
	};
}
#endif
