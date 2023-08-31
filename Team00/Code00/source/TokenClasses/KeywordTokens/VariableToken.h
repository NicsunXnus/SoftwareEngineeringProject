#ifndef VARIABLETOKEN_H
#define VARIABLETOKEN_H

#include <string>
#include <string_view>
#include "KeywordToken.h"

using namespace std::string_view_literals;

/**
* This class represents the variable token
*/
class VariableToken : public KeywordToken {
public:
	VariableToken()
		: KeywordToken{ "variable"sv } {};
};

#endif
