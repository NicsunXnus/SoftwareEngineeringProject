#ifndef PRINTTOKENPQL_H
#define PRINTTOKENPQL_H

#include <string>
#include <string_view>
#include "KeywordToken.h"

using namespace std::string_view_literals;

/**
* This class represents the print token used in a PQL declaration
*/
class PrintTokenPQL : public KeywordToken {
public:
	PrintTokenPQL()
		: KeywordToken{ "print"sv } {};
};

#endif
