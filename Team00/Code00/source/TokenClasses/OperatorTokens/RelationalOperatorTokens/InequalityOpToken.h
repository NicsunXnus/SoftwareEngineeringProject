#ifndef INEQUALITYOPTOKEN_H
#define INEQUALITYOPTOKEN_H

#include <iostream>
#include <string_view>

#include "RelationalOpToken.h"

/**
* This class represents a "!=" symbol.
*/
class InequalityOpToken : public RelationalOpToken {
public:
	InequalityOpToken()
		: RelationalOpToken{ "!="sv } {};
};

#endif
