#ifndef ANDOPTOKEN_H
#define ANDOPTOKEN_H

#include <iostream>
#include <string_view>

#include "BooleanOpToken.h"

/**
* This class represents a "&&" symbol.
*/
class AndOpToken : public BooleanOpToken {
public:
	AndOpToken()
		: BooleanOpToken{ "&&"sv } {};
};

#endif
