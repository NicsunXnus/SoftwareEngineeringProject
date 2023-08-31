#ifndef NOTOPTOKEN_H
#define NOTOPTOKEN_H

#include <iostream>
#include <string_view>

#include "BooleanOpToken.h"

using namespace std::string_view_literals;

/**
* This class represents a "!" symbol when used to invert boolean values
*/
class NotOpToken : public BooleanOpToken {
public:
	NotOpToken()
		: BooleanOpToken{ "!"sv } {};
};

#endif
