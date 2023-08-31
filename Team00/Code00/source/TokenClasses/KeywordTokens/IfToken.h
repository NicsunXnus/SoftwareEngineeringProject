#ifndef IFTOKEN_H
#define IFTOKEN_H

#include <string>
#include <string_view>

#include "KeywordToken.h"

using namespace std::string_view_literals;

/**
* This class represents the assign token used in a PQL declaration
*/
class IfToken : public KeywordToken {
public:
	IfToken()
		: KeywordToken{ "if"sv } {};
};

#endif
