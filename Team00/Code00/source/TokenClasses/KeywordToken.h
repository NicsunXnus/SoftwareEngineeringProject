#ifndef KEYWORDTOKEN_H
#define KEYWORDTOKEN_H

#include <iostream>
#include <string_view>

#include "Token.h"

/**
* This class represents tokens objects that stem from reserved keywords 
*/
class KeywordToken : public Token {
private:
public:
	KeywordToken(std::string_view tokenName)
		: m_tokenName{tokenName} {
	};
};

#endif
