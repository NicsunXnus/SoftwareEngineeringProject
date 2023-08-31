#ifndef PLUSOPTOKEN_H
#define PLUSOPTOKEN_H

#include <iostream>
#include <string_view>

#include "OperatorToken.h"

/**
* This class represents a "+" symbol.
*/
class PlusOpToken : public OperatorToken {
private:
public:
	PlusOpToken()
		: OperatorToken { "+"sv } {};
};

#endif
