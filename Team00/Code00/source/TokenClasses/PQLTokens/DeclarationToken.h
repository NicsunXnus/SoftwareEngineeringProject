#ifndef DECLARATIONTOKEN_H
#define DECLARATIONTOKEN_H

#include <string_view>
#include "Token.h"

/**
* This class represents the tokens in a PQL declaration
*/
class DeclarationToken : public Token {
private:
public:
	DeclarationToken(std::string_view tokenName)
		: Token{ tokenName } {};

};

#endif
