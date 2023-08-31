#ifndef PROCEDURETOKEN_H
#define PROCEDURETOKEN_H

#include <string>
#include <string_view>
#include "KeywordToken.h"

using namespace std::string_view_literals;

/**
* This class represents the assign token
*/
class ProcedureToken : public KeywordToken {
public:
	ProcedureToken()
		: KeywordToken{ "procedure"sv } {};
};

#endif
