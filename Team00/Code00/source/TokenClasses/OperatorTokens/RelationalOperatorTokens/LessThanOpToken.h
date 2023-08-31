#ifndef LESSTHANOPTOKEN_H
#define LESSTHANOPTOKEN_H

#include <iostream>
#include <string_view>

#include "RelationalOpToken.h"

/**
* This class represents a "<" symbol.
*/
class LessThanOpToken : public RelationalOpToken {
public:
	LessThanOpToken()
		: RelationalOpToken{ "<"sv } {};
};

#endif
