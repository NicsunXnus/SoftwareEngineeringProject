#ifndef EQUALITYOPTOKEN_H
#define EQUALITYOPTOKEN_H

#include <iostream>
#include <string_view>

#include "RelationalOpToken.h"

using namespace std::string_view_literals;

/**
* This class represents a "==" symbol.
*/
namespace Tokens {
	class EqualityOpToken : public RelationalOpToken {
	public:
		EqualityOpToken()
			: RelationalOpToken{ "=="sv } {};
	};
}
#endif
