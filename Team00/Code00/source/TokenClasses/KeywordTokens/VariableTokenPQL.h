#ifndef VARIABLETOKEN_H
#define VARIABLETOKEN_H

#include <string>
#include <string_view>
#include "KeywordToken.h"

using namespace std::string_view_literals;

/**
* This class represents the assign token used in a PQL declaration
*/
class VariableTokenPQL : public KeywordToken {
public:
	VariableTokenPQL()
		: KeywordToken{ "variable"sv } {};
};

#endif
