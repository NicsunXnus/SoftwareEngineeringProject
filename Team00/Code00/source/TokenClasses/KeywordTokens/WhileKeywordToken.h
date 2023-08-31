#ifndef WHILEKEYWORDTOKEN_H
#define WHILEKEYWORDTOKEN_H

#include <string>
#include <string_view>

#include "KeywordToken.h"

using namespace std::string_view_literals;

/**
* This class represents the token for the "while" keyword, used in PQL declarations and in SIMPLE
*/
class WhileKeywordToken : public KeywordToken {
public:
	WhileKeywordToken()
		: KeywordToken{ "while"sv } {};
};

#endif
