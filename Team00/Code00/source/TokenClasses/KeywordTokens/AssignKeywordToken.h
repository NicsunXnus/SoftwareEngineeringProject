#ifndef ASSIGNKEYWORDTOKEN_H
#define ASSIGNKEYWORDTOKEN_H

#include <string>
#include <string_view>

#include "KeywordToken.h"

using namespace std::string_view_literals;

/**
* This class represents the token for the "assign" keyword, used in PQL declarations
*/
class AssignKeywordToken : public KeywordToken {
public:
	AssignKeywordToken()
		: KeywordToken{ "assign"sv } {};
};

#endif
