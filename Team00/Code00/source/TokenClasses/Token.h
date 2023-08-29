#ifndef TOKEN_H
#define TOKEN_H

#include <iostream>
#include <string_view>

/**
* This class represents all token objects
*/
class Token
{
private:
	// token represented in string form
	std::string m_tokenName;

public:
	Token(std::string_view tokenName)
		: m_tokenName{ tokenName } 
	{};
};

#endif
