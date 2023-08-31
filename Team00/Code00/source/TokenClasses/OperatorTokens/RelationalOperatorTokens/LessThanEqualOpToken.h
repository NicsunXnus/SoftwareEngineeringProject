#ifndef LESSTHANEQUALOPTOKEN_H
#define LESSTHANEQUALOPTOKEN_H

#include <iostream>
#include <string_view>

#include "RelationalOpToken.h"

/**
* This class represents a "<=" symbol.
*/
class LessThanEqualOpToken : public RelationalOpToken {
public:
	LessThanEqualOpToken()
		: RelationalOpToken { "<="sv } {};
};

#endif
