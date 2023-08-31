#ifndef STMTKEYWORDTOKEN_H
#define STMTKEYWORDTOKEN_H

#include <string>
#include <string_view>

#include "KeywordToken.h"

using namespace std::string_view_literals;

/**
* This class represents the token for the "stmt" keyword, used in PQL declarations
*/
class StmtKeywordToken : public KeywordToken {
public:
	StmtKeywordToken()
		: KeywordToken{ "stmt"sv } {
	};
};

#endif