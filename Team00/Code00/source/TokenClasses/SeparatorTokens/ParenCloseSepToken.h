#ifndef PARENCLOSESEPTOKEN_H
#define PARENCLOSESEPTOKEN_H

#include <iostream>
#include <string_view>

#include "SeparatorToken.h"

/**
* This class represents a ")" symbol.
*/
class ParenCloseSepToken : public SeparatorToken {
public:
	ParenCloseSepToken()
		: SeparatorToken{ ")"sv } {};
};

#endif
