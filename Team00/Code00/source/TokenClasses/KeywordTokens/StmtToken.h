#ifndef STMTTOKEN_H
#define STMTTOKEN_H

#include <string>
#include <string_view>
#include "KeywordToken.h"

using namespace std::string_view_literals;

/**
* This class represents the stmt token
*/
class StmtToken : public KeywordToken {
public:
	StmtToken()
		: KeywordToken{ "stmt"sv } {};
};

#endif