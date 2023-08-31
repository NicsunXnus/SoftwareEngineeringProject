#ifndef ASSIGNTOKEN_H
#define ASSIGNTOKEN_H

#include <string>
#include <string_view>
#include "DeclarationToken.h"

using namespace std::string_view_literals;

/**
* This class represents the assign token used in a PQL declaration
*/
class AssignToken : public DeclarationToken {
public:
	AssignToken()
		: DeclarationToken{ "assign"sv }
	{};
};

#endif
