#ifndef SEMICOLONSEPTOKEN_H
#define SEMICOLONSEPTOKEN_H

#include <iostream>
#include <string_view>

#include "SeparatorToken.h"

using namespace std::string_view_literals;

/**
* This class represents a ";" symbol.
*/
namespace Tokens {
	class SemicolonSepToken : public SeparatorToken {
	public:
		SemicolonSepToken()
			: SeparatorToken{ ";"sv } {};
	};
}
#endif
