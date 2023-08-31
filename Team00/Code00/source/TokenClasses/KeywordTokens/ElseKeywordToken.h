#ifndef ELSEKEYWORDTOKEN_H
#define ELSEKEYWORDTOKEN_H

#include <string>
#include <string_view>

#include "KeywordToken.h"

using namespace std::string_view_literals;

/**
* This class represents the token for the "then" keyword, in SIMPLE
*/
class ElseKeywordToken : public KeywordToken {
public:
	ElseKeywordToken()
		: KeywordToken{ "else"sv } {};
};

#endif
