#ifndef TOKEN_H
#define TOKEN_H

#include <iostream>
#include <string_view>
#include <typeinfo>

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
	// Getter for the token name since the field is private.
	virtual std::string getName() {
		return this->tokenName;
	}

	// Checks if the two tokens supplied are the same runtime type and same stored tokenName.
	static bool checkEquality(Token* left, Token* right) {
		std::string leftId = typeid(*left).name();
		std::string rightId = typeid(*right).name();
		bool sameType = leftId == rightId;
		if (!sameType) {
			return false;
		}
		std::string leftName = left->tokenName;
		std::string rightName = right->tokenName;
		return leftName == rightName;
	}

	// Overloaded method to take in shared_ptr instead of regular pointers
	static bool checkEquality(std::shared_ptr<Token> left, std::shared_ptr<Token> right) {
		return checkEquality(left.get(), right.get());
	}
};
#endif
