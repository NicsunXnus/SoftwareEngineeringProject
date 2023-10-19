#include "Token.h"

// Getter for the token name since the field is private.
std::string Token::getName() {
	return this->tokenName;
}
// Overriden
bool Token::operator==(Token& other) {
	Token* otherPtr = &other;
	return checkEquality(this, otherPtr);
}

// Checks if the two tokens supplied are the same runtime type and same stored tokenName.
// The arguments supplied MUST be pointers.
bool Token::checkEquality(Token* left, Token* right) {
	if (left == nullptr || right == nullptr) {
		return false;
	}
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
bool Token::checkEquality(std::shared_ptr<Token> left, std::shared_ptr<Token> right) {
	return checkEquality(left.get(), right.get());
}

// Overloaded method to take in shared_ptr on the left instead of regular pointers
bool Token::checkEquality(std::shared_ptr<Token> left, Token* right) {
	return checkEquality(left.get(), right);
}

// Overloaded method to take in shared_ptr on the right instead of regular pointers
bool Token::checkEquality(Token* left, std::shared_ptr<Token> right) {
	return checkEquality(left, right.get());
}
