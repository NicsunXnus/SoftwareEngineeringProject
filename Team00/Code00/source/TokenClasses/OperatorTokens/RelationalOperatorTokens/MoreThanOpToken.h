#ifndef MORETHANOPTOKEN_H
#define MORETHANOPTOKEN_H

#include <iostream>
#include <string_view>

#include "RelationalOpToken.h"

/**
* This class represents a ">" symbol.
*/
class MoreThanOpToken : public RelationalOpToken {
public:
	MoreThanOpToken()
		: RelationalOpToken{ ">"sv } {};
};

#endif
