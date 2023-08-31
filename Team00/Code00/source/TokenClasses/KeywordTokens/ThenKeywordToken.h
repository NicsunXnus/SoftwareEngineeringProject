#ifndef THENKEYWORDTOKEN_H
#define THENKEYWORDTOKEN_H

#include <string>
#include <string_view>

#include "KeywordToken.h"

using namespace std::string_view_literals;

/**
* This class represents the token for the "then" keyword, in SIMPLE
*/
namespace Tokens {
	class ThenKeywordToken : public KeywordToken {
	public:
		ThenKeywordToken()
			: KeywordToken{ "then"sv } {
		};
	};
}
#endif
