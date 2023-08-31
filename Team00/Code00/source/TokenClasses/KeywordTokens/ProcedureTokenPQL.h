#ifndef PROCEDURETOKENPQL_H
#define PROCEDURETOKENPQL_H

#include <string>
#include <string_view>
#include "KeywordToken.h"

using namespace std::string_view_literals;

/**
* This class represents the assign token used in a PQL declaration
*/
class ProcedureTokenPQL : public KeywordToken {
public:
	ProcedureTokenPQL()
		: KeywordToken{ "procedure"sv } {};
};

#endif
