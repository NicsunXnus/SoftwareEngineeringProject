#ifndef PARENOPENSEPTOKEN_H
#define PARENOPENSEPTOKEN_H

#include <iostream>
#include <string_view>

#include "SeparatorToken.h"

using namespace std::string_view_literals;

/**
* This class represents a "(" symbol.
*/
class ParenOpenSepToken : public SeparatorToken {
public:
	ParenOpenSepToken()
		: SeparatorToken{ "("sv } {};
};

#endif
