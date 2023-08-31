#ifndef INEQUALITYOPTOKEN_H
#define INEQUALITYOPTOKEN_H

#include <iostream>
#include <string_view>

#include "RelationalOpToken.h"

using namespace std::string_view_literals;

/**
* This class represents a "!=" symbol.
*/
class InequalityOpToken : public RelationalOpToken {
public:
	InequalityOpToken()
		: RelationalOpToken{ "!="sv } {};
};

#endif
