#ifndef MORETHANOPTOKEN_H
#define MORETHANOPTOKEN_H

#include <iostream>
#include <string_view>

#include "RelationalOpToken.h"

using namespace std::string_view_literals;

/**
* This class represents a ">" symbol.
*/
namespace Tokens {
	class MoreThanOpToken : public RelationalOpToken {
	public:
		MoreThanOpToken()
			: RelationalOpToken{ ">"sv } {};
	};
}
#endif
