#ifndef PRINTTOKEN_H
#define PRINTTOKEN_H

#include <string>
#include <string_view>
#include "KeywordToken.h"

using namespace std::string_view_literals;

/**
* This class represents the print token
*/
class PrintToken : public KeywordToken {
public:
	PrintToken()
		: KeywordToken{ "print"sv } {};
};

#endif
