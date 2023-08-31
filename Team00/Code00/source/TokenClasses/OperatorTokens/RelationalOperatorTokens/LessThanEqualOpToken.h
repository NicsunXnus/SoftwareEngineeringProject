#ifndef LESSTHANEQUALOPTOKEN_H
#define LESSTHANEQUALOPTOKEN_H

#include <iostream>
#include <string_view>

#include "RelationalOpToken.h"

using namespace std::string_view_literals;

/**
* This class represents a "<=" symbol.
*/
namespace Tokens {
	class LessThanEqualOpToken : public RelationalOpToken {
	public:
		LessThanEqualOpToken()
			: RelationalOpToken{ "<="sv } {};
	};
}
#endif
