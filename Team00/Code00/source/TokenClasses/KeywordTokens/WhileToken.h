#ifndef WHILETOKEN_H
#define WHILETOKEN_H

#include <string>
#include <string_view>
#include "KeywordToken.h"

using namespace std::string_view_literals;

/**
* This class represents the while token
*/
class WhileToken : public KeywordToken {
public:
	WhileToken()
		: KeywordToken{ "while"sv } {};
};

#endif
