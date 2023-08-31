#ifndef WHILETOKENPQL_H
#define WHILETOKENPQL_H

#include <string>
#include <string_view>
#include "KeywordToken.h"

using namespace std::string_view_literals;

/**
* This class represents the assign token used in a PQL declaration
*/
class WhileTokenPQL : public KeywordToken {
public:
	WhileTokenPQL()
		: KeywordToken{ "while"sv } {};
};

#endif
