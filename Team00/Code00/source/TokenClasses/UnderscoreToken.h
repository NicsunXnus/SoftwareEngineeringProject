#ifndef UNDERSCORETOKEN_H
#define UNDERSCORETOKEN_H

#include <iostream>
#include <string_view>

#include "Token.h"

using namespace std::string_view_literals;

/**
* This class represents a "_" symbol.
*/
class UnderscoreToken : public Token {
public:
	UnderscoreToken()
		: Token{ "_"sv } {};
};

#endif
