#ifndef CALLKEYWORDTOKEN_H
#define CALLKEYWORDTOKEN_H

#include <string>
#include <string_view>

#include "KeywordToken.h"

using namespace std::string_view_literals;

/**
* This class represents the token for the "call" keyword, used in PQL declarations and in SIMPLE
*/
class CallKeywordToken : public KeywordToken {
public:
	CallKeywordToken()
		: KeywordToken{ "call"sv } {
	};
};

#endif
