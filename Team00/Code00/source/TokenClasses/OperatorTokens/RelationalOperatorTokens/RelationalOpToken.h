#ifndef RELATIONALOPTOKEN_H
#define RELATIONALOPTOKEN_H

#include <iostream>
#include <string_view>

#include "../OperatorToken.h"

/**
* This class represents tokens objects that are relational operators between expressions
*/
namespace Tokens {
	class RelationalOpToken : public OperatorToken {
	public:
		RelationalOpToken(std::string_view tokenName)
			: OperatorToken{ tokenName } {
		};
	};
}
#endif
