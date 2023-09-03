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

	
	// Overload == operator for token class comparisons for testing purposes
	bool operator==(Token& t) {
		std::string tName{ typeid(t).name() };
		std::string thisName{ typeid(t).name() };

		if (tName.compare(thisName)) {
			return false;
		}
		return this->tokenName == t.tokenName;
	}
	
};
#endif
