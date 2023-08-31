#ifndef PROCEDUREKEYWORDTOKEN_H
#define PROCEDUREKEYWORDTOKEN_H

#include <string>
#include <string_view>

#include "KeywordToken.h"

using namespace std::string_view_literals;

/**
* This class represents the token for the "procedure" keyword, used in PQL declarations and in SIMPLE
*/
namespace Tokens {
	class ProcedureKeywordToken : public KeywordToken {
	public:
		ProcedureKeywordToken()
			: KeywordToken{ "procedure"sv } {
		};
	};
}
#endif
