#ifndef READTOKENPQL_H
#define READTOKENPQL_H

#include <string>
#include <string_view>
#include "KeywordToken.h"

using namespace std::string_view_literals;

/**
* This class represents the read token used in a PQL declaration
*/
class ReadTokenPQL : public KeywordToken {
public:
	ReadTokenPQL()
		: KeywordToken{ "read"sv } {};
};

#endif
