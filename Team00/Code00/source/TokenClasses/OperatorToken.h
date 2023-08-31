#ifndef OPERATORTOKEN_H
#define OPERATORTOKEN_H

#include <iostream>
#include <string_view>

#include "Token.h"

/**
* This class represents tokens objects that are operators between expressions
*/
class OperatorToken : public Token {
private:
public:
	OperatorToken(std::string_view tokenName)
		: m_tokenName{tokenName} {
	};
};

#endif
