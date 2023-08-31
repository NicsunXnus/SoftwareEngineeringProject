#ifndef ASSIGNTOKEN_H
#define ASSIGNTOKEN_H

#include <string>
#include <string_view>

#include "KeywordToken.h"

using namespace std::string_view_literals;

/**
* This class represents the assign token
*/
class AssignToken : public KeywordToken {
public:
	AssignToken()
		: KeywordToken{ "assign"sv } {};
};

#endif
