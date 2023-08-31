#ifndef CONSTANTTOKEN_H
#define CONSTANTTOKEN_H

#include <string>
#include <string_view>

#include "KeywordToken.h"

using namespace std::string_view_literals;

/**
* This class represents the assign token used in a PQL declaration
*/
class ConstantToken : public KeywordToken {
public:
	ConstantToken()
		: KeywordToken{ "constant"sv } {};
};

#endif
