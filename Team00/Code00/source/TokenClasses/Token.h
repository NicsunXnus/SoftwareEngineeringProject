#ifndef TOKEN_H
#define TOKEN_H

#include <iostream>
#include <string_view>

/**
* This class represents all token objects
*/
class Token {
private:
	// token represented in string form
	std::string tokenName;

public:
	Token(std::string_view tokenName)
		: tokenName{ tokenName } {
	};
	/*
	* Returns string form of token
	*/
	std::string getTokenName() {
		return tokenName;
	}
};
#endif
