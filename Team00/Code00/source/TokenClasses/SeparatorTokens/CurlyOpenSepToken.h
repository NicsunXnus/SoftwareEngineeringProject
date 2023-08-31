#ifndef CURLYOPENSEPTOKEN_H
#define CURLYOPENSEPTOKEN_H

#include <iostream>
#include <string_view>

#include "SeparatorToken.h"

/**
* This class represents a "{" symbol.
*/
class CurlyOpenSepToken : public SeparatorToken {
public:
	CurlyOpenSepToken()
		: SeparatorToken{ "{"sv } {};
};

#endif
