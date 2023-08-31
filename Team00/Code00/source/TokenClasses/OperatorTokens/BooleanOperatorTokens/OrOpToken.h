#ifndef OROPTOKEN_H
#define OROPTOKEN_H

#include <iostream>
#include <string_view>

#include "BooleanOpToken.h"

using namespace std::string_view_literals;

/**
* This class represents a "||" symbol.
*/
class OrOpToken : public BooleanOpToken {
public:
	OrOpToken()
		: BooleanOpToken{ "||"sv } {};
};

#endif
