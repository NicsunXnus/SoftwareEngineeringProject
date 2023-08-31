#ifndef CONSTANTKEYWORDTOKEN_H
#define CONSTANTKEYWORDTOKEN_H

#include <string>
#include <string_view>

#include "KeywordToken.h"

using namespace std::string_view_literals;

/**
* This class represents the token for the "constant" keyword, used in PQL declarations
*/
class ConstantKeywordToken : public KeywordToken {
public:
	ConstantKeywordToken()
		: KeywordToken{ "constant"sv } {
	};
};

#endif
