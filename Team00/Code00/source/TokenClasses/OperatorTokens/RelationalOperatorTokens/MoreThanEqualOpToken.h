#ifndef MORETHANEQUALOPTOKEN_H
#define MORETHANEQUALOPTOKEN_H

#include <iostream>
#include <string_view>

#include "RelationalOpToken.h"

using namespace std::string_view_literals;

/**
* This class represents a ">=" symbol.
*/
namespace Tokens {
	class MoreThanEqualOpToken : public RelationalOpToken {
	public:
		MoreThanEqualOpToken()
			: RelationalOpToken{ ">="sv } {};
	};
}
#endif
