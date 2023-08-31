#ifndef IFKEYWORDTOKEN_H
#define IFKEYWORDTOKEN_H

#include <string>
#include <string_view>

#include "KeywordToken.h"

using namespace std::string_view_literals;

/**
* This class represents the token for the "if" keyword, used in PQL declarations and in SIMPLE
*/
class IfKeywordToken : public KeywordToken {
public:
	IfKeywordToken()
		: KeywordToken{ "if"sv } {};
};

#endif
