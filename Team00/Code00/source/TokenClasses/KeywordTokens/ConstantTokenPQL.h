#ifndef CONSTANTTOKENPQL_H
#define CONSTANTTOKENPQL_H

#include <string>
#include <string_view>
#include "KeywordToken.h"

using namespace std::string_view_literals;

/**
* This class represents the assign token used in a PQL declaration
*/
class ConstantTokenPQL : public KeywordToken {
public:
	ConstantTokenPQL()
		: KeywordToken{ "constant"sv } {};
};

#endif
