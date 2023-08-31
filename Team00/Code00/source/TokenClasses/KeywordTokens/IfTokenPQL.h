#ifndef IFTOKENPQL_H
#define IFTOKENPQL_H

#include <string>
#include <string_view>
#include "KeywordToken.h"

using namespace std::string_view_literals;

/**
* This class represents the assign token used in a PQL declaration
*/
class IfTokenPQL : public KeywordToken {
public:
	IfTokenPQL()
		: KeywordToken{ "if"sv } {};
};

#endif
