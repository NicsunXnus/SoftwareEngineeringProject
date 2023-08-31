#ifndef CALLTOKENPQL_H
#define CALLTOKENPQL_H

#include <string>
#include <string_view>
#include "KeywordToken.h"

using namespace std::string_view_literals;

/**
* This class represents the assign token used in a PQL declaration
*/
class CallTokenPQL : public KeywordToken {
public:
	CallTokenPQL()
		: KeywordToken{ "call"sv } {};
};

#endif
