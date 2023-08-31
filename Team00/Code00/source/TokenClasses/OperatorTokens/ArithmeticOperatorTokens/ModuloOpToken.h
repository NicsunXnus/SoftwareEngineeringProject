#ifndef MODULOOPTOKEN_H
#define MODULOOPTOKEN_H

#include <iostream>
#include <string_view>

#include "ArithmeticOpToken.h"

/**
* This class represents a "%" symbol.
*/
class ModuloOpToken : public ArithmeticOpToken {
public:
	ModuloOpToken()
		: ArithmeticOpToken{ "%"sv } {};
};

#endif
