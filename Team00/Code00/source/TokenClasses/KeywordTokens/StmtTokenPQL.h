#ifndef STMTTOKENPQL_H
#define STMTTOKENPQL_H

#include <string>
#include <string_view>
#include "KeywordToken.h"

using namespace std::string_view_literals;

/**
* This class represents the stmt token used in a PQL declaration
*/
class StmtTokenPQL : public KeywordToken {
public:
	StmtTokenPQL()
		: KeywordToken{ "stmt"sv } {};
};

#endif