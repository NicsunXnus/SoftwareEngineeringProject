#ifndef EQUALITYOPTOKEN_H
#define EQUALITYOPTOKEN_H

#include <iostream>
#include <string_view>

#include "RelationalOpToken.h"

/**
* This class represents a "==" symbol.
*/
class EqualityOpToken : public RelationalOpToken {
public:
	EqualityOpToken()
		: RelationalOpToken{ "=="sv } {};
};

#endif
