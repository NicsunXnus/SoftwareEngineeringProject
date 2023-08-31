#ifndef INTEGERLITERALTOKEN_H
#define INTEGERLITERALTOKEN_H

#include <string>
#include <string_view>

#include "LiteralToken.h"

/**
* This class represents the token for the integer literals, used in PQL declarations and in SIMPLE
*/
namespace Tokens {
	class IntegerLiteralToken : public LiteralToken {
	public:
		IntegerLiteralToken(std::string_view tokenName)
			: LiteralToken{ tokenName } {
		};
	};
}
#endif
