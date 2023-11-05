#ifndef TOKEN_H
#define TOKEN_H

#include <iostream>
#include <string_view>
#include <typeinfo>

using namespace std;

/**
 * This class represents all token objects
 */
class Token {
private:
	// token represented in string form
	string tokenName;
public:
	Token(string_view tokenName)
		: tokenName{ tokenName } {};

	// Getter for the token name since the field is private.
	virtual string getName();

	// Overriden
	bool operator==(Token& other);

	// Checks if the two tokens supplied are the same runtime type and same stored tokenName.
	static bool checkEquality(shared_ptr<Token> left, shared_ptr<Token> right);
};
#endif
