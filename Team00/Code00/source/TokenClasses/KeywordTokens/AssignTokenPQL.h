#ifndef ASSIGNTOKENPQL_H
#define ASSIGNTOKENPQL_H

#include <string>
#include <string_view>
#include "KeywordToken.h"

using namespace std::string_view_literals;

/**
* This class represents the assign token used in a PQL declaration
*/
class AssignTokenPQL : public KeywordToken {
public:
	AssignTokenPQL()
		: KeywordToken{ "assign"sv } {};
};

#endif
