#ifndef CURLYCLOSESEPTOKEN_H
#define CURLYCLOSESEPTOKEN_H

#include <iostream>
#include <string_view>

#include "SeparatorToken.h"

using namespace std::string_view_literals;

/**
* This class represents a "}" symbol.
*/
namespace Tokens {
	class CurlyCloseSepToken : public SeparatorToken {
	public:
		CurlyCloseSepToken()
			: SeparatorToken{ "}"sv } {};
	};
}
#endif
