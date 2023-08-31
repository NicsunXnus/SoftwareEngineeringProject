#ifndef OROPTOKEN_H
#define OROPTOKEN_H

#include <iostream>
#include <string_view>

#include "BooleanOpToken.h"

/**
* This class represents a "||" symbol.
*/
class OrOpToken : public BooleanOpToken {
public:
	OrOpToken()
		: BooleanOpToken{ "||"sv } {};
};

#endif
