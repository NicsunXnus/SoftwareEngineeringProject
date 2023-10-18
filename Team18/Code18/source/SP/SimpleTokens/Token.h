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
		: tokenName{ tokenName } {};

	// Getter for the token name since the field is private.
	virtual std::string getName();

	// Overriden
	bool operator==(Token& other);

	// Checks if the two tokens supplied are the same runtime type and same stored tokenName.
	// The arguments supplied MUST be pointers.
	static bool checkEquality(Token* left, Token* right);

	// Overloaded method to take in shared_ptr instead of regular pointers
	static bool checkEquality(std::shared_ptr<Token> left, std::shared_ptr<Token> right);

	// Overloaded method to take in shared_ptr on the left instead of regular pointers
	static bool checkEquality(std::shared_ptr<Token> left, Token* right);

	// Overloaded method to take in shared_ptr on the right instead of regular pointers
	static bool checkEquality(Token* left, std::shared_ptr<Token> right);
};
#endif
