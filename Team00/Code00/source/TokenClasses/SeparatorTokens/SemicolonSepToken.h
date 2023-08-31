#ifndef SEMICOLONSEPTOKEN_H
#define SEMICOLONSEPTOKEN_H

#include <iostream>
#include <string_view>

#include "SeparatorToken.h"

/**
* This class represents a ";" symbol.
*/
class SemicolonSepToken : public SeparatorToken {
public:
	SemicolonSepToken()
		: SeparatorToken{ ";"sv } {};
};

#endif
