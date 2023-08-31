#ifndef EQUALSOPTOKEN_H
#define EQUALSOPTOKEN_H

#include <iostream>
#include <string_view>

#include "OperatorToken.h"

/**
* This class represents a "=" symbol.
* 
* Not to be confused with the equality operator "==" or its token.
*/
class EqualsOpToken : public OperatorToken {
public:
	EqualsOpToken()
		: OperatorToken{ "="sv } {};
};

#endif
