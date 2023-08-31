#ifndef DOUBLEQUOTATIONSEPTOKEN_H
#define DOUBLEQUOTATIONSEPTOKEN_H

#include <iostream>
#include <string_view>

#include "SeparatorToken.h"

using namespace std::string_view_literals;

/**
* This class represents a " symbol.
*/
namespace Tokens {
	class DoubleQuoSepToken : public SeparatorToken {
	public:
		DoubleQuoSepToken()
			: SeparatorToken{ '"'sv } {};
	};
}
#endif
