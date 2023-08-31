#ifndef PRINTKEYWORDTOKEN_H
#define PRINTKEYWORDTOKEN_H

#include <string>
#include <string_view>

#include "KeywordToken.h"

using namespace std::string_view_literals;

/**
* This class represents the token for the "print" keyword, used in PQL declarations and in SIMPLE
*/
namespace Tokens {
	class PrintKeywordToken : public KeywordToken {
	public:
		PrintKeywordToken()
			: KeywordToken{ "print"sv } {
		};
	};
}
#endif
