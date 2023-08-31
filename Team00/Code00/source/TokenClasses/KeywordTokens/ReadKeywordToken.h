#ifndef READKEYWORDTOKEN_H
#define READKEYWORDTOKEN_H

#include <string>
#include <string_view>

#include "KeywordToken.h"

using namespace std::string_view_literals;

/**
* This class represents the token for the "read" keyword, used in PQL declarations and in SIMPLE
*/
namespace Tokens {
	class ReadKeywordToken : public KeywordToken {
	public:
		ReadKeywordToken()
			: KeywordToken{ "read"sv } {
		};
	};
}
#endif
