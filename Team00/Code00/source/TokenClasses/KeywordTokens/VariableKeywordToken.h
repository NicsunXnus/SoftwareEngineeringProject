#ifndef VARIABLEKEYWORDTOKEN_H
#define VARIABLEKEYWORDTOKEN_H

#include <string>
#include <string_view>

#include "KeywordToken.h"

using namespace std::string_view_literals;

/**
* This class represents the token for the "variable" keyword, used in PQL declarations
*/
class VariableKeywordToken : public KeywordToken {
public:
	VariableKeywordToken()
		: KeywordToken{ "variable"sv } {
	};
};

#endif
