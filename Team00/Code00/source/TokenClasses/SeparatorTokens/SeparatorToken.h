#ifndef SEPARATORTOKEN_H
#define SEPARATORTOKEN_H

#include <iostream>
#include <string_view>

#include "../Token.h"

/**
* This class represents tokens objects that stem from user-defined names.
*/
namespace Tokens {
	class SeparatorToken : public Token {
	private:
	public:
		SeparatorToken(std::string_view tokenName)
			: Token{ tokenName } {
		};
	};
}
#endif
