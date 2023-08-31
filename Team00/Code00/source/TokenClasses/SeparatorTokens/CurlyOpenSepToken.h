#ifndef CURLYOPENSEPTOKEN_H
#define CURLYOPENSEPTOKEN_H

#include <iostream>
#include <string_view>

#include "SeparatorToken.h"

using namespace std::string_view_literals;

/**
* This class represents a "{" symbol.
*/
namespace Tokens {
	class CurlyOpenSepToken : public SeparatorToken {
	public:
		CurlyOpenSepToken()
			: SeparatorToken{ "{"sv } {};
	};
}
#endif
