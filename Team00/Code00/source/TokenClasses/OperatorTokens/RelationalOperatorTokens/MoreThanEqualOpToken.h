#ifndef MORETHANEQUALOPTOKEN_H
#define MORETHANEQUALOPTOKEN_H

#include <iostream>
#include <string_view>

#include "RelationalOpToken.h"

/**
* This class represents a ">=" symbol.
*/
class MoreThanEqualOpToken : public RelationalOpToken {
public:
	MoreThanEqualOpToken()
		: RelationalOpToken{ ">="sv } {};
};

#endif
